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
#include "remote_kernel.h"
#include "remote_command_queue.h"
#include "message/msg_kernel.h"
using dcl::network::message::dcl_message;
using dcl::network::message::base_message;
using dcl::network::message::msgEnqueueNDRangeKernel;
using dcl::info::ndrange;
using dcl::info::generic_kernel;
using dcl::info::generic_command_queue;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_kernel::execute( const generic_command_queue* queue_ptr, 
                             const ndrange& offset, const ndrange& global, 
                             const ndrange& local )
{
    dcl_message< msgEnqueueNDRangeKernel > msg;

    msg.set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_remote_id() );
    msg.set_kernel_id( get_remote_id() );

    msg.get_offset().copy( offset );
    msg.get_global().copy( global );
    msg.get_local().copy( local );

    session_ref_.send_message( reinterpret_cast< base_message* >( &msg ) );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
