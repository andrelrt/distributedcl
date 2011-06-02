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
#ifndef _DCL_NETWORK_SESSION_H_
#define _DCL_NETWORK_SESSION_H_

#include "distributedcl_internal.h"
#include "message/packet.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace platform {
//-----------------------------------------------------------------------------
template< template< class > class COMM >
class session
{
public:
    typedef COMM< session > communication_t;
    typedef typename COMM< session >::config_info_t config_info_t;

    static const session_id_t invalid_session = 0xffff;

    ~session(){}

	inline session_id_t get_session_id() const
    {
        return session_id_;
    }

protected:
    // create session to a server
    session( const config_info_t& config ) :
    	session_id_( invalid_session ), communication_( config ),
        sequence_number_( 0 ), remote_sequence_number_( 0 )
    {
    }

	inline void set_session_id( session_id_t id )
    {
        session_id_ = id;
    }

    inline session_sequence_t get_sequence_number()
    {
        return sequence_number_++;
    }

    inline void set_sequence_number( session_sequence_t number )
    {
        sequence_number_ = number;
    }

    inline session_sequence_t get_remote_sequence_number()
    {
        return remote_sequence_number_++;
    }

    inline void set_remote_sequence_number( session_sequence_t number )
    {
        remote_sequence_number_ = number;
    }

    inline communication_t& get_communication()
    {
        return communication_;
    }

    inline dcl::network::message::packet* receive_packet()
    {
        // Receive buffer
        std::size_t recv_len;
        uint8_t* buffer_ptr = communication_.receive_message( &recv_len );

        // Parse packet
        dcl::network::message::packet* packet_ptr = new dcl::network::message::packet( buffer_ptr, recv_len );
        packet_ptr->parse();

        if( packet_ptr->get_session_id() != session_id_ )
        {
	    	throw dcl::library_exception( "Invalid session" );
    	}

		if( packet_ptr->get_sequence_number() != get_remote_sequence_number() )
		{
            remote_sequence_number_--;
			throw dcl::library_exception( "Invalid sequence number" );
    	}

        return( packet_ptr );
    }

    inline dcl::network::message::packet* create_packet()
    {
        std::size_t max_length = 0;
        uint8_t* buffer_ptr = communication_.get_message_buffer( &max_length );

        return( new dcl::network::message::packet( buffer_ptr, max_length, get_session_id(),
                                                   get_sequence_number() ) );
    }

    inline void send_packet( dcl::network::message::packet* packet_ptr )
    {
        packet_ptr->create_packet();
        communication_.send_message( packet_ptr->get_length() );
    }

private:
    session_id_t session_id_;
    communication_t communication_;
    session_sequence_t sequence_number_;
    session_sequence_t remote_sequence_number_;
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::platform;
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_SESSION_H_
