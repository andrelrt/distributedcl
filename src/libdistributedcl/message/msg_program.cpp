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
using dcl::remote_id_t;
using dcl::remote_ids_t;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
// msgCreateProgramWithSource
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::create_request( void* payload_ptr )
{
    msgCreateProgramWithSource_request* request_ptr = 
        reinterpret_cast< msgCreateProgramWithSource_request* >( payload_ptr );

    request_ptr->context_id_ = host_to_network( context_id_ );
    request_ptr->source_code_len_ = host_to_network( static_cast<uint32_t>( source_code_.length() ) );
    
    memcpy( request_ptr->source_code_, source_code_.data(), source_code_.length() );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::parse_request( const void* payload_ptr )
{
    const msgCreateProgramWithSource_request* request_ptr = 
        reinterpret_cast< const msgCreateProgramWithSource_request* >( payload_ptr );

    context_id_ = network_to_host( request_ptr->context_id_ );

    source_code_.assign( reinterpret_cast<const char*>( request_ptr->source_code_ ), 
                         network_to_host( request_ptr->source_code_len_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::create_response( void* payload_ptr )
{
    dcl::remote_id_t* response_ptr = reinterpret_cast< dcl::remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::parse_response( const void* payload_ptr )
{
    const dcl::remote_id_t* response_ptr =
        reinterpret_cast< const dcl::remote_id_t* >( payload_ptr );

    id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
// msgBuildProgram
//-----------------------------------------------------------------------------
void dcl_message< msgBuildProgram >::create_request( void* payload_ptr )
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
void dcl_message< msgBuildProgram >::parse_request( const void* payload_ptr )
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
// msgGetProgramBuildInfo
//-----------------------------------------------------------------------------
void dcl_message< msgGetProgramBuildInfo >::create_request( void* payload_ptr )
{
    msgGetProgramBuildInfo_request* request_ptr = 
        reinterpret_cast< msgGetProgramBuildInfo_request* >( payload_ptr );

    request_ptr->device_id_ = host_to_network( device_id_ );
    request_ptr->build_info_ = host_to_network( static_cast<uint16_t>( build_info_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetProgramBuildInfo >::parse_request( const void* payload_ptr )
{
    const msgGetProgramBuildInfo_request* request_ptr = 
        reinterpret_cast< const msgGetProgramBuildInfo_request* >( payload_ptr );

    device_id_ = network_to_host( request_ptr->device_id_ );
    build_info_ = static_cast<cl_program_build_info>( network_to_host( request_ptr->build_info_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetProgramBuildInfo >::create_response( void* payload_ptr )
{
    uint32_t* response_ptr = reinterpret_cast< uint32_t* >( payload_ptr );

    if( build_info_ == CL_PROGRAM_BUILD_STATUS )
    {
        *response_ptr = host_to_network( static_cast<uint32_t>( build_status_ ) );
    }
    else if( build_info_ == CL_PROGRAM_BUILD_LOG )
    {
        *response_ptr = host_to_network( static_cast<uint32_t>( build_log_.length() ) );

        memcpy( response_ptr+1, build_log_.data(), build_log_.length() );
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetProgramBuildInfo >::parse_response( const void* payload_ptr )
{
    const uint32_t* response_ptr = 
        reinterpret_cast< const uint32_t* >( payload_ptr );

    if( build_info_ == CL_PROGRAM_BUILD_STATUS )
    {
        build_status_ = static_cast<cl_build_status>( network_to_host( *response_ptr ) );
    }
    else if( build_info_ == CL_PROGRAM_BUILD_LOG )
    {
        uint32_t len = network_to_host( *response_ptr );
        const uint8_t* begin = reinterpret_cast<const uint8_t*>( response_ptr + 1 );

        build_log_.assign( begin, begin + len );
    }
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
