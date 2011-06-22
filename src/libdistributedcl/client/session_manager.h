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
#ifndef _DCL_CLIENT_SESSION_MANAGER_H_
#define _DCL_CLIENT_SESSION_MANAGER_H_

#include "distributedcl_internal.h"
#include "client_session.h"
#include "network/tcp_transport.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace client {
//-----------------------------------------------------------------------------
class session_manager
{
public:
    typedef client_session< dcl::network::platform::tcp_transport > session_t;

    inline static session_t& create_session( const std::string& connection_string )
    {
        if( instance_.session_ptr_ == NULL )
        {
            session_t::config_info_t config_info;

            sockaddr_in* sin = reinterpret_cast< sockaddr_in* >( &(config_info.bind_addr) );
            sin->sin_addr.s_addr = inet_addr( "127.0.0.1" );

            instance_.session_ptr_ = new session_t( config_info );

            instance_.session_ptr_->connect();
        }

        return *(instance_.session_ptr_);
    }

private:
    static session_manager instance_;
    session_t* session_ptr_;

    session_manager() : session_ptr_( NULL ){}

    ~session_manager()
    {
        delete session_ptr_;
        session_ptr_ = NULL;
    }
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::client
//-----------------------------------------------------------------------------
#endif // _DCL_CLIENT_SESSION_MANAGER_H_
