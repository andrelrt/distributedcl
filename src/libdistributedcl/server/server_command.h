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
#ifndef _DCL_SERVER_SERVER_COMMAND_H_
#define _DCL_SERVER_SERVER_COMMAND_H_

#include <queue>
#include "distributedcl_internal.h"
#include "message/packet.h"
#include "message/message.h"
#include "message/msg_internal.h"
#include "network/server_session.h"
#include "composite/composite_command_queue.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
class command
{
public:
    virtual void execute() = 0;
    virtual void enqueue_response(){}
    virtual void enqueue_error( int32_t error_code ){}
    virtual dcl::composite::composite_command_queue* get_queue(){ return NULL; }
};
//-----------------------------------------------------------------------------
class async_server
{
public:
    ~async_server();

    static async_server& get_instance()
    {
		if( instance_ptr_ == NULL )
			instance_ptr_ = new async_server();
			
        return *instance_ptr_;
    }

    inline void flush_queue()
    {
        semaphore_.post();
    }

    void wait();
    void enqueue( boost::shared_ptr< command > command_sp );

private:
    static async_server* instance_ptr_;

    bool stop_;
    mutex_t queue_mutex_;
    mutex_t execute_mutex_;
    boost::scoped_ptr< boost::thread > async_execute_sp_;
    boost::interprocess::interprocess_semaphore semaphore_;
    std::queue< boost::shared_ptr< command > > server_command_queue_;
    std::set< dcl::composite::composite_command_queue* > queues_;

    async_server() : stop_( false ), semaphore_( 0 )
    {
        async_execute_sp_.reset( new boost::thread( &dcl::server::async_server::work_thread, this ) );
    }
    
    void flush();
    void work_thread();
};
//-----------------------------------------------------------------------------
template< dcl::network::message::message_type TYPE >
class server_command : public command
{
protected:
    typedef typename boost::shared_ptr< dcl::network::message::dcl_message< TYPE > > dcl_message_sp_t;

    server_command( message_sp_t message_sp, dcl::network::server::server_session_context* session_context_ptr ) :
        message_( boost::static_pointer_cast< dcl::network::message::dcl_message< TYPE > >( message_sp ) ),
        session_context_ptr_( session_context_ptr ){}

    dcl_message_sp_t message_;
    dcl::network::server::server_session_context* session_context_ptr_;
};
//-----------------------------------------------------------------------------
template< dcl::network::message::message_type TYPE >
class async_server_command :
    public server_command< TYPE >
{
private:
    dcl::composite::composite_command_queue* queue_ptr_;

public:
    inline void async_execute( boost::shared_ptr< command > command_sp )
    {
		if( async_run() )
		{
			async_server::get_instance().enqueue( command_sp );
		}
		else
		{
			this->execute();
		}
    }

    virtual void enqueue_response()
    {
        if( this->session_context_ptr_ != NULL )
        {
            this->session_context_ptr_->add( server_command< TYPE >::message_ );
        }
    }

    virtual void enqueue_error( int32_t error_code )
    {
        if( this->session_context_ptr_ != NULL )
        {
            message_sp_t
                err_msg_sp( new dcl::network::message::dcl_message< dcl::network::message::msg_error_message >( error_code ) );

            this->session_context_ptr_->add( err_msg_sp );
        }
    }

    dcl::composite::composite_command_queue* get_queue()
    {
        return queue_ptr_;
    }

protected:
    async_server_command( message_sp_t message_sp, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< TYPE >( message_sp, session_context_ptr ), queue_ptr_( NULL ){}
        
	virtual bool async_run() const = 0;

    inline void set_command_queue( dcl::composite::composite_command_queue* queue_ptr )
    {
        queue_ptr_ = queue_ptr;
    }
};
//-----------------------------------------------------------------------------
template< dcl::network::message::message_type MESSAGE_T, class DCL_TYPE_T >
class release_command : public async_server_command< MESSAGE_T >
{
private:
    typedef typename boost::shared_ptr< dcl::network::message::release_message< MESSAGE_T > > release_message_sp_t;

    release_message_sp_t message_;
    dcl::info::object_manager< DCL_TYPE_T >& manager_;

    virtual bool async_run() const{ return true; }

public:
	release_command( message_sp_t message_sp, dcl::info::object_manager< DCL_TYPE_T >& manager,
                     dcl::network::server::server_session_context* session_context_ptr ) :
        async_server_command< MESSAGE_T >( message_sp, session_context_ptr ),
		message_( boost::static_pointer_cast< dcl::network::message::release_message< MESSAGE_T > >( message_sp ) ),
        manager_( manager ){}
		
	virtual void execute()
	{
        manager_.remove( this->message_->get_remote_id() );
	}
};
//-----------------------------------------------------------------------------
class msg_flush_server_command :
    public server_command< dcl::network::message::msg_flush_server >
{
public:
    msg_flush_server_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msg_flush_server >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif //_DCL_SERVER_SERVER_COMMAND_H_
