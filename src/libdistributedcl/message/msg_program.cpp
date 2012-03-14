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
#include "msg_program.h"
#include "remote/remote_device.h"
using dcl::remote_id_t;
using dcl::remote_ids_t;
using dcl::remote::remote_device;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::create_request( uint8_t* payload_ptr )
{
    msgCreateProgramWithSource_request* request_ptr = 
        reinterpret_cast< msgCreateProgramWithSource_request* >( payload_ptr );

    request_ptr->context_id_ = host_to_network( context_id_ );
    request_ptr->source_code_len_ = host_to_network( static_cast<uint32_t>( source_code_.length() ) );
    
    memcpy( request_ptr->source_code_, source_code_.data(), source_code_.length() );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::parse_request( const uint8_t* payload_ptr )
{
    const msgCreateProgramWithSource_request* request_ptr = 
        reinterpret_cast< const msgCreateProgramWithSource_request* >( payload_ptr );

    context_id_ = network_to_host( request_ptr->context_id_ );

    source_code_.assign( reinterpret_cast<const char*>( request_ptr->source_code_ ), 
                         network_to_host( request_ptr->source_code_len_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::create_response( uint8_t* payload_ptr )
{
    dcl::remote_id_t* response_ptr = reinterpret_cast< dcl::remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::parse_response( const base_message* message_ptr )
{
    const dcl_message< msgCreateProgramWithSource >* msg_response_ptr = 
        reinterpret_cast< const dcl_message< msgCreateProgramWithSource >* >( message_ptr );

    const dcl::remote_id_t* response_ptr = 
        reinterpret_cast< const dcl::remote_id_t* >( msg_response_ptr->get_payload() );

    id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msgBuildProgram >::create_request( uint8_t* payload_ptr )
{
    msgBuildProgram_request* request_ptr = 
        reinterpret_cast< msgBuildProgram_request* >( payload_ptr );

    request_ptr->program_id_ = host_to_network( program_id_ );
    request_ptr->devices_count_ = host_to_network( static_cast<uint16_t>( devices_.size() ) );
    request_ptr->build_options_len_ = host_to_network( static_cast<uint32_t>( build_options_.length() ) );
    

    remote_id_t* devices_ptr = reinterpret_cast<dcl::remote_id_t*>( request_ptr->buffer_ );

    for( remote_ids_t::iterator it = devices_.begin(); it != devices_.end(); it++ )
    {
        *devices_ptr++ = *it;
    }

    uint8_t* build_options_ptr = request_ptr->buffer_ + sizeof( remote_id_t ) * devices_.size();

    memcpy( build_options_ptr, build_options_.data(), build_options_.length() );
}
//-----------------------------------------------------------------------------
void dcl_message< msgBuildProgram >::parse_request( const uint8_t* payload_ptr )
{
    const msgBuildProgram_request* request_ptr = 
        reinterpret_cast< const msgBuildProgram_request* >( payload_ptr );

    program_id_ = network_to_host( request_ptr->program_id_ );

    uint32_t device_count = network_to_host( request_ptr->devices_count_ );

    devices_.clear();

    if( device_count != 0 )
    {
        devices_.reserve( device_count );
        const remote_id_t* devices_ptr = reinterpret_cast<const remote_id_t*>( request_ptr->buffer_ );

        for( uint32_t i = 0; i < device_count; i++ )
        {
            devices_.push_back( devices_ptr[ i ] );
        }
    }

    const uint8_t* build_options_ptr = request_ptr->buffer_ + sizeof( remote_id_t ) * device_count;

    build_options_.assign( reinterpret_cast<const char*>( build_options_ptr ), 
                           network_to_host( request_ptr->build_options_len_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgBuildProgram >::set_devices( const devices_t& devices )
{
    devices_.clear();
    devices_.reserve( devices.size() );

    for( devices_t::const_iterator it = devices.begin(); it != devices.end(); it++ )
    {
        remote_device* remote_device_ptr = reinterpret_cast<remote_device*>( *it );
        devices_.push_back( remote_device_ptr->get_remote_id() );
    }

    update_request_size();
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
