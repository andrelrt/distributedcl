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
#ifndef _DCL_SERVER_CONTEXT_H_
#define _DCL_SERVER_CONTEXT_H_

#include "distributedcl_internal.h"
#include "server_command.h"
#include "message/message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
class msgCreateContext_command : 
    public server_command< dcl::network::message::msgCreateContext >
{
public:
    msgCreateContext_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgCreateContext >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgCreateContextFromType_command : 
    public server_command< dcl::network::message::msgCreateContextFromType >
{
public:
    msgCreateContextFromType_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgCreateContextFromType >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgGetContextInfo_command :
    public server_command< dcl::network::message::msgGetContextInfo >
{
public:
    msgGetContextInfo_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgGetContextInfo >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif // _DCL_SERVER_CONTEXT_H_
