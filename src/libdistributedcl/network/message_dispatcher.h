/*
 * Copyright (c) 2009-2012 Andr� Tupinamb� (andrelrt@gmail.com)
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
#ifndef _DCL_NETWORK_MESSAGE_DISPATCHER_H_
#define _DCL_NETWORK_MESSAGE_DISPATCHER_H_

#include "distributedcl_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace server {
//-----------------------------------------------------------------------------
class server_messages;
//-----------------------------------------------------------------------------
class message_dispatcher
{
public:
    void dispatch_messages( dcl::message_vector_t& messages, server_messages* waiting_messages_ptr );
    void flush_async_queue();
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::server
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_MESSAGE_DISPATCHER_H_
