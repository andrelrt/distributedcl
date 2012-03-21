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

#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include "distributedcl_internal.h"
#include "message_dispatcher.h"
#include "network/session.h"
#include "message/packet.h"
#include "message/msg_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace server {
//-----------------------------------------------------------------------------
template< template< class > class COMM >
class server_session : public dcl::network::platform::session< COMM >
{
public:
	typedef typename dcl::network::platform::session< COMM > session_t;
	typedef typename session_t::config_info_t config_info_t;

    server_session( const config_info_t& config ) :
        session_t( config ), receive_thread_ptr_( NULL )
    {}

    ~server_session()
    {
        shutdown();
    }

    inline void startup()
    {
        if( receive_thread_ptr_ == NULL )
        {
            session_t::get_communication().startup( this );

            receive_thread_ptr_ = new boost::thread( &dcl::network::server::server_session< COMM >::receive_thread, this );
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

private:
    boost::thread* receive_thread_ptr_;
    message_dispatcher dispatcher_;

    void receive_thread()
    {
        //TODO: send hankshake base_messages
        session_t::set_session_id( 1 );
        session_t::set_sequence_number( 1 );
        session_t::set_remote_sequence_number( 1 );

        while( !boost::this_thread::interruption_requested() )
        {
            boost::scoped_ptr< dcl::network::message::packet > recv_packet;
            try
            {
                // Receive packet
                recv_packet.reset( session_t::receive_packet() );
            }
            catch( dcl::library_exception& )
            {
                // Connection reset, close session
                break;
            }

            if( boost::this_thread::interruption_requested() )
                break;

            // Create response packet
            boost::scoped_ptr< dcl::network::message::packet > ret_packet( session_t::create_packet() );

            // Execute packet base_messages
            try
            {
                recv_packet->parse_messages();

                dcl::network::message::message_vector_t messages_ref = recv_packet->get_messages();

                dispatcher_.dispatch_messages( messages_ref );

                if( messages_ref.back()->waiting_response() )
                {
                    dcl::network::message::message_vector_t::reference ref = messages_ref.back();

                    ref->set_response_mode();

                    ret_packet->add( ref );
                }
                else
                {
                    ret_packet->add( new dcl::network::message::dcl_message< dcl::network::message::msg_error_message >( CL_SUCCESS ) );
                }
            }
            catch( dcl::library_exception& ex )
            {
                ret_packet->add( new dcl::network::message::dcl_message< dcl::network::message::msg_error_message >( ex.get_error() ) );
            }

            if( boost::this_thread::interruption_requested() )
                break;

            try
            {
                // Send response
                session_t::send_packet( ret_packet.get() );
            }
            catch( dcl::library_exception& )
            {
                // Connection reset, close session
                break;
            }
        }
    }
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::server
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_SERVER_SESSION_H_
