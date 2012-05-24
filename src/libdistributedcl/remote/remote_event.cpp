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
#include "remote_event.h"
#include "remote_command_queue.h"
#include "message/msg_event.h"
#include "message/msg_internal.h"
using dcl::network::message::dcl_message;
using dcl::network::message::base_message;
using dcl::network::message::enqueue_message;
using dcl::network::message::msg_flush_server;
using dcl::network::message::msgWaitForEvents;
using dcl::network::message::msgGetEventProfilingInfo;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_event::wait()
{
    dcl_message< msgWaitForEvents >* msg_ptr = new dcl_message< msgWaitForEvents >();

    msg_ptr->set_remote_id( get_remote_id() );
    msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr_ )->get_remote_id() );

    message_sp_t message_sp( msg_ptr );
    //session_ref_.send_message( message_sp );

    reinterpret_cast<const remote_command_queue*>( queue_ptr_ )->get_queue_session().send_message( message_sp );
}
//-----------------------------------------------------------------------------
void remote_event::load_info()
{
    if( (local_info_.end_ == 0)    ||
        (local_info_.start_ == 0)  ||
        (local_info_.submit_ == 0) ||
        (local_info_.queued_ == 0) )
    {
        dcl_message< msgGetEventProfilingInfo >* msg_ptr =
            new dcl_message< msgGetEventProfilingInfo >();

        msg_ptr->set_remote_id( get_remote_id() );
        msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr_ )->get_remote_id() );

        message_sp_t message_sp( msg_ptr );
        session_ref_.send_message( message_sp );

        local_info_ = msg_ptr->get_event_info();
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
