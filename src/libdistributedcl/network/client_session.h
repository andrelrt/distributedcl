/*
 * Copyright (c) 2009-2012 Andr� Tupinamb� (andrelrt@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
//-----------------------------------------------------------------------------
#ifndef _DCL_NETWORK_CLIENT_SESSION_H_
#define _DCL_NETWORK_CLIENT_SESSION_H_

#include <queue>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_barrier.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "distributedcl_internal.h"
#include "network/session.h"
#include "message/message.h"
#include "message/packet.h"
#include "message/msg_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace client {
//-----------------------------------------------------------------------------
template< template< class > class COMM >
class client_session : public dcl::network::platform::session< COMM >
{
public:
    client_session( const typename dcl::network::platform::session< COMM >::config_info_t& config ) :
        dcl::network::platform::session< COMM >( config ), running_( true ), send_semaphore_( 0 )
    {
        dcl::network::platform::session< COMM >::get_communication().startup( this );
        send_thread_sp_.reset( new boost::thread( &dcl::network::client::client_session<COMM>::send_thread, this ) );
    }

    ~client_session()
    {
        running_ = false;
        send_semaphore_.post();
        send_thread_sp_->join();

        dcl::network::platform::session< COMM >::get_communication().shutdown();
        clear_received_messages();
    }

    inline void connect()
    {
        dcl::network::platform::session< COMM >::get_communication().connect();

        //TODO: send handshake base_messages
        dcl::network::platform::session< COMM >::set_session_id( 1 );
        dcl::network::platform::session< COMM >::set_sequence_number( 200 );
        dcl::network::platform::session< COMM >::set_remote_sequence_number( 100 );
    }

    inline bool queue_empty() const
    {
        return message_queue_.empty();
    }

    inline void async_send_message( message_sp_t message_sp )
    {
        scoped_lock_t lock( queue_mutex_ );

        message_queue_.push( message_sp );

        send_semaphore_.post();
    }

    inline void async_flush_queue()
    {
        send_semaphore_.post();
    }

    inline void flush_queue()
    {
        scoped_lock_t lock( queue_mutex_ );

        if( message_queue_.empty() )
            return;

        packet_sp_t packet_sp( dcl::network::platform::session< COMM >::create_packet() );

        while( !message_queue_.empty() )
        {
            message_sp_t message_sp = message_queue_.front();

            packet_sp->add( message_sp );
            pending_messages_.insert( message_map_t::value_type( message_sp->get_id(), message_sp ) );
            message_queue_.pop();
        }

        flush_packet( packet_sp );
    }

    inline void wait()
    {
        wait_response_.wait();
    }

    inline void enqueue_message( message_sp_t message_sp )
    {
        scoped_lock_t lock( queue_mutex_ );

        message_queue_.push( message_sp );
    }

    inline void send_message( message_sp_t message_sp )
    {
        scoped_lock_t lock( queue_mutex_ );

        message_queue_.push( message_sp );

        packet_sp_t packet_sp( dcl::network::platform::session< COMM >::create_packet() );

        while( !message_queue_.empty() )
        {
            message_sp_t message_sp = message_queue_.front();

            packet_sp->add( message_sp );
            pending_messages_.insert( message_map_t::value_type( message_sp->get_id(), message_sp ) );
            message_queue_.pop();
        }

        flush_packet( packet_sp );
    }

    inline dcl::message_vector_t& get_received_messages()
    {
        return received_messages_;
    }

    static void setup_barrier( uint32_t count )
    {
        async_barrier_sp_.reset( new boost::interprocess::barrier( count + 1 ) );
    }

    static void wait_all()
    {
        async_barrier_sp_.wait();
    }

private:
    typedef std::queue< message_sp_t > message_queue_t;
    typedef std::map< uint16_t, message_sp_t > message_map_t;

    static boost::scoped_ptr<boost::interprocess::barrier> async_barrier_sp_;

    bool running_;
    dcl::mutex_t queue_mutex_;
    message_queue_t message_queue_;
    message_map_t pending_messages_;
    dcl::message_vector_t received_messages_;
    boost::scoped_ptr<boost::thread> send_thread_sp_;
    boost::interprocess::interprocess_semaphore send_semaphore_;

    void flush_packet( packet_sp_t packet_sp )
    {
        // Send data
        dcl::network::platform::session< COMM >::send_packet( packet_sp );
        packet_sp_t recv_packet_sp;

        try
        {
            // Wait response
            recv_packet_sp = dcl::network::platform::session< COMM >::receive_packet();
        }
        catch( dcl::library_exception& )
        {
            // Connection reset, reconnect
            // TODO: Reconnect
            return;
        }
       
        // Fill internal received base_messages
        clear_received_messages();

        recv_packet_sp->parse( false );

        dcl::message_vector_t::iterator recv_message_it;
        dcl::message_vector_t& recv_messages = recv_packet_sp->get_messages();

        for( recv_message_it = recv_messages.begin(); recv_message_it != recv_messages.end(); recv_message_it++ )
        {
            if( (*recv_message_it)->get_type() == dcl::network::message::msg_error_message )
            {
                (*recv_message_it)->parse_response( (*recv_message_it)->get_payload() );

                dcl::network::message::dcl_message< dcl::network::message::msg_error_message >* error_message_ptr =
                    reinterpret_cast<dcl::network::message::dcl_message< dcl::network::message::msg_error_message >*>( (*recv_message_it).get() );

                if( error_message_ptr->get_error_code() != CL_SUCCESS )
                {
                    throw dcl::library_exception( "Received exception", error_message_ptr->get_error_code() );
                }

                received_messages_.push_back( *recv_message_it );
            }
            else
            {
                message_map_t::iterator it = pending_messages_.find( (*recv_message_it)->get_id() );

                if( it != pending_messages_.end() )
                {
                    it->second->parse_response( (*recv_message_it)->get_payload() );

                    pending_messages_.erase( it );
                }
            }
        }
    }

    inline void clear_received_messages()
    {
        received_messages_.clear();
    }

    void send_thread()
    {
        while( 1 )
        {
            send_semaphore_.wait();

            if( !running_ )
                return;

            flush_queue();

            async_barrier_sp_->wait();
        }
    }
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::client
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_CLIENT_SESSION_H_
