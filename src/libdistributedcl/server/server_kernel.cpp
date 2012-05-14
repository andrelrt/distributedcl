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
#include "server_kernel.h"
#include "server_platform.h"
#include "message/msg_kernel.h"
#include "composite/composite_device.h"
#include "composite/composite_program.h"
#include "composite/composite_kernel.h"
#include "composite/composite_command_queue.h"
#include "composite/composite_memory.h"
#include "composite/composite_event.h"
using dcl::info::generic_event;
using dcl::info::generic_device;
using dcl::info::kernel_group_info;
using dcl::composite::composite_device;
using dcl::composite::composite_program;
using dcl::composite::composite_kernel;
using dcl::composite::composite_command_queue;
using dcl::composite::composite_memory;
using dcl::composite::composite_event;
using dcl::composite::composite_image;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
void msgCreateKernel_command::execute()
{
    server_platform& server = server_platform::get_instance();

    remote_id_t program_id = message_->get_program_id();

    composite_program* program_ptr = server.get_program_manager().get( program_id );

    composite_kernel* kernel_ptr = 
        reinterpret_cast< composite_kernel* >( program_ptr->create_kernel( message_->get_name() ) );

    remote_id_t id = server.get_kernel_manager().add( kernel_ptr );

    message_->set_remote_id( id );
}
//-----------------------------------------------------------------------------
void msgEnqueueNDRangeKernel_command::execute()
{
    server_platform& server = server_platform::get_instance();

    composite_command_queue* queue_ptr = 
        server.get_command_queue_manager().get( message_->get_command_queue_id() );

    composite_kernel* kernel_ptr = 
        server.get_kernel_manager().get( message_->get_kernel_id() );

    dcl::events_t events;

    if( !message_->get_events().empty() )
    {
        async_server::get_instance().wait();

        events.reserve( message_->get_events().size() );

        dcl::remote_ids_t::const_iterator it;

        for( it = message_->get_events().begin(); it != message_->get_events().end(); it ++ )
        {
            events.push_back( reinterpret_cast<generic_event*>( server.get_event_manager().get( *it ) ) );
        }
    }

    if( message_->get_return_event() )
    {
        composite_event* ret_event = NULL;

        kernel_ptr->execute( queue_ptr, message_->get_offset(), 
                             message_->get_global(), message_->get_local(), events,
                             reinterpret_cast<generic_event**>( &ret_event ) );

        server.get_event_manager().add( ret_event, message_->get_event_id() );
    }
    else
    {
        kernel_ptr->execute( queue_ptr, message_->get_offset(), 
                             message_->get_global(), message_->get_local(), events,
                             NULL );
    }

    queue_ptr->flush();
    //queue_ptr->async_flush();
}
//-----------------------------------------------------------------------------
void msgSetKernelArg_command::execute()
{
//    std::cout<< "msgSetKernelArg_command::execute(): ";
    server_platform& server = server_platform::get_instance();

    composite_kernel* kernel_ptr = 
        server.get_kernel_manager().get( message_->get_kernel_id() );

    if( message_->is_memory_object() )
    {
        dcl::remote_id_t memory_id = message_->get_memory_id();

        if( server.get_memory_manager().has( memory_id ) )
        {
//            std::cout<< "server.get_memory_manager().has( memory_id = " << memory_id << " )";
            composite_memory* memory_ptr =
                server.get_memory_manager().get( memory_id );

            kernel_ptr->set_argument( message_->get_index(), memory_ptr );
        }
        else if( server.get_image_manager().has( memory_id ) )
        {
//            std::cout<< "server.get_image_manager().has( memory_id = " << memory_id << " )";
            composite_image* image_ptr =
                server.get_image_manager().get( memory_id );

            kernel_ptr->set_argument( message_->get_index(), image_ptr );
        }
        else
            throw dcl::library_exception( "Invalid memory object" );
    }
    else
    {
//        std::cout<< "kernel_ptr->set_argument(...)";
        kernel_ptr->set_argument( message_->get_index(),
                                  message_->get_buffer_size(),
                                  message_->get_buffer_pointer() );
    }
}
//-----------------------------------------------------------------------------
void msgGetKernelWorkGroupInfo_command::execute()
{
    server_platform& server = server_platform::get_instance();

    composite_kernel* kernel_ptr = 
        server.get_kernel_manager().get( message_->get_kernel_id() );

    composite_device* device_ptr = 
        server.get_device_manager().get( message_->get_device_id() );

    const kernel_group_info& group_info = 
        kernel_ptr->get_group_info( reinterpret_cast<generic_device*>( device_ptr ) );

    kernel_group_info& ret_info = message_->get_info();

    memcpy( &ret_info, &group_info, sizeof(kernel_group_info) );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
