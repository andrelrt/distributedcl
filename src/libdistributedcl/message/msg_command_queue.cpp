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
#include "msg_command_queue.h"
using dcl::remote_id_t;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
// msgCreateCommandQueue
//-----------------------------------------------------------------------------
void dcl_message< msgCreateCommandQueue >::create_request( void* payload_ptr )
{
    msgCreateCommandQueue_request* request_ptr = 
        reinterpret_cast< msgCreateCommandQueue_request* >( payload_ptr );

    request_ptr->device_id_ = host_to_network( device_id_ );
    request_ptr->context_id_ = host_to_network( context_id_ );
    request_ptr->properties_ = host_to_network( properties_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateCommandQueue >::parse_request( const void* payload_ptr )
{
    const msgCreateCommandQueue_request* request_ptr = 
        reinterpret_cast< const msgCreateCommandQueue_request* >( payload_ptr );

    device_id_ = network_to_host( request_ptr->device_id_ );
    context_id_ = network_to_host( request_ptr->context_id_ );
    properties_ = network_to_host( request_ptr->properties_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateCommandQueue >::create_response( void* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateCommandQueue >::parse_response( const void* payload_ptr )
{
    const remote_id_t* response_ptr = 
        reinterpret_cast< const remote_id_t* >( payload_ptr );

    id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
// msgFinish
//-----------------------------------------------------------------------------
void dcl_message< msgFinish >::create_request( void* payload_ptr )
{
    remote_id_t* request_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *request_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgFinish >::parse_request( const void* payload_ptr )
{
    const remote_id_t* request_ptr = reinterpret_cast<const remote_id_t*>( payload_ptr );

    id_ = network_to_host( *request_ptr );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
