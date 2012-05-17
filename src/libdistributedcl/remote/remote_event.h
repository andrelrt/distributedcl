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
#ifndef _DCL_REMOTE_EVENT_H_
#define _DCL_REMOTE_EVENT_H_

#include "distributedcl_internal.h"
#include "remote_object.h"
#include "remote_context.h"
#include "info/event_info.h"
#include <boost/shared_ptr.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
//-----------------------------------------------------------------------------
//namespace dcl {
//namespace network {
//namespace message {
//class enqueue_message;
//}}}
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_command_queue;
//-----------------------------------------------------------------------------
class remote_event :
    public dcl::info::generic_event,
    public remote_object< remote_event, dcl::network::message::msgReleaseEvent >
{
public:
    remote_event( const remote_context& context_ref, dcl::remote_id_t id ) :
        remote_object< remote_event, dcl::network::message::msgReleaseEvent >( context_ref.get_session() )
    {
        set_remote_id( id );
    }

    remote_event( const remote_context& context_ref, message_sp_t message_sp ) :
        remote_object< remote_event, dcl::network::message::msgReleaseEvent >( context_ref.get_session() ),
        message_sp_( message_sp ) {}

    ~remote_event(){}

    virtual void wait();
    virtual void load_info();
    void wait_remote_id();

private:
    message_sp_t message_sp_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_EVENT_H_
