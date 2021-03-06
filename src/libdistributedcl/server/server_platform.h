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
#ifndef _DCL_SERVER_PLATFORM_H_
#define _DCL_SERVER_PLATFORM_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <deque>
#include "distributedcl_internal.h"
#include "server_command.h"
#include "info/object_manager.h"
#include "message/message.h"
#include "composite/composite_device.h"
#include "composite/composite_context.h"
#include "composite/composite_program.h"
#include "composite/composite_kernel.h"
#include "composite/composite_command_queue.h"
#include "composite/composite_memory.h"
#include "composite/composite_event.h"
#include "composite/composite_sampler.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
class msgGetDeviceIDs_command : 
    public server_command< dcl::network::message::msgGetDeviceIDs >
{
public:
    msgGetDeviceIDs_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgGetDeviceIDs >( message_ptr, session_context_ptr ) {}

    void execute();
    void load_devices();
};
//-----------------------------------------------------------------------------
class msgGetDeviceInfo_command : 
    public server_command< dcl::network::message::msgGetDeviceInfo >
{
public:
    msgGetDeviceInfo_command( message_sp_t message_ptr, dcl::network::server::server_session_context* session_context_ptr ) :
        server_command< dcl::network::message::msgGetDeviceInfo >( message_ptr, session_context_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class async_execute
{
public:
    async_execute( dcl::composite::composite_command_queue* queue_ptr );
    ~async_execute();

    void stop();
    void enqueue( boost::shared_ptr<command> command_sp );
    void flush();
    void wait();

    inline bool empty() const
    {
        return server_queue_.empty();
    }

private:
    volatile bool running_;
    //uint32_t blocking_count_;
    std::deque< boost::shared_ptr<command> > server_queue_;
    dcl::composite::composite_command_queue* queue_ptr_;

    boost::scoped_ptr<boost::thread> thread_sp_;
    boost::interprocess::interprocess_mutex mutex_;
    boost::interprocess::interprocess_mutex condition_mutex_;
    boost::interprocess::interprocess_semaphore semaphore_;

    void execute_queue();
    void work_thread();
};
//-----------------------------------------------------------------------------
class server_platform
{
public:
    server_platform(){}
    ~server_platform();

    typedef dcl::info::object_manager< dcl::composite::composite_event > event_manager_t;
    typedef dcl::info::object_manager< dcl::composite::composite_image > image_manager_t;
    typedef dcl::info::object_manager< dcl::composite::composite_device > device_manager_t;
    typedef dcl::info::object_manager< dcl::composite::composite_kernel > kernel_manager_t;
    typedef dcl::info::object_manager< dcl::composite::composite_memory > memory_manager_t;
    typedef dcl::info::object_manager< dcl::composite::composite_context > context_manager_t;
    typedef dcl::info::object_manager< dcl::composite::composite_program > program_manager_t;
    typedef dcl::info::object_manager< dcl::composite::composite_sampler > sampler_manager_t;
    typedef dcl::info::object_manager< dcl::composite::composite_command_queue > command_queue_manager_t;

    inline event_manager_t& get_event_manager(){ return event_manager_; }
    inline image_manager_t& get_image_manager(){ return image_manager_; }
    inline device_manager_t& get_device_manager(){ return device_manager_; }
    inline kernel_manager_t& get_kernel_manager(){ return kernel_manager_; }
    inline memory_manager_t& get_memory_manager(){ return memory_manager_; }
    inline context_manager_t& get_context_manager(){ return context_manager_; }
    inline program_manager_t& get_program_manager(){ return program_manager_; }
    inline sampler_manager_t& get_sampler_manager(){ return sampler_manager_; }
    inline command_queue_manager_t& get_command_queue_manager(){ return command_queue_manager_; }

    void clear_all_data();
    void open_queue( dcl::composite::composite_command_queue* queue_ptr );
    void enqueue( remote_id_t queue_id, boost::shared_ptr<command> command_sp );
    void wait( remote_id_t queue_id );
//    void wait_unblock( remote_id_t queue_id );
    void wait_all();
    //void wait_unblock_all();
    void close_queue( dcl::composite::composite_command_queue* queue_ptr );
    void flush_all();
    void flush( remote_id_t queue_id );

private:
    device_manager_t device_manager_;
    context_manager_t context_manager_;
    program_manager_t program_manager_;
    kernel_manager_t kernel_manager_;
    command_queue_manager_t command_queue_manager_;
    memory_manager_t memory_manager_;
    event_manager_t event_manager_;
    image_manager_t image_manager_;
    sampler_manager_t sampler_manager_;
    
    typedef std::map< dcl::composite::composite_command_queue*, async_execute* > queue_thread_map_t;

    queue_thread_map_t queue_thread_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif // _DCL_SERVER_PLATFORM_H_
