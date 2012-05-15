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
#ifndef _DCL_SERVER_SERVER_COMMAND_H_
#define _DCL_SERVER_SERVER_COMMAND_H_

#include <queue>
#include "distributedcl_internal.h"
#include "message/packet.h"
#include "message/message.h"
#include "message/msg_internal.h"
#include "network/server_session.h"
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
};
//-----------------------------------------------------------------------------
class async_server
{
public:
    ~async_server();

    static async_server& get_instance()
    {
        return instance_;
    }

    inline void flush_queue()
    {
        semaphore_.post();
    }

    void wait();
    void enqueue( boost::shared_ptr< command > command_sp );

private:
    static async_server instance_;

    bool stop_;
    mutex_t mutex_;
    boost::scoped_ptr< boost::thread > async_execute_sp_;
    boost::interprocess::interprocess_semaphore semaphore_;
    std::queue< boost::shared_ptr< command > > server_command_queue_;

    async_server() : stop_( false ), semaphore_( 0 )
    {
        async_execute_sp_.reset( new boost::thread( &dcl::server::async_server::work_thread, this ) );
    }

    void work_thread();
};
//-----------------------------------------------------------------------------
template< dcl::network::message::message_type TYPE >
class server_command : public command
{
protected:
    typedef typename boost::shared_ptr< dcl::network::message::dcl_message< TYPE > > dcl_message_sp_t;

    server_command( message_sp_t message_sp ) :
        message_( boost::static_pointer_cast< dcl::network::message::dcl_message< TYPE > >( message_sp ) ){}

    dcl_message_sp_t message_;
};
//-----------------------------------------------------------------------------
template< dcl::network::message::message_type TYPE >
class async_server_command :
    public server_command< TYPE >
{
public:
    inline void async_execute( boost::shared_ptr< command > command_sp )
    {
        async_server::get_instance().enqueue( command_sp );
    }

    virtual void enqueue_response()
    {
        waiting_messages_ptr_->add( server_command< TYPE >::message_ );
    }

protected:
    async_server_command( message_sp_t message_sp, dcl::network::server::server_messages* waiting_messages_ptr ) :
        server_command< TYPE >( message_sp ),
        waiting_messages_ptr_( waiting_messages_ptr ){}

private:
    dcl::network::server::server_messages* waiting_messages_ptr_;
};
//-----------------------------------------------------------------------------
class msg_flush_server_command :
    public server_command< dcl::network::message::msg_flush_server >
{
public:
    msg_flush_server_command( message_sp_t message_ptr ) : 
        server_command< dcl::network::message::msg_flush_server >( message_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif //_DCL_SERVER_SERVER_COMMAND_H_
