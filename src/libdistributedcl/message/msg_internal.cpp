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
#include "msg_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
dcl_message< msg_error_message > dcl_message< msg_error_message >::success( CL_SUCCESS );
//-----------------------------------------------------------------------------
void dcl_message< msg_error_message >::create_response( void* payload_ptr )
{
    uint32_t* response_ptr = reinterpret_cast< uint32_t* >( payload_ptr );
    uint32_t* error_code_ptr = reinterpret_cast< uint32_t* >( &error_code_ );

    *response_ptr = host_to_network( *error_code_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msg_error_message >::parse_response( const void* payload_ptr )
{
    const uint32_t* response_ptr = 
        reinterpret_cast< const uint32_t* >( payload_ptr );

    uint32_t* error_code_ptr = reinterpret_cast< uint32_t* >( &error_code_ );

    *error_code_ptr = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msg_get_context >::create_response( void* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast<remote_id_t*>( payload_ptr );

    *response_ptr = host_to_network( remote_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msg_get_context >::parse_response( const void* payload_ptr )
{
    const remote_id_t* response_ptr =
        reinterpret_cast<const remote_id_t*>( payload_ptr );

    remote_id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msg_attach_context >::create_request( void* payload_ptr )
{
    remote_id_t* request_ptr = reinterpret_cast<remote_id_t*>( payload_ptr );

    *request_ptr = host_to_network( remote_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msg_attach_context >::parse_request( const void* payload_ptr )
{
    const remote_id_t* request_ptr =
        reinterpret_cast<const remote_id_t*>( payload_ptr );

    remote_id_ = network_to_host( *request_ptr );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
