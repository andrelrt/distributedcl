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
#include "remote_kernel.h"
#include "remote_command_queue.h"
#include "remote_memory.h"
#include "remote_device.h"
#include "remote_event.h"
#include "message/msg_kernel.h"
using dcl::network::message::dcl_message;
using dcl::network::message::base_message;
using dcl::network::message::msgEnqueueNDRangeKernel;
using dcl::network::message::msgSetKernelArg;
using dcl::network::message::msgGetKernelWorkGroupInfo;
using dcl::info::ndrange;
using dcl::info::kernel_group_info;
using dcl::info::generic_device;
using dcl::info::generic_kernel;
using dcl::info::generic_memory_object;
using dcl::info::generic_command_queue;
using dcl::info::generic_event;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_kernel::execute( const generic_command_queue* queue_ptr, 
                             const ndrange& offset, const ndrange& global, 
                             const ndrange& local, events_t& wait_events, 
                             generic_event** event_ptr )
{
    dcl_message< msgEnqueueNDRangeKernel >* msg_ptr = new dcl_message< msgEnqueueNDRangeKernel >();

    msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_remote_id() );
    msg_ptr->set_kernel_id( get_remote_id() );

    msg_ptr->get_offset().copy( offset );
    msg_ptr->get_global().copy( global );
    msg_ptr->get_local().copy( local );

    for( events_t::iterator it = wait_events.begin(); it != wait_events.end(); it++ )
    {
        reinterpret_cast<remote_event*>( *it )->wait_remote_id();

        msg_ptr->add_event( reinterpret_cast<remote_event*>( *it )->get_remote_id() );
    }

    message_sp_t message_sp( msg_ptr );

    if( event_ptr != NULL )
    {
        remote_event* ptr = new remote_event( context_ref_, message_sp );
        *event_ptr = reinterpret_cast<generic_event*>( ptr );

        ptr->set_remote_id( msg_ptr->get_event_id( *event_ptr ) );

        session_ref_.enqueue_message( message_sp );
    }
    else
    {
        session_ref_.send_message( message_sp );
        //session_ref_.enqueue_message( message_sp );
    }
}
//-----------------------------------------------------------------------------
void remote_kernel::set_argument( uint32_t arg_index, const generic_memory_object* memory_ptr )
{
    dcl_message< msgSetKernelArg >* msg_ptr = new dcl_message< msgSetKernelArg >();

    msg_ptr->set_index( arg_index );
    msg_ptr->set_kernel_id( get_remote_id() );
    
    if( memory_ptr->get_type() == CL_MEM_OBJECT_BUFFER )
    {
        msg_ptr->set_memory_id( reinterpret_cast<const remote_memory*>( memory_ptr )->get_remote_id() );
    }
    else if( memory_ptr->get_type() == CL_MEM_OBJECT_IMAGE2D )
    {
        msg_ptr->set_memory_id( reinterpret_cast<const remote_image*>( memory_ptr )->get_remote_id() );
    }

    message_sp_t message_sp( msg_ptr );
    session_ref_.enqueue_message( message_sp );
}
//-----------------------------------------------------------------------------
void remote_kernel::set_argument( uint32_t arg_index, size_t arg_size, const void* arg_value )
{
    dcl_message< msgSetKernelArg >* msg_ptr = new dcl_message< msgSetKernelArg >();

    msg_ptr->set_index( arg_index );
    msg_ptr->set_buffer( reinterpret_cast<const uint8_t*>( arg_value ), arg_size );
    msg_ptr->set_kernel_id( get_remote_id() );

    message_sp_t message_sp( msg_ptr );
    session_ref_.enqueue_message( message_sp );
}
//-----------------------------------------------------------------------------
const kernel_group_info& remote_kernel::get_group_info( const generic_device* device_ptr )
{
    kernel_group_info_map_t::iterator it = kernel_group_info_map_.find( device_ptr );

    if( it != kernel_group_info_map_.end() )
    {
        return it->second;
    }

    dcl_message< msgGetKernelWorkGroupInfo >* msg_ptr = new dcl_message< msgGetKernelWorkGroupInfo >();

    msg_ptr->set_kernel_id( get_remote_id() );
    msg_ptr->set_device_id( reinterpret_cast<const remote_device*>( device_ptr )->get_remote_id() );

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    kernel_group_info_map_.insert( kernel_group_info_map_t::value_type( device_ptr, msg_ptr->get_info() ) );

    return kernel_group_info_map_[ device_ptr ];
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
