/*
 * Copyright (c) 2009-2010 André Tupinambá (andrelrt@gmail.com)
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
#ifndef _DCL_NETWORK_SESSION_H_
#define _DCL_NETWORK_SESSION_H_

#include "distributedcl_internal.h"
//-----------------------------------------------------------------------------
namespace dcl { namespace network { namespace message { class message; }}}
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace client {
//-----------------------------------------------------------------------------
class client;
//-----------------------------------------------------------------------------
class session
{
public:
    session( client& client_ref );
    ~session();

    void send_message( dcl::network::message::message& messege_ref );
    void enqueue_message( dcl::network::message::message& messege_ref );

private:
    client& client_ref_;
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::client
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_SESSION_H_
