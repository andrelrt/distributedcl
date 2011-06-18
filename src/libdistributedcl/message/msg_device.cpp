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
#include "msg_device.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
void dcl_message< msgGetDeviceIDs >::set_response( const base_message* response_ptr )
{
    const dcl_message< msgGetDeviceIDs >* msg_response_ptr = reinterpret_cast< const dcl_message< msgGetDeviceIDs >* >( response_ptr );
    const msgGetDeviceIDs_response* response = reinterpret_cast< const msgGetDeviceIDs_response* >( msg_response_ptr ->get_payload() );

    cpu_count_ = ntohs( response->cpu_count );
    gpu_count_ = ntohs( response->gpu_count );
    accelerator_count_ = ntohs( response->accelerator_count );
    other_count_ = ntohs( response->other_count );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetDeviceIDs >::create_response( uint8_t* payload_ptr )
{
    msgGetDeviceIDs_response* response = reinterpret_cast< msgGetDeviceIDs_response* >( payload_ptr );

    response->cpu_count = htons( static_cast< uint16_t >( cpu_count_ ) );
    response->gpu_count = htons( static_cast< uint16_t >( gpu_count_ ) );
    response->accelerator_count  = htons( static_cast< uint16_t >( accelerator_count_ ) );
    response->other_count = htons( static_cast< uint16_t >( other_count_ ) );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
