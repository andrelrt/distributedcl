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
#include "server_command_queue.h"
#include "server_platform.h"
#include "message/msg_command_queue.h"
#include "composite/composite_device.h"
#include "composite/composite_context.h"
#include "composite/composite_command_queue.h"
using dcl::composite::composite_device;
using dcl::composite::composite_context;
using dcl::composite::composite_command_queue;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
void msgCreateCommandQueue_command::execute()
{
    server_platform& server = session_context_ptr_->get_server_platform();

    composite_context* context_ptr =
        server.get_context_manager().get( message_->get_context_id() );

    composite_device* device_ptr =
        server.get_device_manager().get( message_->get_device_id() );

    composite_command_queue* command_queue_ptr =
        reinterpret_cast< composite_command_queue* >
        ( context_ptr->create_command_queue( device_ptr, message_->get_properties() ) );

    remote_id_t id = server.get_command_queue_manager().add( command_queue_ptr );
    server.open_queue( command_queue_ptr );

    message_->set_remote_id( id );
}
//-----------------------------------------------------------------------------
void msgFlush_command::execute()
{
    server_platform& server = session_context_ptr_->get_server_platform();

    server.flush( message_->get_remote_id() );

    composite_command_queue* queue_ptr = 
        server.get_command_queue_manager().get( message_->get_remote_id() );

    queue_ptr->flush();
}
//-----------------------------------------------------------------------------
void msgFinish_command::execute()
{
    server_platform& server = session_context_ptr_->get_server_platform();

    server.wait( message_->get_remote_id() );

    composite_command_queue* queue_ptr = 
        server.get_command_queue_manager().get( message_->get_remote_id() );

    queue_ptr->finish();
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
