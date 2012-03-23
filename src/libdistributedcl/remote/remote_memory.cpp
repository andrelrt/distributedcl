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
#include "remote_memory.h"
#include "remote_command_queue.h"
#include "message/msg_memory.h"
using dcl::info::generic_command_queue;
using dcl::network::message::dcl_message;
using dcl::network::message::base_message;
using dcl::network::message::msgEnqueueWriteBuffer;
using dcl::network::message::msgEnqueueReadBuffer;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_memory::write( generic_command_queue* queue_ptr, const void* data_ptr, 
                           size_t size, size_t offset, cl_bool blocking )
{
    dcl_message< msgEnqueueWriteBuffer >* msg_ptr = new dcl_message< msgEnqueueWriteBuffer >();

    msg_ptr->set_remote_id( get_remote_id() );
    msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_remote_id() );
    msg_ptr->set_buffer( reinterpret_cast<const uint8_t*>( data_ptr ), size, offset );

    boost::shared_ptr< base_message > message_sp( msg_ptr );
    session_ref_.send_message( message_sp );
}
//-----------------------------------------------------------------------------
void remote_memory::read( generic_command_queue* queue_ptr, void* data_ptr, 
                          size_t size, size_t offset, cl_bool blocking )
{
    dcl_message< msgEnqueueReadBuffer >* msg_ptr = new dcl_message< msgEnqueueReadBuffer >();

    msg_ptr->set_remote_id( get_remote_id() );
    msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_remote_id() );
    msg_ptr->set_buffer_size( size );
    msg_ptr->set_offset( offset );

    boost::shared_ptr< base_message > message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    memcpy( data_ptr, msg_ptr->get_buffer().data(), size );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
