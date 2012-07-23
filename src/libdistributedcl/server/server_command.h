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
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <queue>
#include "distributedcl_internal.h"
#include "message/packet.h"
#include "message/message.h"
#include "message/msg_internal.h"
#include "network/server_session.h"
#include "composite/composite_command_queue.h"
#include "composite/composite_event.h"
#include "composite/composite_memory.h"
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
    virtual void async_prepare(){}
    virtual void enqueue_response(){}
    virtual void enqueue_error( int32_t error_code ){}
    virtual dcl::composite::composite_command_queue* get_queue(){ return NULL; }
    virtual bool get_blocking() const { return false; }
};
//-----------------------------------------------------------------------------
//class async_server
//{
//public:
//    ~async_server();
//
//    static async_server& get_instance()
//    {
//		if( instance_ptr_ == NULL )
//			instance_ptr_ = new async_server();
//			
//        return *instance_ptr_;
//    }
//
//    inline void flush_queue()
//    {
//        semaphore_.post();
//    }
//
//    void wait();
//    void enqueue( boost::shared_ptr< command > command_sp );
//
//private:
//    static async_server* instance_ptr_;
//
//    bool stop_;
//    mutex_t queue_mutex_;
//    mutex_t execute_mutex_;
//    boost::scoped_ptr< boost::thread > async_execute_sp_;
//    boost::interprocess::interprocess_semaphore semaphore_;
//    std::queue< boost::shared_ptr< command > > server_command_queue_;
//    std::set< dcl::composite::composite_command_queue* > queues_;
//
//    async_server() : stop_( false ), semaphore_( 0 )
//    {
//        async_execute_sp_.reset( new boost::thread( &dcl::server::async_server::work_thread, this ) );
//    }
//    
//    void flush();
//    void work_thread();
//};
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
template< dcl::network::message::message_type MESSAGE_T, class DCL_TYPE_T >
class release_command : public server_command< MESSAGE_T >
{
private:
    typedef typename boost::shared_ptr< dcl::network::message::release_message< MESSAGE_T > > release_message_sp_t;

    release_message_sp_t message_;
    dcl::info::object_manager< DCL_TYPE_T >& manager_;

    //virtual bool async_run() const{ return true; }

public:
	release_command( message_sp_t message_sp, dcl::info::object_manager< DCL_TYPE_T >& manager,
                     dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< MESSAGE_T >( message_sp, session_context_ptr ),
		message_( boost::static_pointer_cast< dcl::network::message::release_message< MESSAGE_T > >( message_sp ) ),
        manager_( manager ){}
		
	virtual void execute()
	{
        this->session_context_ptr_->get_server_platform().wait_all();
        manager_.remove( this->message_->get_remote_id() );
        //std::cerr << this->message_->get_remote_id();
	}
};
//-----------------------------------------------------------------------------
template<>
class release_command<dcl::network::message::msgReleaseCommandQueue, dcl::composite::composite_command_queue> :
    public server_command< dcl::network::message::msgReleaseCommandQueue >
{
private:
    typedef boost::shared_ptr< dcl::network::message::release_message< dcl::network::message::msgReleaseCommandQueue > > release_message_sp_t;

    release_message_sp_t message_;
    dcl::info::object_manager< dcl::composite::composite_command_queue >& manager_;

    //virtual bool async_run() const{ return true; }

public:
	release_command( message_sp_t message_sp, dcl::info::object_manager< dcl::composite::composite_command_queue >& manager,
                     dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgReleaseCommandQueue >( message_sp, session_context_ptr ),
		message_( boost::static_pointer_cast< dcl::network::message::release_message< dcl::network::message::msgReleaseCommandQueue > >( message_sp ) ),
        manager_( manager ){}
		
	virtual void execute();
};
//-----------------------------------------------------------------------------
template<>
class release_command<dcl::network::message::msgReleaseMemObject, dcl::composite::composite_memory> :
    public server_command< dcl::network::message::msgReleaseMemObject >
{
private:
    typedef boost::shared_ptr< dcl::network::message::release_message< dcl::network::message::msgReleaseMemObject > > release_message_sp_t;

    release_message_sp_t message_;

public:
	release_command( message_sp_t message_sp, dcl::info::object_manager< dcl::composite::composite_memory >& manager,
                     dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgReleaseMemObject >( message_sp, session_context_ptr ),
		message_( boost::static_pointer_cast< dcl::network::message::release_message< dcl::network::message::msgReleaseMemObject > >( message_sp ) )
        {}
		
	virtual void execute();
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
class msg_get_context_command :
    public server_command< dcl::network::message::msg_get_context >
{
public:
    msg_get_context_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msg_get_context >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class msg_attach_context_command :
    public server_command< dcl::network::message::msg_attach_context >
{
public:
    msg_attach_context_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msg_attach_context >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif //_DCL_SERVER_SERVER_COMMAND_H_
