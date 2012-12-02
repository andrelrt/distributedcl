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
#include "msg_context.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
// msgCreateContext
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContext >::create_request( void* payload_ptr )
{
    msgCreateContext_request* request_ptr =
        reinterpret_cast< msgCreateContext_request* >( payload_ptr );

    request_ptr->device_count_ = host_to_network( static_cast<uint32_t>( devices_.size() ) );
    
    uint32_t i = 0;
    for( remote_ids_t::iterator it = devices_.begin(); it != devices_.end(); ++it )
    {
        request_ptr->devices_[ i ] = host_to_network( *it );

        i++;
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContext >::parse_request( const void* payload_ptr )
{
    const msgCreateContext_request* request_ptr =
        reinterpret_cast< const msgCreateContext_request* >( payload_ptr );

    uint32_t device_count = network_to_host( request_ptr->device_count_ );

    set_device_count( device_count );

    for( uint32_t i = 0; i < device_count; i++ )
    {
        devices_.push_back( network_to_host( request_ptr->devices_[ i ] ) );
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContext >::create_response( void* payload_ptr )
{
    remote_id_t* id_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *id_ptr = host_to_network( remote_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContext >::parse_response( const void* payload_ptr )
{
    const remote_id_t* id_ptr = reinterpret_cast< const remote_id_t* >( payload_ptr );

    remote_id_ = network_to_host( *id_ptr );
}
//-----------------------------------------------------------------------------
// msgCreateContextFromType
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContextFromType >::create_request( void* payload_ptr )
{
    cl_device_type* device_type_ptr = reinterpret_cast< cl_device_type* >( payload_ptr );

    *device_type_ptr = host_to_network( device_type_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContextFromType >::parse_request( const void* payload_ptr )
{
    const cl_device_type* device_type_ptr = reinterpret_cast< const cl_device_type* >( payload_ptr );

    device_type_ = network_to_host( *device_type_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContextFromType >::create_response( void* payload_ptr )
{
    remote_id_t* id_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *id_ptr = host_to_network( remote_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContextFromType >::parse_response( const void* payload_ptr )
{
    const remote_id_t* id_ptr = 
        reinterpret_cast< const remote_id_t* >( payload_ptr );

    remote_id_ = network_to_host( *id_ptr );
}
//-----------------------------------------------------------------------------
// msgGetContextInfo
//-----------------------------------------------------------------------------
void dcl_message< msgGetContextInfo >::create_request( void* payload_ptr )
{
    remote_id_t* id_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *id_ptr = host_to_network( remote_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetContextInfo >::parse_request( const void* payload_ptr )
{
    const remote_id_t* id_ptr = reinterpret_cast< const remote_id_t* >( payload_ptr );

    remote_id_ = network_to_host( *id_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetContextInfo >::create_response( void* payload_ptr )
{
    msgGetContextInfo_response* response_ptr =
        reinterpret_cast< msgGetContextInfo_response* >( payload_ptr );

    response_ptr->device_count_ = host_to_network( static_cast<uint32_t>( devices_.size() ) );

    uint32_t i = 0;
    for( remote_ids_t::iterator it = devices_.begin(); it != devices_.end(); ++it )
    {
        response_ptr->devices_[ i ] = host_to_network( *it );

        i++;
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetContextInfo >::parse_response( const void* payload_ptr )
{
    const msgGetContextInfo_response* response_ptr =
        reinterpret_cast< const msgGetContextInfo_response* >( payload_ptr );

    uint32_t device_count = network_to_host( response_ptr->device_count_ );

    set_device_count( device_count );

    for( uint32_t i = 0; i < device_count; i++ )
    {
        devices_.push_back( network_to_host( response_ptr->devices_[ i ] ) );
    }
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
