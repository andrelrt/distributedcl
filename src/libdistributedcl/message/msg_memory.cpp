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
#include "msg_memory.h"
using dcl::remote_id_t;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
// msgCreateBuffer
//-----------------------------------------------------------------------------
void dcl_message< msgCreateBuffer >::create_request( uint8_t* payload_ptr )
{
    msgCreateBuffer_request* request_ptr = 
        reinterpret_cast< msgCreateBuffer_request* >( payload_ptr );

    request_ptr->context_id_ = host_to_network( context_id_ );
    request_ptr->flags_ = host_to_network( static_cast<uint16_t>( flags_ ) );
    request_ptr->buffer_len_ = host_to_network( static_cast<uint32_t>( buffer_.size() ) );

    memcpy( request_ptr->buffer_, buffer_.data(), buffer_.size() );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateBuffer >::parse_request( const uint8_t* payload_ptr )
{
    const msgCreateBuffer_request* request_ptr = 
        reinterpret_cast< const msgCreateBuffer_request* >( payload_ptr );

    context_id_ = network_to_host( request_ptr->context_id_ );
    flags_ = static_cast<cl_mem_flags>( network_to_host( request_ptr->flags_ ) );

    size_t size = network_to_host( request_ptr->buffer_len_ );
    const uint8_t* begin = reinterpret_cast<const uint8_t*>( request_ptr->buffer_ );

    buffer_.assign( begin, begin + size );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateBuffer >::create_response( uint8_t* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateBuffer >::parse_response( const base_message* message_ptr )
{
    const dcl_message< msgCreateBuffer >* msg_response_ptr = 
        reinterpret_cast< const dcl_message< msgCreateBuffer >* >( message_ptr );

    const remote_id_t* response_ptr = 
        reinterpret_cast< const remote_id_t* >( msg_response_ptr->get_payload() );

    id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
