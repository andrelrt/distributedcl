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
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif
#ifndef _DCL_REMOTE_OBJECT_H_
#define _DCL_REMOTE_OBJECT_H_

#include "distributedcl_internal.h"
#include "network/session_manager.h"
#include "message/msg_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
template< typename DCL_TYPE_T, dcl::network::message::message_type MSG_RELEASE_OBJECT >
class remote_object
{
public:
    inline dcl::network::client::session_manager::session_t& get_session() const
    {
        return session_ref_;
    }

    inline dcl::remote_id_t get_remote_id() const
    {
        return remote_id_;
    }

    inline void set_remote_id( dcl::remote_id_t remote_id )
    {
        remote_id_ = remote_id;
    }
    
    virtual ~remote_object()
    {
		if( MSG_RELEASE_OBJECT != dcl::network::message::msg_dummy_message )
		{
            // Ensures that all send queues are empty
            session_ref_.flush_childs();
            
            try
            {
			    dcl::network::message::release_message< MSG_RELEASE_OBJECT >* msg_ptr =
				    new dcl::network::message::release_message< MSG_RELEASE_OBJECT >();

			    msg_ptr->set_remote_id( remote_id_ );

			    message_sp_t message_sp( msg_ptr );
			    session_ref_.send_message( message_sp ); //enqueue
            }
            catch(...)
            {
                // hidding exception
            }
		}
	}

protected:
    dcl::network::client::session_manager::session_t& session_ref_;

    remote_object( dcl::network::client::session_manager::session_t& session_ref ) :
        session_ref_( session_ref ), remote_id_( 0 ) {}

private:
    dcl::remote_id_t remote_id_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_OBJECT_H_
