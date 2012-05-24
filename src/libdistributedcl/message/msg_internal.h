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
#ifndef _DCL_MESSAGE_INTERNAL_H_
#define _DCL_MESSAGE_INTERNAL_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
template<>
class dcl_message< msg_error_message > : public base_message
{
public:
    static dcl_message< msg_error_message > success;

    dcl_message< msg_error_message >( int32_t error_code = CL_SUCCESS ) :
        base_message( msg_error_message, true, 0, sizeof( uint32_t ) ), error_code_( error_code )
    {
        set_response_mode();
    }

    inline int32_t get_error_code() const
    {
        return error_code_;
    }

    virtual void create_response( void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

private:
    int32_t error_code_;
};
//-----------------------------------------------------------------------------
template<>
class dcl_message< msg_flush_server > : public base_message
{
public:
    dcl_message< msg_flush_server >() :
        base_message( msg_flush_server, true ){}
};
//-----------------------------------------------------------------------------
template<>
class dcl_message< msg_dummy_message > : public base_message
{
public:
    dcl_message< msg_dummy_message >() :
        base_message( msg_dummy_message, false ){}
};
//-----------------------------------------------------------------------------
template<>
class dcl_message< msg_attach_context > : public base_message
{
public:
    dcl_message< msg_attach_context >() :
        base_message( msg_attach_context, false, sizeof(dcl::remote_id_t) ){}

	// Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )

    virtual void create_response( void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

private:
    dcl::remote_id_t remote_id_;
};
//-----------------------------------------------------------------------------
template< message_type MESSAGE_NUMBER >
class release_message : public base_message
{
public:
	release_message() :
		base_message( MESSAGE_NUMBER, false, sizeof(dcl::remote_id_t) ){}

	// Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )

private:
    dcl::remote_id_t remote_id_;

    virtual void create_request( void* payload_ptr )
    {
		dcl::remote_id_t* request_ptr = reinterpret_cast< dcl::remote_id_t* >( payload_ptr );

		*request_ptr = host_to_network( remote_id_ );
	}

    virtual void parse_request( const void* payload_ptr )
    {
		const dcl::remote_id_t* request_ptr = reinterpret_cast< const dcl::remote_id_t* >( payload_ptr );

		remote_id_ = network_to_host( *request_ptr );
	}
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_MESSAGE_INTERNAL_H_
