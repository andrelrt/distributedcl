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
#include "msg_sampler.h"
using dcl::remote_id_t;
using dcl::remote_ids_t;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
// msgCreateSampler
//-----------------------------------------------------------------------------
void dcl_message< msgCreateSampler >::create_request( void* payload_ptr )
{
    msgCreateSampler_request* request_ptr = 
        reinterpret_cast< msgCreateSampler_request* >( payload_ptr );

    request_ptr->context_id_ = host_to_network( context_id_ );
    request_ptr->normalized_coords_ = (normalized_coords_ == CL_TRUE)? 1 : 0;
    request_ptr->addressing_mode_ = addressing_mode_ & 7;
    request_ptr->filter_mode_ = (filter_mode_ == CL_FILTER_NEAREST)? 1 : 0;
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateSampler >::parse_request( const void* payload_ptr )
{
    const msgCreateSampler_request* request_ptr = 
        reinterpret_cast< const msgCreateSampler_request* >( payload_ptr );

    context_id_ = network_to_host( request_ptr->context_id_ );
    normalized_coords_ = (request_ptr->normalized_coords_ == 1)? CL_TRUE : CL_FALSE;
    addressing_mode_ = request_ptr->addressing_mode_ | CL_ADDRESS_NONE;
    filter_mode_ = (request_ptr->filter_mode_ == 1)? CL_FILTER_NEAREST : CL_FILTER_LINEAR;
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateSampler >::create_response( void* payload_ptr )
{
    dcl::remote_id_t* response_ptr = reinterpret_cast< dcl::remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( remote_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateSampler >::parse_response( const void* payload_ptr )
{
    const dcl::remote_id_t* response_ptr =
        reinterpret_cast< const dcl::remote_id_t* >( payload_ptr );

    remote_id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
