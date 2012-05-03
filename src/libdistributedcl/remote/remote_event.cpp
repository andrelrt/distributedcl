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
using dcl::network::message::dcl_message;
using dcl::network::message::base_message;
using dcl::network::message::enqueue_message;
using dcl::network::message::msgWaitForEvents;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_event::wait()
{
    wait_remote_id();

    dcl_message< msgWaitForEvents >* msg_ptr = new dcl_message< msgWaitForEvents >();

    msg_ptr->set_event_id( get_remote_id() );

    boost::shared_ptr< base_message > message_sp( msg_ptr );
    session_ref_.send_message( message_sp );
}
//-----------------------------------------------------------------------------
void remote_event::wait_remote_id()
{
    if( get_remote_id() == 0 )
    {
        get_session().flush_queue();

        enqueue_message* msg_ptr = reinterpret_cast<enqueue_message*>( message_sp_.get() );

        msg_ptr->wait();

        set_remote_id( msg_ptr->get_event_id() );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
