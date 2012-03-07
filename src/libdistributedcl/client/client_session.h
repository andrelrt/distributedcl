/*
 * Copyright (c) 2009-2011 Andr� Tupinamb� (andrelrt@gmail.com)
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
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/scoped_ptr.hpp>
#include "distributedcl_internal.h"
#include "network/session.h"
#include "message/message.h"
#include "message/packet.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace client {
//-----------------------------------------------------------------------------
template< template< class > class COMM >
class client_session : public dcl::network::platform::session< COMM >
{
public:
    client_session( const typename session< COMM >::config_info_t& config ) :
        session< COMM >( config )
    {
        session< COMM >::get_communication().startup( this );
    }

    ~client_session()
    {
        session< COMM >::get_communication().shutdown();
        clear_received_messages();
    }

    inline void connect()
    {
        session< COMM >::get_communication().connect();

        //TODO: send handshake base_messages
        session< COMM >::set_session_id( 1 );
        session< COMM >::set_sequence_number( 1 );
        session< COMM >::set_remote_sequence_number( 1 );
    }

    void flush_queue()
    {
        scoped_lock_t lock( mutex_ );

        boost::scoped_ptr< dcl::network::message::packet > packet_ptr( session< COMM >::create_packet() );

        while( !message_queue_.empty() )
        {
            dcl::network::message::base_message* message_ptr = message_queue_.front();

            packet_ptr->add( message_ptr );
            message_queue_.pop();
        }

        // Send data
        session< COMM >::send_packet( packet_ptr.get() );
        boost::scoped_ptr< dcl::network::message::packet > recv_packet_ptr;

        try
        {
            // Wait response
            recv_packet_ptr.reset( session< COMM >::receive_packet() );
        }
        catch( dcl::library_exception& )
        {
            // Connection reset, reconnect
            // TODO: Reconnect
        }

        if( recv_packet_ptr->get_message_count() != 1 )
        {
            throw new dcl::library_exception( "Invalid received base_message count" );
        }

        // Fill internal received base_messages
        clear_received_messages();

        recv_packet_ptr->parse_messages();

        dcl::network::message::message_vector_t::iterator recv_message_it = recv_packet_ptr->get_messages().begin();

        if( (*recv_message_it)->get_type() == dcl::network::message::msg_error_message )
        {
            received_messages_.insert( received_messages_.end(),
                                       recv_packet_ptr->get_messages().begin(),
                                       recv_packet_ptr->get_messages().end() );
        }
        else
        {
            dcl::network::message::message_vector_t::iterator sent_message_it = packet_ptr->get_messages().end();
            sent_message_it--;

            if( (*sent_message_it)->waiting_response() )
            {
                (*sent_message_it)->set_response( (*recv_message_it) );
            }
        }
    }

    void enqueue_message( dcl::network::message::base_message* message_ptr )
    {
        scoped_lock_t lock( mutex_ );

        message_queue_.push( message_ptr );
    }

    inline void send_message( dcl::network::message::base_message* message_ptr )
    {
        enqueue_message( message_ptr );
        flush_queue();
    }

    inline dcl::network::message::message_vector_t& get_received_messages()
    {
        return received_messages_;
    }

private:
    typedef std::queue< dcl::network::message::base_message* > message_queue_t;
    typedef boost::interprocess::scoped_lock< boost::interprocess::interprocess_mutex > scoped_lock_t;

    message_queue_t message_queue_;
    boost::interprocess::interprocess_mutex mutex_;
    dcl::network::message::message_vector_t received_messages_;

    void clear_received_messages()
    {
        dcl::network::message::message_vector_t::iterator it;
        for( it = received_messages_.begin(); it != received_messages_.end(); it++ )
        {
            delete( *it );
        }

        received_messages_.clear();
    }
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::client
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_CLIENT_SESSION_H_
