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
#include <string.h>
#include "msg_kernel.h"
using dcl::remote_id_t;
using dcl::info::ndrange;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
// msgCreateKernel
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::create_request( void* payload_ptr )
{
    msgCreateKernel_request* request_ptr = 
        reinterpret_cast< msgCreateKernel_request* >( payload_ptr );

    request_ptr->program_id_ = host_to_network( program_id_ );
    request_ptr->name_len_ = host_to_network( static_cast<uint32_t>( name_.length() ) );

    memcpy( request_ptr->name_, name_.data(), name_.length() );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::parse_request( const void* payload_ptr )
{
    const msgCreateKernel_request* request_ptr = 
        reinterpret_cast< const msgCreateKernel_request* >( payload_ptr );

    program_id_ = network_to_host( request_ptr->program_id_ );

    name_.assign( reinterpret_cast<const char*>( request_ptr->name_ ), 
                  network_to_host( request_ptr->name_len_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::create_response( void* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::parse_response( const void* payload_ptr )
{
    const remote_id_t* response_ptr = 
        reinterpret_cast< const remote_id_t* >( payload_ptr );

    id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
// msgEnqueueNDRangeKernel
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueNDRangeKernel >::create_request( void* payload_ptr )
{
    msgEnqueueNDRangeKernel_request* request_ptr = 
        reinterpret_cast< msgEnqueueNDRangeKernel_request* >( payload_ptr );

    request_ptr->command_queue_id_ = host_to_network( command_queue_id_ );
    request_ptr->kernel_id_ = host_to_network( kernel_id_ );
    request_ptr->dimensions_ = host_to_network( static_cast<uint16_t>( global_.get_dimensions() ) );

    for( size_t i = 0; i < global_.get_dimensions(); i++ )
    {
        request_ptr->offset_[ i ] = host_to_network( static_cast<uint16_t>( offset_.get_range()[ i ] ) );
        request_ptr->global_[ i ] = host_to_network( static_cast<uint16_t>( global_.get_range()[ i ] ) );
        request_ptr->local_[ i ]  = host_to_network( static_cast<uint16_t>( local_.get_range()[ i ] ) );
    }

    request_ptr->return_event_ = host_to_network( static_cast<uint16_t>( return_event_ ? 1 : 0 ) );
    request_ptr->event_count_ = host_to_network( static_cast<uint16_t>( events_.size() ) );

    for( uint32_t i = 0; i < events_.size(); i++ )
    {
        request_ptr->events_[ i ] = host_to_network( events_[ i ] );
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueNDRangeKernel >::parse_request( const void* payload_ptr )
{
    const msgEnqueueNDRangeKernel_request* request_ptr = 
        reinterpret_cast< const msgEnqueueNDRangeKernel_request* >( payload_ptr );

    command_queue_id_ = network_to_host( request_ptr->command_queue_id_ );
    kernel_id_ = network_to_host( request_ptr->kernel_id_ );
    uint16_t dimensions = network_to_host( request_ptr->dimensions_ );
    size_t value[ 3 ];

    value[ 0 ] = network_to_host( request_ptr->offset_[ 0 ] );
    value[ 1 ] = network_to_host( request_ptr->offset_[ 1 ] );
    value[ 2 ] = network_to_host( request_ptr->offset_[ 2 ] );
    offset_.copy( ndrange( dimensions, value ) );

    value[ 0 ] = network_to_host( request_ptr->global_[ 0 ] );
    value[ 1 ] = network_to_host( request_ptr->global_[ 1 ] );
    value[ 2 ] = network_to_host( request_ptr->global_[ 2 ] );
    global_.copy( ndrange( dimensions, value ) );

    value[ 0 ] = network_to_host( request_ptr->local_[ 0 ] );
    value[ 1 ] = network_to_host( request_ptr->local_[ 1 ] );
    value[ 2 ] = network_to_host( request_ptr->local_[ 2 ] );
    local_.copy( ndrange( dimensions, value ) );

    return_event_ = (network_to_host( request_ptr->return_event_ ) != 0) ? true : false;

    set_response_size( return_event_? sizeof(dcl::remote_id_t) : 0 );

    events_.clear();
    uint32_t event_count = network_to_host( request_ptr->event_count_ );

    if( event_count != 0 )
    {
        events_.reserve( event_count );

        for( uint32_t i = 0; i < events_.size(); i++ )
        {
            events_.push_back( network_to_host( request_ptr->events_[ i ] ) );
        }
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueNDRangeKernel >::create_response( void* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( event_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueNDRangeKernel >::parse_response( const void* payload_ptr )
{
    const remote_id_t* response_ptr = 
        reinterpret_cast< const remote_id_t* >( payload_ptr );

    event_id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
// msgSetKernelArg
//-----------------------------------------------------------------------------
void dcl_message< msgSetKernelArg >::create_request( void* payload_ptr )
{
    if( is_memory_object_ )
    {
        msgSetKernelArg_memory_request* request_ptr =
            reinterpret_cast< msgSetKernelArg_memory_request* >( payload_ptr );

        request_ptr->is_memory_object_ = host_to_network( static_cast<uint16_t>( 1 ) );
        request_ptr->index_ = host_to_network( index_ );
        request_ptr->kernel_id_ = host_to_network( kernel_id_ );
        request_ptr->memory_id_ = host_to_network( memory_id_ );
    }
    else
    {
        msgSetKernelArg_buffer_request* request_ptr =
            reinterpret_cast< msgSetKernelArg_buffer_request* >( payload_ptr );

        request_ptr->is_memory_object_ = host_to_network( static_cast<uint16_t>( 0 ) );
        request_ptr->index_ = host_to_network( index_ );
        request_ptr->kernel_id_ = host_to_network( kernel_id_ );
        request_ptr->size_ = host_to_network( size_ );
        request_ptr->is_null_ = host_to_network( static_cast<uint16_t>( is_null_? 1 : 0 ) );

        if( !is_null_ )
        {
            memcpy( request_ptr->buffer_, buffer_.data(), buffer_.size() );
        }
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgSetKernelArg >::parse_request( const void* payload_ptr )
{
    const msgSetKernelArg_memory_request* request_ptr =
        reinterpret_cast< const msgSetKernelArg_memory_request* >( payload_ptr );

    if( request_ptr->is_memory_object_ )
    {
        is_memory_object_ = true;
        index_ = network_to_host( request_ptr->index_ );
        kernel_id_ = network_to_host( request_ptr->kernel_id_ );
        memory_id_ = network_to_host( request_ptr->memory_id_ );
    }
    else
    {
        const msgSetKernelArg_buffer_request* req_ptr =
            reinterpret_cast< const msgSetKernelArg_buffer_request* >( payload_ptr );

        is_memory_object_ = false;

        size_ = network_to_host( req_ptr->size_ );
        index_ = network_to_host( req_ptr->index_ );
        kernel_id_ = network_to_host( req_ptr->kernel_id_ );
        is_null_ = network_to_host( req_ptr->is_null_ ) ? true : false;

        if( !is_null_ )
        {
            buffer_.assign( req_ptr->buffer_, req_ptr->buffer_ + network_to_host( req_ptr->size_ ) );
        }
    }
}
//-----------------------------------------------------------------------------
// msgGetKernelWorkGroupInfo
//-----------------------------------------------------------------------------
void dcl_message< msgGetKernelWorkGroupInfo >::create_request( void* payload_ptr )
{
    remote_id_t* request_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    request_ptr[ 0 ] = host_to_network( kernel_id_ );
    request_ptr[ 1 ] = host_to_network( device_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetKernelWorkGroupInfo >::parse_request( const void* payload_ptr )
{
    const remote_id_t* request_ptr = reinterpret_cast< const remote_id_t* >( payload_ptr );

    kernel_id_ = network_to_host( request_ptr[ 0 ] );
    device_id_ = network_to_host( request_ptr[ 1 ] );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetKernelWorkGroupInfo >::create_response( void* payload_ptr )
{
    msgGetKernelWorkGroupInfo_response* response_ptr =
        reinterpret_cast< msgGetKernelWorkGroupInfo_response* >( payload_ptr );

    response_ptr->local_mem_size_ = host_to_network( info_.local_mem_size_ );
    response_ptr->work_group_size_ = host_to_network( static_cast<uint32_t>( info_.work_group_size_ ) );
    response_ptr->private_mem_size_ = host_to_network( info_.private_mem_size_ );
    response_ptr->preferred_work_group_size_multiple_ = host_to_network( static_cast<uint32_t>( info_.preferred_work_group_size_multiple_ ) );

    response_ptr->compile_work_group_size_[ 0 ] = host_to_network( static_cast<uint32_t>( info_.compile_work_group_size_[ 0 ] ) );
    response_ptr->compile_work_group_size_[ 1 ] = host_to_network( static_cast<uint32_t>( info_.compile_work_group_size_[ 1 ] ) );
    response_ptr->compile_work_group_size_[ 2 ] = host_to_network( static_cast<uint32_t>( info_.compile_work_group_size_[ 2 ] ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetKernelWorkGroupInfo >::parse_response( const void* payload_ptr )
{
    const msgGetKernelWorkGroupInfo_response* response_ptr =
        reinterpret_cast< const msgGetKernelWorkGroupInfo_response* >( payload_ptr );

    info_.local_mem_size_ = network_to_host( response_ptr->local_mem_size_ );
    info_.work_group_size_ = network_to_host( response_ptr->work_group_size_ );
    info_.private_mem_size_ = network_to_host( response_ptr->private_mem_size_ );
    info_.preferred_work_group_size_multiple_ = network_to_host( response_ptr->preferred_work_group_size_multiple_ );

    info_.compile_work_group_size_[ 0 ] = network_to_host( response_ptr->compile_work_group_size_[ 0 ] );
    info_.compile_work_group_size_[ 1 ] = network_to_host( response_ptr->compile_work_group_size_[ 1 ] );
    info_.compile_work_group_size_[ 2 ] = network_to_host( response_ptr->compile_work_group_size_[ 2 ] );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
