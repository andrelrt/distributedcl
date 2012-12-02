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
#include <iostream>
#include "remote_context.h"
#include "remote_program.h"
#include "remote_device.h"
#include "remote_command_queue.h"
#include "remote_memory.h"
#include "remote_sampler.h"
#include "message/msg_program.h"
#include "message/msg_context.h"
#include "message/msg_command_queue.h"
#include "message/msg_memory.h"
#include "message/msg_internal.h"
#include "message/msg_sampler.h"
#include "network/session_manager.h"
using dcl::info::generic_device;
using dcl::info::generic_program;
using dcl::info::generic_command_queue;
using dcl::info::generic_memory;
using dcl::info::generic_image;
using dcl::info::generic_sampler;
using dcl::network::message::base_message;
using dcl::network::message::dcl_message;
using dcl::network::message::msgGetContextInfo;
using dcl::network::message::msgCreateProgramWithSource;
using dcl::network::message::msgCreateCommandQueue;
using dcl::network::message::msgCreateBuffer;
using dcl::network::message::msgCreateImage2D;
using dcl::network::message::msgCreateSampler;
using dcl::network::message::msg_get_context;
using dcl::network::message::msg_attach_context;
using dcl::network::client::session_manager;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_context::load_devices()
{
    dcl_message< msgGetContextInfo >* msg_ptr = new dcl_message< msgGetContextInfo >();

    msg_ptr->set_remote_id( get_remote_id() );

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    for( uint32_t i = 0; i < msg_ptr->get_device_count(); i++ )
    {
        devices_.push_back( remote_device::get_remote_device( platform_ptr_, msg_ptr->get_devices()[ i ] ) );
    }
}
//-----------------------------------------------------------------------------
generic_program* remote_context::do_create_program( const std::string& source_code )
{
    dcl_message< msgCreateProgramWithSource >* msg_ptr = new dcl_message< msgCreateProgramWithSource >();

    msg_ptr->set_source_code( source_code );
    msg_ptr->set_context_id( get_remote_id() );

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    remote_program* program_ptr = new remote_program( *this, source_code );
    program_ptr->set_remote_id( msg_ptr->get_remote_id() );

    return reinterpret_cast< generic_program* >( program_ptr );
}
//-----------------------------------------------------------------------------
generic_command_queue*
    remote_context::do_create_command_queue( const generic_device* device_ptr,
                                             cl_command_queue_properties properties )
{
    // Get connection context
    dcl_message< msg_get_context >* msg_get_ptr =
        new dcl_message< msg_get_context >();

    message_sp_t msg_get_sp( msg_get_ptr );
    session_ref_.send_message( msg_get_sp );


    // Create new connection
    session_manager::session_t* child_session_ptr = session_ref_.create_child();

    dcl_message< msg_attach_context >* msg_attach_ptr =
        new dcl_message< msg_attach_context >();

    msg_attach_ptr->set_remote_id( msg_get_ptr->get_remote_id() );

    message_sp_t msg_attach_sp( msg_attach_ptr );
    child_session_ptr->enqueue_message( msg_attach_sp );


    // Create command queue
    dcl_message< msgCreateCommandQueue >* msg_ptr = new dcl_message< msgCreateCommandQueue >();

    const remote_device* device = reinterpret_cast< const remote_device* >( device_ptr );

    msg_ptr->set_context_id( get_remote_id() );
    msg_ptr->set_device_id( device->get_remote_id() );
    msg_ptr->set_properties( properties );

    message_sp_t message_sp( msg_ptr );
    child_session_ptr->send_message( message_sp );

    remote_command_queue* command_queue_ptr =
        new remote_command_queue( *child_session_ptr, this, device, properties );

    command_queue_ptr->set_remote_id( msg_ptr->get_remote_id() );

    return reinterpret_cast< generic_command_queue* >( command_queue_ptr );
}
//-----------------------------------------------------------------------------
generic_memory* remote_context::do_create_buffer( const void* host_ptr, size_t size, cl_mem_flags flags )
{
    dcl_message< msgCreateBuffer >* msg_ptr = new dcl_message< msgCreateBuffer >();

    msg_ptr->set_context_id( get_remote_id() );
    msg_ptr->set_buffer( reinterpret_cast<const uint8_t*>( host_ptr ), size );
    msg_ptr->set_flags( flags );

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    remote_memory* memory_ptr = new remote_memory( *this, host_ptr, size, flags );
    memory_ptr->set_remote_id( msg_ptr->get_remote_id() );

    return reinterpret_cast< generic_memory* >( memory_ptr );
}
//-----------------------------------------------------------------------------
generic_image*
remote_context::do_create_image( const void* host_ptr, cl_mem_flags flags,
                                 const cl_image_format* format, size_t width,
                                 size_t height, size_t row_pitch )
{
    dcl_message< msgCreateImage2D >* msg_ptr = new dcl_message< msgCreateImage2D >();

    msg_ptr->set_context_id( get_remote_id() );
    msg_ptr->set_flags( flags );
    msg_ptr->set_channel_order( format->image_channel_order );
    msg_ptr->set_channel_type( format->image_channel_data_type );
    msg_ptr->set_width( width );
    msg_ptr->set_height( height );
    msg_ptr->set_row_pitch( row_pitch );
    msg_ptr->set_buffer( reinterpret_cast<const uint8_t*>( host_ptr ) );

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    remote_image* image_ptr = new remote_image( *this, host_ptr, flags, format,
                                                width, height, row_pitch );
    image_ptr->set_remote_id( msg_ptr->get_remote_id() );

    return reinterpret_cast< generic_image* >( image_ptr );
}
//-----------------------------------------------------------------------------
generic_sampler*
remote_context::do_create_sampler( cl_bool normalized_coords, cl_addressing_mode addressing_mode,
                                   cl_filter_mode filter_mode )
{
    dcl_message< msgCreateSampler >* msg_ptr = new dcl_message< msgCreateSampler >();

    msg_ptr->set_context_id( get_remote_id() );
    msg_ptr->set_normalized_coords( normalized_coords );
    msg_ptr->set_addressing_mode( addressing_mode );
    msg_ptr->set_filter_mode( filter_mode );

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    remote_sampler* sampler_ptr =
        new remote_sampler( *this, normalized_coords, addressing_mode, filter_mode );

    sampler_ptr->set_remote_id( msg_ptr->get_remote_id() );

    return reinterpret_cast< generic_sampler* >( sampler_ptr );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
