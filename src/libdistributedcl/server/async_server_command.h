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
#ifndef _DCL_ASYNC_SERVER_SERVER_COMMAND_H_
#define _DCL_ASYNC_SERVER_SERVER_COMMAND_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "server_command.h"
#include "server_platform.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
template< dcl::network::message::message_type TYPE >
class async_server_command :
    public server_command< TYPE >
{
private:
    dcl::composite::composite_command_queue* queue_ptr_;

public:
    inline void async_execute( boost::shared_ptr< command > command_sp, remote_id_t queue_id )
    {
        this->session_context_ptr_->get_server_platform().enqueue( queue_id, command_sp );

        if( !async_run() )
        {
            this->session_context_ptr_->get_server_platform().flush( queue_id );
        }

        //if( async_run() )
        //{
        //    this->session_context_ptr_->get_server_platform().enqueue( queue_id, command_sp );
        //    //this->session_context_ptr_->get_server_platform().flush( queue_id );
        //    //async_server::get_instance().enqueue( command_sp );
        //}
        //else
        //{
        //    this->execute();
        //}
    }

    virtual bool get_blocking() const
    {
        return !async_run();
    }

    virtual void enqueue_response()
    {
        if( this->session_context_ptr_ != NULL )
        {
            this->session_context_ptr_->add_waiting_message( server_command< TYPE >::message_ );
        }
    }

    virtual void enqueue_error( int32_t error_code )
    {
        if( this->session_context_ptr_ != NULL )
        {
            message_sp_t
                err_msg_sp( new dcl::network::message::dcl_message< dcl::network::message::msg_error_message >( error_code ) );

            this->session_context_ptr_->add_waiting_message( err_msg_sp );
        }
    }

    dcl::composite::composite_command_queue* get_queue()
    {
        return queue_ptr_;
    }

    void set_command_event()
    {
        server_event_ = true;
    }

protected:
    bool server_event_;
    dcl::composite::composite_event* event_ptr_;

    async_server_command( message_sp_t message_sp, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< TYPE >( message_sp, session_context_ptr ),
        queue_ptr_( NULL ), server_event_( false ), event_ptr_( NULL ){}

    virtual bool async_run() const = 0;

    inline void set_command_queue( dcl::composite::composite_command_queue* queue_ptr )
    {
        queue_ptr_ = queue_ptr;

        if( server_event_ ||
            this->message_->get_return_event() )
        {
            event_ptr_ =
                new dcl::composite::composite_event( *(reinterpret_cast<const dcl::composite::composite_context*>(queue_ptr->get_context())), queue_ptr );

            if( this->message_->get_return_event() )
            {
                server_platform& server = this->session_context_ptr_->get_server_platform();
                server.get_event_manager().add( event_ptr_, this->message_->get_event_id() );
            }

            if( server_event_ )
            {
                this->message_->set_server_event( event_ptr_ );
            }
        }
    }
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif //_DCL_ASYNC_SERVER_SERVER_COMMAND_H_
