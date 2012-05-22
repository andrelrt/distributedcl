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
#include "server_event.h"
#include "server_platform.h"
#include "message/msg_event.h"
#include "composite/composite_event.h"
using dcl::composite::composite_event;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
void msgWaitForEvents_command::execute()
{
    server_platform& server = session_context_ptr_->get_server_platform();

    server.wait( message_->get_command_queue_id() );

    remote_id_t event_id = message_->get_remote_id();

    composite_event* event_ptr = server.get_event_manager().get( event_id );

    event_ptr->wait();
}
//-----------------------------------------------------------------------------
void msgGetEventProfilingInfo_command::execute()
{
    server_platform& server = session_context_ptr_->get_server_platform();

    server.wait( message_->get_command_queue_id() );

    remote_id_t event_id = message_->get_remote_id();

    composite_event* event_ptr = server.get_event_manager().get( event_id );

    event_ptr->load_info();

    message_->set_event_info( event_ptr->get_info() );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
