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
#include "server_platform.h"
#include "message/message.h"
#include "message/msg_device.h"
#include "composite/opencl_composite.h"
#include "composite/composite_platform.h"
#include "composite/composite_device.h"
using dcl::network::message::dcl_message;
using dcl::network::message::msgGetDeviceIDs;
using dcl::composite::opencl_composite;
using dcl::composite::composite_platform;
using dcl::composite::composite_device;
using dcl::composite::composite_command_queue;
using dcl::remote_id_t;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
void msgGetDeviceIDs_command::execute()
{
    // Double the devices to hide the network latency
    //load_devices();
    load_devices();

    message_->update_response_size();
}
//-----------------------------------------------------------------------------
void msgGetDeviceIDs_command::load_devices()
{
    const composite_platform& platform = opencl_composite::get_instance().get_platform();
    const devices_t& devs = platform.get_devices();

    for( devices_t::const_iterator it = devs.begin(); it != devs.end(); it++ )
    {
        composite_device* device_ptr = reinterpret_cast< composite_device* >( *it );
        remote_id_t id = session_context_ptr_->get_server_platform().get_device_manager().add( device_ptr );

        switch( device_ptr->get_type() )
        {
            case CL_DEVICE_TYPE_CPU:
                message_->add_cpu_device( id );
                break;

            case CL_DEVICE_TYPE_GPU:
                message_->add_gpu_device( id );
                break;

            case CL_DEVICE_TYPE_ACCELERATOR:
                message_->add_accelerator_device( id );
                break;

            default:
                message_->add_other_device( id );
                break;
        }
    }
}
//-----------------------------------------------------------------------------
void msgGetDeviceInfo_command::execute()
{
    remote_id_t remote_id = message_->get_remote_id();

    const composite_device* device_ptr = session_context_ptr_->get_server_platform().get_device_manager().get( remote_id );

    message_->set_info( device_ptr->get_info() );
}
//-----------------------------------------------------------------------------
// async_execute
//-----------------------------------------------------------------------------
async_execute::async_execute( composite_command_queue* queue_ptr ) :
    running_( false ), queue_ptr_( queue_ptr ), semaphore_( 0 )
{
    thread_sp_.reset( new boost::thread( &dcl::server::async_execute::work_thread, this ) );
}
//-----------------------------------------------------------------------------
async_execute::~async_execute()
{
    stop();
    thread_sp_->join();
}
//-----------------------------------------------------------------------------
void async_execute::stop()
{
    running_ = false;
    semaphore_.post();
}
//-----------------------------------------------------------------------------
void async_execute::enqueue( boost::shared_ptr<command> command_sp )
{
    dcl::scoped_lock_t lock( mutex_ );

    //if( command_sp->get_queue()->get_device()->get_type() == CL_DEVICE_TYPE_CPU )
    //{
    //    // TODO: lower the thread priority
    //}

    server_queue_.push_back( command_sp );

    //if( command_sp->get_blocking() )
    //{
    //    blocking_count_++;
    //}
}
//-----------------------------------------------------------------------------
void async_execute::flush()
{
    semaphore_.post();
}
////-----------------------------------------------------------------------------
//void async_execute::wait()
//{
//    execute_queue( false, true );
//}
////-----------------------------------------------------------------------------
//void async_execute::wait_unblock()
//{
//    execute_queue( true, true );
//}
////-----------------------------------------------------------------------------
//bool async_execute::has_blocking_command()
//{
//    return( blocking_count_ != 0 );
//}
//-----------------------------------------------------------------------------
void async_execute::execute_queue()
{
    dcl::scoped_lock_t lock( mutex_ );

    while( !server_queue_.empty() )
    {
		try
		{
			server_queue_.front()->execute();
			server_queue_.front()->enqueue_response();
			server_queue_.pop_front();
		}
		catch( dcl::library_exception& ex )
		{
			server_queue_.front()->enqueue_error( ex.get_error() );
			server_queue_.clear();
		}
    }
}
//-----------------------------------------------------------------------------
void async_execute::work_thread()
{
    running_ = true;
    while( 1 )
    {
        semaphore_.wait();

        if( !running_ )
            return;

        execute_queue();

        //barrier_sp_->wait();
        queue_ptr_->flush();
    }
}
//-----------------------------------------------------------------------------
// server_platform
//-----------------------------------------------------------------------------
server_platform::~server_platform()
{
    clear_all_data();
}
//-----------------------------------------------------------------------------
void server_platform::clear_all_data()
{
    for( queue_thread_map_t::iterator it = queue_thread_.begin(); it != queue_thread_.end(); it++ )
    {
        delete it->second;
    }

    queue_thread_.clear();

    kernel_manager_.clear();
    command_queue_manager_.clear();
    memory_manager_.clear();
    image_manager_.clear();

    program_manager_.clear();
    event_manager_.clear();
    context_manager_.clear();
}
//-----------------------------------------------------------------------------
void server_platform::open_queue( composite_command_queue* queue_ptr )
{
    queue_thread_[ queue_ptr ] = new async_execute( queue_ptr );

    //barrier_sp_ = boost::shared_ptr<boost::interprocess::barrier>(
    //    new boost::interprocess::barrier( queue_thread_.size() + 1 ) );

    //for( queue_thread_map_t::iterator it = queue_thread_.begin(); it != queue_thread_.end(); it++ )
    //{
    //    it->second->setup_barrier( barrier_sp_ );
    //}
}
//-----------------------------------------------------------------------------
void server_platform::enqueue( remote_id_t queue_id, boost::shared_ptr<command> command_sp )
{
    composite_command_queue* queue_ptr = command_queue_manager_.get( queue_id );

    queue_thread_[ queue_ptr ]->enqueue( command_sp );
}
//-----------------------------------------------------------------------------
void server_platform::flush( remote_id_t queue_id )
{
    composite_command_queue* queue_ptr = command_queue_manager_.get( queue_id );

    queue_thread_[ queue_ptr ]->flush();
}
//-----------------------------------------------------------------------------
//void server_platform::wait( remote_id_t queue_id )
//{
//    flush( queue_id );
//
//    composite_command_queue* queue_ptr = command_queue_manager_.get( queue_id );
//
//    queue_thread_[ queue_ptr ]->wait();
//}
////-----------------------------------------------------------------------------
//void server_platform::wait_unblock( remote_id_t queue_id )
//{
//    flush( queue_id );
//
//    composite_command_queue* queue_ptr = command_queue_manager_.get( queue_id );
//
//    queue_thread_[ queue_ptr ]->wait_unblock();
//}
////-----------------------------------------------------------------------------
//void server_platform::wait_all()
//{
//    if( queue_thread_.size() != 0 )
//    {
//        flush_all();
//
//        barrier_sp_->wait();
//    }
//}
////-----------------------------------------------------------------------------
//void server_platform::wait_unblock_all()
//{
//    wait_all();
//}
//-----------------------------------------------------------------------------
void server_platform::flush_all()
{
    for( queue_thread_map_t::iterator it = queue_thread_.begin(); it != queue_thread_.end(); it++ )
    {
        it->second->flush();
    }

    boost::this_thread::yield();
}
//-----------------------------------------------------------------------------
void server_platform::close_queue( composite_command_queue* queue_ptr )
{
    delete queue_thread_[ queue_ptr ];
    queue_thread_.erase( queue_ptr );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
