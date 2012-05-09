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
#include "server_memory.h"
#include "server_platform.h"
#include "message/msg_memory.h"
#include "composite/composite_command_queue.h"
#include "composite/composite_context.h"
#include "composite/composite_memory.h"
#include "composite/composite_event.h"
using dcl::composite::composite_event;
using dcl::composite::composite_context;
using dcl::composite::composite_memory;
using dcl::composite::composite_command_queue;
using dcl::composite::composite_image;
using dcl::info::generic_memory_object;
using dcl::info::generic_event;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
void msgCreateBuffer_command::execute()
{
    server_platform& server = server_platform::get_instance();

    remote_id_t context_id = message_->get_context_id();

    composite_context* context_ptr = server.get_context_manager().get( context_id );

    generic_memory_object* ptr =
        context_ptr->create_buffer( message_->get_buffer_pointer(),
                                    message_->get_buffer_size(),
                                    message_->get_flags() );

    composite_memory* buffer_ptr = reinterpret_cast<composite_memory*>( ptr );

    remote_id_t id = server.get_memory_manager().add( buffer_ptr );

    message_->set_remote_id( id );
}
//-----------------------------------------------------------------------------
void msgEnqueueWriteBuffer_command::execute()
{
    server_platform& server = server_platform::get_instance();

    remote_id_t id = message_->get_remote_id();
    remote_id_t command_queue_id = message_->get_command_queue_id();

    composite_memory* buffer_ptr = server.get_memory_manager().get( id );
    composite_command_queue* queue_ptr = server.get_command_queue_manager().get( command_queue_id );

    dcl::events_t events;

    if( !message_->get_events().empty() )
    {
        async_server::get_instance().wait();

        events.reserve( message_->get_events().size() );

        for( dcl::remote_ids_t::const_iterator it = message_->get_events().begin(); it != message_->get_events().end(); it ++ )
        {
            events.push_back( reinterpret_cast<generic_event*>( server.get_event_manager().get( *it ) ) );
        }
    }

    if( message_->get_return_event() )
    {
        composite_event* ret_event = NULL;

        // Always no blocking
        buffer_ptr->write( queue_ptr, message_->get_buffer_pointer(),
                           message_->get_buffer_size(), 0,
                           false, events,
                           reinterpret_cast<generic_event**>( &ret_event ) );

        server.get_event_manager().add( ret_event, message_->get_event_id() );
    }
    else
    {
        // Always no blocking
        buffer_ptr->write( queue_ptr, message_->get_buffer_pointer(),
                           message_->get_buffer_size(), 0, 
                           false, events, NULL );
    }

    queue_ptr->flush();
    //queue_ptr->async_flush();
}
//-----------------------------------------------------------------------------
void msgEnqueueReadBuffer_command::execute()
{
    server_platform& server = server_platform::get_instance();

    remote_id_t id = message_->get_remote_id();
    remote_id_t command_queue_id = message_->get_command_queue_id();

    composite_memory* buffer_ptr = server.get_memory_manager().get( id );
    composite_command_queue* queue_ptr = server.get_command_queue_manager().get( command_queue_id );

    message_->allocate_buffer();

    dcl::events_t events;

    if( !message_->get_events().empty() )
    {
        async_server::get_instance().wait();

        events.reserve( message_->get_events().size() );

        for( dcl::remote_ids_t::const_iterator it = message_->get_events().begin(); it != message_->get_events().end(); it ++ )
        {
            events.push_back( reinterpret_cast<generic_event*>( server.get_event_manager().get( *it ) ) );
        }
    }

    if( message_->get_return_event() )
    {
        composite_event* ret_event = NULL;

        // Always blocking
        buffer_ptr->read( queue_ptr, message_->get_buffer_pointer(), 
                          message_->get_buffer_size(), message_->get_offset(), true,
                          events, reinterpret_cast<generic_event**>( &ret_event ) );

        server.get_event_manager().add( ret_event, message_->get_event_id() );
    }
    else
    {
        // Always blocking
        buffer_ptr->read( queue_ptr, message_->get_buffer_pointer(),
                          message_->get_buffer_size(), message_->get_offset(),
                          true, events, NULL );
    }

    queue_ptr->flush();
    //queue_ptr->async_flush();
}
//-----------------------------------------------------------------------------
void msgCreateImage2D_command::execute()
{
    server_platform& server = server_platform::get_instance();

    remote_id_t context_id = message_->get_context_id();

    composite_context* context_ptr = server.get_context_manager().get( context_id );

    cl_image_format format;
    format.image_channel_order = message_->get_channel_order();
    format.image_channel_data_type = message_->get_channel_type();

    generic_memory_object* ptr =
        context_ptr->create_image( message_->get_buffer(),
                                   message_->get_flags(), &format,
                                   message_->get_width(), message_->get_height(),
                                   message_->get_row_pitch() );

    composite_image* image_ptr = reinterpret_cast<composite_image*>( ptr );

    remote_id_t id = server.get_image_manager().add( image_ptr );

    message_->set_remote_id( id );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
