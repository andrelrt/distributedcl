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
#include "async_server_command.h"
#include "server_platform.h"
#include "message/message.h"
#include "message/msg_memory.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
class msgCreateBuffer_command : 
    public server_command< dcl::network::message::msgCreateBuffer >
{
public:
    msgCreateBuffer_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgCreateBuffer >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgEnqueueWriteBuffer_command : 
    public async_server_command< dcl::network::message::msgEnqueueWriteBuffer >
{
public:
    msgEnqueueWriteBuffer_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        async_server_command< dcl::network::message::msgEnqueueWriteBuffer >( message_ptr, session_context_ptr )
    {
        server_platform& server = session_context_ptr_->get_server_platform();

        dcl::composite::composite_command_queue* queue_ptr = 
            server.get_command_queue_manager().get( message_->get_command_queue_id() );

        set_command_queue( queue_ptr );
    }

    void execute();
    virtual bool async_run() const;
    virtual void async_prepare();
};
//-----------------------------------------------------------------------------
class msgEnqueueReadBuffer_command : 
    public async_server_command< dcl::network::message::msgEnqueueReadBuffer >
{
public:
    msgEnqueueReadBuffer_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        async_server_command< dcl::network::message::msgEnqueueReadBuffer >( message_ptr, session_context_ptr )
    {
        if( message_->get_blocking() )
            set_command_event();

        server_platform& server = session_context_ptr_->get_server_platform();

        dcl::composite::composite_command_queue* queue_ptr = 
            server.get_command_queue_manager().get( message_->get_command_queue_id() );

        set_command_queue( queue_ptr );
    }

    void execute();
    virtual bool async_run() const;
};
//-----------------------------------------------------------------------------
class msgEnqueueCopyBuffer_command : 
    public async_server_command< dcl::network::message::msgEnqueueCopyBuffer >
{
public:
    msgEnqueueCopyBuffer_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        async_server_command< dcl::network::message::msgEnqueueCopyBuffer >( message_ptr, session_context_ptr )
    {
        if( message_->get_blocking() )
            set_command_event();

        server_platform& server = session_context_ptr_->get_server_platform();

        dcl::composite::composite_command_queue* queue_ptr = 
            server.get_command_queue_manager().get( message_->get_command_queue_id() );

        set_command_queue( queue_ptr );
    }

    void execute();
    virtual bool async_run() const;
};
//-----------------------------------------------------------------------------
class msgCreateImage2D_command : 
    public server_command< dcl::network::message::msgCreateImage2D >
{
public:
    msgCreateImage2D_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgCreateImage2D >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msgEnqueueWriteImage_command :
    public async_server_command< dcl::network::message::msgEnqueueWriteImage >
{
public:
    msgEnqueueWriteImage_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        async_server_command< dcl::network::message::msgEnqueueWriteImage >( message_ptr, session_context_ptr )
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
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif // _DCL_SERVER_MEMORY_H_
