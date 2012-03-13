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
#include "msg_context.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContextFromType >::create_request( uint8_t* payload_ptr )
{
    cl_device_type* device_type_ptr = reinterpret_cast< cl_device_type* >( payload_ptr );

    *device_type_ptr = host_to_network( device_type_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContextFromType >::parse_request( const uint8_t* payload_ptr )
{
    const cl_device_type* device_type_ptr = reinterpret_cast< const cl_device_type* >( payload_ptr );

    device_type_ = network_to_host( *device_type_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContextFromType >::create_response( uint8_t* payload_ptr )
{
    remote_id_t* id_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *id_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateContextFromType >::parse_response( const base_message* message_ptr )
{
    const dcl_message< msgCreateContextFromType >* msg_response_ptr = 
        reinterpret_cast< const dcl_message< msgCreateContextFromType >* >( message_ptr );

    const remote_id_t* id_ptr = 
        reinterpret_cast< const remote_id_t* >( msg_response_ptr->get_payload() );

    id_ = network_to_host( *id_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetContextInfo >::create_request( uint8_t* payload_ptr )
{
    remote_id_t* id_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *id_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetContextInfo >::parse_request( const uint8_t* payload_ptr )
{
    const remote_id_t* id_ptr = reinterpret_cast< const remote_id_t* >( payload_ptr );

    id_ = network_to_host( *id_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetContextInfo >::create_response( uint8_t* payload_ptr )
{
    msgGetContextInfo_response* response_ptr =
        reinterpret_cast< msgGetContextInfo_response* >( payload_ptr );

    response_ptr->device_count_ = host_to_network( device_count_ );
    
    for( uint32_t i = 0; i < device_count_; i++ )
    {
        response_ptr->devices_[ i ] = host_to_network( devices_[ i ] );
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetContextInfo >::parse_response( const base_message* message_ptr )
{
    const dcl_message< msgGetContextInfo >* msg_response_ptr =
        reinterpret_cast< const dcl_message< msgGetContextInfo >* >( message_ptr );

    const msgGetContextInfo_response* response_ptr =
        reinterpret_cast< const msgGetContextInfo_response* >( msg_response_ptr->get_payload() );

    set_device_count( network_to_host( response_ptr->device_count_ ) );

    for( uint32_t i = 0; i < device_count_; i++ )
    {
        devices_[ i ] = network_to_host( response_ptr->devices_[ i ] );
    }
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
