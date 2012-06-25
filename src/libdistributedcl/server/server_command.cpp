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
#include "server_command.h"
#include "server_platform.h"
using dcl::composite::composite_memory;
using dcl::composite::composite_command_queue;
using dcl::network::message::msgReleaseMemObject;
using dcl::network::message::msgReleaseCommandQueue;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
// async_server
//-----------------------------------------------------------------------------
//async_server* async_server::instance_ptr_ = NULL;
////-----------------------------------------------------------------------------
//async_server::~async_server()
//{
//    stop_ = true;
//    wait();
//}
////-----------------------------------------------------------------------------
//void async_server::enqueue( boost::shared_ptr< command > command_sp )
//{
//    scoped_lock_t lock( queue_mutex_ );
//
//    server_command_queue_.push( command_sp );
//
//    semaphore_.post();
//}
////-----------------------------------------------------------------------------
//void async_server::wait()
//{
//  flush();
//}
////-----------------------------------------------------------------------------
//void async_server::flush()
//{
//    scoped_lock_t lock_queue( execute_mutex_ );
//    while( 1 )
//    {
//        boost::shared_ptr< command > running_cmd;
//        {
//            scoped_lock_t lock_queue( queue_mutex_ );
//
//            if( server_command_queue_.empty() )
//                break;
//
//            running_cmd.swap( server_command_queue_.front() );
//
//            server_command_queue_.pop();
//        }
//
//        try
//        {
//            running_cmd->execute();
//            running_cmd->enqueue_response();
//        }
//        catch( dcl::library_exception& ex )
//        {
//            running_cmd->enqueue_error( ex.get_error() );
//        }
//        catch( ... )
//        {
//            running_cmd->enqueue_error( CL_INVALID_VALUE );
//        }
//
//        if( running_cmd->get_queue() != NULL )
//        {
//            queues_.insert( running_cmd->get_queue() );
//        }
//    }
//
//    std::set< composite_command_queue* >::iterator it;
//
//    for( it = queues_.begin(); it != queues_.end(); it++ )
//    {
//        (*it)->flush();
//    }
//
//    queues_.clear();
//}
////-----------------------------------------------------------------------------
//void async_server::work_thread()
//{
//    while( 1 )
//    {
//        semaphore_.wait();
//
//        if( stop_ )
//            return;
//
//        flush();
//    }
//}
//-----------------------------------------------------------------------------
// msg_flush_server_command
//-----------------------------------------------------------------------------
void msg_flush_server_command::execute()
{
//    async_server::get_instance().wait();
    session_context_ptr_->get_server_platform().flush_all();
}
//-----------------------------------------------------------------------------
// msg_get_context_command
//-----------------------------------------------------------------------------
void msg_get_context_command::execute()
{
    message_->set_remote_id( session_context_ptr_->get_server() );
}
//-----------------------------------------------------------------------------
// msg_attach_context_command
//-----------------------------------------------------------------------------
void msg_attach_context_command::execute()
{
    session_context_ptr_->attach_server( message_->get_remote_id() );
}
//-----------------------------------------------------------------------------
// release_command<msgReleaseCommandQueue, composite_command_queue>
//-----------------------------------------------------------------------------
void release_command<msgReleaseCommandQueue, composite_command_queue>::execute()
{
    session_context_ptr_->get_server_platform().close_queue( manager_.get( this->message_->get_remote_id() ) );
    manager_.remove( this->message_->get_remote_id() );
}
//-----------------------------------------------------------------------------
// release_command<msgReleaseMemObject, composite_memory>
//-----------------------------------------------------------------------------
void release_command<msgReleaseMemObject, composite_memory>::execute()
{
    server_platform& server = session_context_ptr_->get_server_platform();

    server.wait_all();

    if( server.get_memory_manager().has( message_->get_remote_id() ) )
    {
        server.get_memory_manager().remove( message_->get_remote_id() );
    }
    else if( server.get_image_manager().has( message_->get_remote_id() ) )
    {
        server.get_image_manager().remove( message_->get_remote_id() );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
