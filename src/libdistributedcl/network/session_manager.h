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
#ifndef _DCL_CLIENT_SESSION_MANAGER_H_
#define _DCL_CLIENT_SESSION_MANAGER_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

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
        session_t::config_info_t config_info( connection_string );

        session_t* session_ptr = new session_t( config_info );

        session_ptr->connect();

        get_instance()->sessions_.insert( session_ptr );

        return *session_ptr;
    }

private:
	static session_manager* get_instance()
	{
		if( instance_ptr_ == NULL )
			instance_ptr_ = new session_manager();
			
		return instance_ptr_;
	}
	
    static session_manager* instance_ptr_;
    std::set<session_t*> sessions_;

    session_manager(){}

    ~session_manager()
    {
        std::set<session_t*>::iterator it;

        for( it = sessions_.begin(); it != sessions_.end(); it++ )
        {
            delete *it;
        }
        sessions_.clear();
    }
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::client
//-----------------------------------------------------------------------------
#endif // _DCL_CLIENT_SESSION_MANAGER_H_
