/*
 * Copyright (c) 2009-2011 André Tupinambá (andrelrt@gmail.com)
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
#ifndef _DCL_LOCALHOST_SESSION_H_
#define _DCL_LOCALHOST_SESSION_H_

#include "distributedcl_internal.h"
#include "client/client.h"
#include "client/session.h"
using dcl::network::client::session;
//-----------------------------------------------------------------------------
namespace dcl {
namespace opencl {
//-----------------------------------------------------------------------------
class client;
//-----------------------------------------------------------------------------
class local_session
{
public:
    static local_session& get_instance()
    {
        if( local_session_ptr_ == NULL )
        {
            local_session_ptr_ = new local_session();
        }
        return *local_session_ptr_;
    }

    session& get_session()
    {
        return *session_ptr_;
    }

private:
    static local_session* local_session_ptr_;
    session* session_ptr_;

    local_session()
    {
        //FIXME: Change the follow lines to establish the network session to localhost
        dcl::network::client::client network_client;
        session client_session( network_client );
    }

    ~local_session()
    {
    }

};
//-----------------------------------------------------------------------------
local_session* local_session::session_ptr_ = NULL;
//-----------------------------------------------------------------------------
}} // namespace dcl::opencl
//-----------------------------------------------------------------------------
#endif // _DCL_LOCALHOST_SESSION_H_
