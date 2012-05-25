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
#ifndef _DCL_SERVER_KERNEL_H_
#define _DCL_SERVER_KERNEL_H_

#include "distributedcl_internal.h"
#include "async_server_command.h"
#include "server_platform.h"
#include "message/message.h"
#include "message/msg_kernel.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
class msgCreateKernel_command : 
    public server_command< dcl::network::message::msgCreateKernel >
{
public:
    msgCreateKernel_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgCreateKernel >( message_ptr, session_context_ptr ){}

    void execute();
};
//-----------------------------------------------------------------------------
class msgEnqueueNDRangeKernel_command : 
    public async_server_command< dcl::network::message::msgEnqueueNDRangeKernel >
{
public:
    msgEnqueueNDRangeKernel_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        async_server_command< dcl::network::message::msgEnqueueNDRangeKernel >( message_ptr, session_context_ptr )
    {
        server_platform& server = session_context_ptr_->get_server_platform();

        dcl::composite::composite_command_queue* queue_ptr = 
            server.get_command_queue_manager().get( message_->get_command_queue_id() );

        set_command_queue( queue_ptr );
    }

    void execute();
    virtual bool async_run() const;
};
//-----------------------------------------------------------------------------
class msgSetKernelArg_command : 
    public server_command< dcl::network::message::msgSetKernelArg >
{
public:
    msgSetKernelArg_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgSetKernelArg >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgGetKernelWorkGroupInfo_command :
    public server_command< dcl::network::message::msgGetKernelWorkGroupInfo >
{
public:
    msgGetKernelWorkGroupInfo_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgGetKernelWorkGroupInfo >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif // _DCL_SERVER_KERNEL_H_
