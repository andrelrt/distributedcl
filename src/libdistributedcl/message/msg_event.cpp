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
#include "msg_event.h"
using dcl::remote_id_t;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
// msgWaitForEvents
//-----------------------------------------------------------------------------
void dcl_message< msgWaitForEvents >::create_request( void* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast<remote_id_t*>( payload_ptr );

    *response_ptr = host_to_network( remote_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgWaitForEvents >::parse_request( const void* payload_ptr )
{
    const remote_id_t* response_ptr =
        reinterpret_cast<const remote_id_t*>( payload_ptr );

    remote_id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
// msgGetEventProfilingInfo
//-----------------------------------------------------------------------------
void dcl_message< msgGetEventProfilingInfo >::create_request( void* payload_ptr )
{
    remote_id_t* request_ptr = reinterpret_cast<remote_id_t*>( payload_ptr );

    *request_ptr = host_to_network( remote_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetEventProfilingInfo >::parse_request( const void* payload_ptr )
{
    const remote_id_t* request_ptr =
        reinterpret_cast<const remote_id_t*>( payload_ptr );

    remote_id_ = network_to_host( *request_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetEventProfilingInfo >::create_response( void* payload_ptr )
{
    msgGetEventProfilingInfo_response* response_ptr =
        reinterpret_cast< msgGetEventProfilingInfo_response* >( payload_ptr );

    response_ptr->queued_ = host_to_network( event_info_.queued_ );
    response_ptr->submit_ = host_to_network( event_info_.submit_ );
    response_ptr->start_  = host_to_network( event_info_.start_ );
    response_ptr->end_    = host_to_network( event_info_.end_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetEventProfilingInfo >::parse_response( const void* payload_ptr )
{
    const msgGetEventProfilingInfo_response* response_ptr =
        reinterpret_cast< const msgGetEventProfilingInfo_response* >( payload_ptr );

    event_info_.queued_ = network_to_host( response_ptr->queued_ );
    event_info_.submit_ = network_to_host( response_ptr->submit_ );
    event_info_.start_  = network_to_host( response_ptr->start_ );
    event_info_.end_    = network_to_host( response_ptr->end_ );
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
