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
#ifndef _DCL_SERVER_MEMORY_H_
#define _DCL_SERVER_MEMORY_H_

#include "distributedcl_internal.h"
#include "server_command.h"
#include "message/message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
class msgCreateBuffer_command : 
    public server_command< dcl::network::message::msgCreateBuffer >
{
public:
    msgCreateBuffer_command( message_sp_t message_ptr ) :
        server_command< dcl::network::message::msgCreateBuffer >( message_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgEnqueueWriteBuffer_command : 
    public async_server_command< dcl::network::message::msgEnqueueWriteBuffer >
{
public:
    msgEnqueueWriteBuffer_command( message_sp_t message_ptr,
                                   dcl::network::server::server_messages* waiting_messages_ptr ) :
        async_server_command< dcl::network::message::msgEnqueueWriteBuffer >( message_ptr, waiting_messages_ptr ) {}

    void execute();
    virtual bool async_run() const;
};
//-----------------------------------------------------------------------------
class msgEnqueueReadBuffer_command : 
    public async_server_command< dcl::network::message::msgEnqueueReadBuffer >
{
public:
    msgEnqueueReadBuffer_command( message_sp_t message_ptr,
                                  dcl::network::server::server_messages* waiting_messages_ptr ) :
        async_server_command< dcl::network::message::msgEnqueueReadBuffer >( message_ptr, waiting_messages_ptr ) {}

    void execute();
    virtual bool async_run() const;
};
//-----------------------------------------------------------------------------
class msgCreateImage2D_command : 
    public server_command< dcl::network::message::msgCreateImage2D >
{
public:
    msgCreateImage2D_command( message_sp_t message_ptr ) :
        server_command< dcl::network::message::msgCreateImage2D >( message_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif // _DCL_SERVER_MEMORY_H_
