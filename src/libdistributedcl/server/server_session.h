/*
 * Copyright (c) 2009-2011 André Tupinambá (andrelrt@gmail.com)
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
#include "message/packet.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace server {
//-----------------------------------------------------------------------------
template< template< class > class COMM >
class server_session : public session< COMM >
{
public:
	typedef typename session< COMM >::config_info_t config_info_t;

    server_session( const config_info_t& config ) :
        session< COMM >( config ), receive_thread_ptr_( NULL )
    {}

    ~server_session()
    {
        shutdown();
    }

    inline void startup()
    {
        if( receive_thread_ptr_ == NULL )
        {
            session< COMM >::get_communication().startup( this );

            receive_thread_ptr_ = new boost::thread( &ocg::server::server_session< COMM >::receive_thread, this );
        }
    }

    inline void shutdown()
    {
        if( receive_thread_ptr_ != NULL )
        {
            receive_thread_ptr_->interrupt();

            session< COMM >::get_communication().close();

            receive_thread_ptr_->join();

            delete receive_thread_ptr_;
            receive_thread_ptr_ = NULL;
        }

        session< COMM >::get_communication().shutdown();
    }

private:
    boost::thread* receive_thread_ptr_;
    message_dispatcher dispatcher_;

    void receive_thread()
    {
        //TODO: send hankshake base_messages
        session< COMM >::set_session_id( 1 );
        session< COMM >::set_sequence_number( 1 );
        session< COMM >::set_remote_sequence_number( 1 );

        while( !boost::this_thread::interruption_requested() )
        {
            // Receive packet
            boost::scoped_ptr< packet > recv_packet( session< COMM >::receive_packet() );

            if( boost::this_thread::interruption_requested() )
                break;

            // Create response packet
            boost::scoped_ptr< packet > ret_packet( session< COMM >::create_packet() );

            // Execute packet base_messages
            try
            {
                recv_packet->parse_messages();
                dispatcher_.dispatch_messages( recv_packet->get_messages() );

                ret_packet->add( &msg_error::success );
            }
            catch( dcl::library_exception& ex )
            {
                ret_packet->add( new msg_error( ex.get_error_code() ) );
            }

            ret_packet->create_packet();

            if( boost::this_thread::interruption_requested() )
                break;

            // Send response
            session< COMM >::send_packet( ret_packet.get() );
        }
    }
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::server
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_SERVER_SESSION_H_
