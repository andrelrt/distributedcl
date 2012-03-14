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
#include "msg_kernel.h"
using dcl::remote_id_t;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::create_request( uint8_t* payload_ptr )
{
    msgCreateKernel_request* request_ptr = 
        reinterpret_cast< msgCreateKernel_request* >( payload_ptr );

    request_ptr->program_id_ = host_to_network( program_id_ );
    request_ptr->name_len_ = host_to_network( static_cast<uint32_t>( name_.length() ) );

    memcpy( request_ptr->name_, name_.data(), name_.length() );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::parse_request( const uint8_t* payload_ptr )
{
    const msgCreateKernel_request* request_ptr = 
        reinterpret_cast< const msgCreateKernel_request* >( payload_ptr );

    program_id_ = network_to_host( request_ptr->program_id_ );

    name_.assign( reinterpret_cast<const char*>( request_ptr->name_ ), 
                  network_to_host( request_ptr->name_len_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::create_response( uint8_t* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::parse_response( const base_message* message_ptr )
{
    const dcl_message< msgCreateKernel >* msg_response_ptr = 
        reinterpret_cast< const dcl_message< msgCreateKernel >* >( message_ptr );

    const remote_id_t* response_ptr = 
        reinterpret_cast< const remote_id_t* >( msg_response_ptr->get_payload() );

    id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
