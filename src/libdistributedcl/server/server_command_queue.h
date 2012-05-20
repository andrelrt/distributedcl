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
#ifndef _DCL_SERVER_COMMAND_QUEUE_H_
#define _DCL_SERVER_COMMAND_QUEUE_H_

#include "distributedcl_internal.h"
#include "server_command.h"
#include "message/message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
class msgCreateCommandQueue_command :
    public server_command< dcl::network::message::msgCreateCommandQueue >
{
public:
    msgCreateCommandQueue_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgCreateCommandQueue >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgFlush_command :
    public server_command< dcl::network::message::msgFlush >
{
public:
    msgFlush_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgFlush >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgFinish_command :
    public server_command< dcl::network::message::msgFinish >
{
public:
    msgFinish_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgFinish >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif // _DCL_SERVER_COMMAND_QUEUE_H_
