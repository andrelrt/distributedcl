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
#include "server_command.h"
#include "message/message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
class msgCreateKernel_command : 
    public server_command< dcl::network::message::msgCreateKernel >
{
public:
    msgCreateKernel_command( message_sp_t message_ptr ) :
        server_command< dcl::network::message::msgCreateKernel >( message_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgEnqueueNDRangeKernel_command : 
    public server_command< dcl::network::message::msgEnqueueNDRangeKernel >
{
public:
    msgEnqueueNDRangeKernel_command( message_sp_t message_ptr ) :
        server_command< dcl::network::message::msgEnqueueNDRangeKernel >( message_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgSetKernelArg_command : 
    public server_command< dcl::network::message::msgSetKernelArg >
{
public:
    msgSetKernelArg_command( message_sp_t message_ptr ) :
        server_command< dcl::network::message::msgSetKernelArg >( message_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgGetKernelWorkGroupInfo_command :
    public server_command< dcl::network::message::msgGetKernelWorkGroupInfo >
{
public:
    msgGetKernelWorkGroupInfo_command( message_sp_t message_ptr ) :
        server_command< dcl::network::message::msgGetKernelWorkGroupInfo >( message_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif // _DCL_SERVER_KERNEL_H_
