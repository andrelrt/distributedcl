/*
 * Copyright (c) 2009-2012 André Tupinambá (andrelrt@gmail.com)
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
#ifndef _DCL_NETWORK_SERVER_SESSION_H_
#define _DCL_NETWORK_SERVER_SESSION_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <iostream>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include "distributedcl_internal.h"
#include "session.h"
#include "server/message_dispatcher.h"
#include "message/packet.h"
#include "message/msg_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
class server_platform;
}}
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace server {
//-----------------------------------------------------------------------------
class server_session_context
{
public:
    inline void add_waiting_message( message_sp_t message_sp )
    {
        dcl::scoped_lock_t lock( waiting_messages_mutex_ );

        waiting_messages_.push_back( message_sp );
    }

    inline dcl::server::server_platform& get_server_platform()
    {
        return *server_platform_sp_;
    }

    inline void attach_server( dcl::remote_id_t id )
    {
        server_session_context* server_ptr = session_objects_.get( id );
        server_platform_sp_ = server_ptr->server_platform_sp_;
    }

    inline dcl::remote_id_t get_server()
    {
        return session_objects_.get( this );
    }

    static const char* get_name() { return "server_session_context"; }

protected:
    dcl::mutex_t waiting_messages_mutex_;
    dcl::message_vector_t waiting_messages_;
    boost::shared_ptr<dcl::server::server_platform> server_platform_sp_;

    static dcl::info::object_manager< server_session_context > session_objects_;

    server_session_context();
    virtual ~server_session_context();
};
//-----------------------------------------------------------------------------
template< template< class > class COMM >
class server_session :
    public dcl::network::platform::session< COMM >,
    public server_session_context
{
public:
	typedef typename dcl::network::platform::session< COMM > session_t;
	typedef typename session_t::config_info_t config_info_t;

    server_session( const config_info_t& config, boost::interprocess::interprocess_semaphore& observer_semaphore ) :
        session_t( config ), running_( false ), receive_thread_ptr_( NULL ), observer_semaphore_( observer_semaphore )
    {}

    virtual ~server_session()
    {
        shutdown();
    }

    inline void startup()
    {
        if( receive_thread_ptr_ == NULL )
        {
            session_t::get_communication().startup( this );

            receive_thread_ptr_ = new boost::thread( &dcl::network::server::server_session< COMM >::start_thread, this );
        }
    }

    inline void shutdown()
    {
        if( receive_thread_ptr_ != NULL )
        {
            receive_thread_ptr_->interrupt();

            session_t::get_communication().close();

            receive_thread_ptr_->join();

            delete receive_thread_ptr_;
            receive_thread_ptr_ = NULL;
        }

        session_t::get_communication().shutdown();
    }

    inline bool running()
    {
        return running_;
    }

private:
    bool running_;
    boost::thread* receive_thread_ptr_;
    dcl::server::message_dispatcher dispatcher_;
    boost::interprocess::interprocess_semaphore& observer_semaphore_;

    void start_thread()
    {
        running_ = true;
        receive_thread();
        
        running_ = false;
        observer_semaphore_.post();
    }

    void receive_thread()
    {
        std::cerr << "Client connected" << std::endl;

        //TODO: send hankshake base_messages
        session_t::set_session_id( 1 );
        session_t::set_sequence_number( 100 );
        session_t::set_remote_sequence_number( 200 );

        while( !boost::this_thread::interruption_requested() )
        {
            packet_sp_t recv_packet_sp;
            try
            {
                // Receive packet
                recv_packet_sp = session_t::receive_packet();
            }
            catch( dcl::library_exception& )
            {
                // Connection reset, close session
                std::cout << "Connection reset, close session" << std::endl;
                return;
            }

            if( boost::this_thread::interruption_requested() )
                return;

            // Create response packet
            packet_sp_t ret_packet_sp( session_t::create_packet() );

            // Execute packet base_messages
            try
            {
                recv_packet_sp->parse( true );

                dcl::message_vector_t& messages_ref = recv_packet_sp->get_messages();

                // Execute all messages in package
                dispatcher_.dispatch_messages( messages_ref, this );
                dispatcher_.wait_messages( messages_ref, this );

                // Create return package
                dcl::message_vector_t::iterator it;

                {
                    dcl::scoped_lock_t lock( waiting_messages_mutex_ );

                    for( it = waiting_messages_.begin(); it != waiting_messages_.end(); ++it )
                    {
                        if( (*it)->waiting_response() )
                        {
                            (*it)->set_response_mode();

                            ret_packet_sp->add( *it );
                        }
                    }

                    waiting_messages_.clear();
                }

                for( it = messages_ref.begin(); it != messages_ref.end(); ++it )
                {
                    if( (!(*it)->is_async()) &&
                        ((*it)->waiting_response()) )
                    {
                        (*it)->set_response_mode();

                        ret_packet_sp->add( *it );
                    }
                }

                if( ret_packet_sp->get_messages().empty() )
                {
                    message_sp_t
                        ret_msg_sp( new dcl::network::message::dcl_message< dcl::network::message::msg_error_message >( CL_SUCCESS ) );

                    ret_packet_sp->add( ret_msg_sp );
                }
            }
            catch( dcl::library_exception& ex )
            {
                std::cerr << "--- exception in dispacher: " << ex.get_error() << " ---" << std::endl;
                message_sp_t
                    ret_msg_sp( new dcl::network::message::dcl_message< dcl::network::message::msg_error_message >( ex.get_error() ) );

                ret_packet_sp->add( ret_msg_sp );
            }

            if( boost::this_thread::interruption_requested() )
                return;

            try
            {
                // Send response
                session_t::send_packet( ret_packet_sp, true );
            }
            catch( dcl::library_exception& )
            {
                // Connection reset, close session
                std::cout << "Connection reset, close session" << std::endl;
                return;
            }

            // Set semaphore
			dispatcher_.flush_async_queue( this );
        }
    }
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::server
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_SERVER_SESSION_H_
