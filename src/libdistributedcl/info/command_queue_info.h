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
#ifndef _DCL_INFO_COMMAND_QUEUE_H_
#define _DCL_INFO_COMMAND_QUEUE_H_

#include <string>
#include <queue>
#include "distributedcl_internal.h"
#include "library_exception.h"
#include "dcl_objects.h"
#include "icd_object.h"
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
class generic_device;
class generic_context;
//-----------------------------------------------------------------------------
struct command_queue_info
{
public:
    const generic_device* device_ptr_;
    const generic_context* context_ptr_;
    cl_command_queue_properties properties_;

    inline command_queue_info(){}

    inline command_queue_info( const generic_context* context_ptr, const generic_device* device_ptr, 
                               cl_command_queue_properties properties ) : 
        device_ptr_( device_ptr ), context_ptr_( context_ptr ), properties_( properties ){}
};
//-----------------------------------------------------------------------------
class generic_command_queue :
    public cl_object< cl_command_queue, cl_command_queue_info, CL_INVALID_COMMAND_QUEUE >,
    public icd_object< cl_command_queue, dcl_command_queue_id >,
    public dcl_object< command_queue_info >
{
public:
    generic_command_queue( const generic_context* context_ptr, const generic_device* device_ptr,
                           cl_command_queue_properties properties ) :
        async_semaphore_( 0 )
    {
        local_info_.device_ptr_ = device_ptr;
        local_info_.context_ptr_ = context_ptr;
        local_info_.properties_ = properties;

        async_worker_thread_sp_.reset( new boost::thread( &dcl::info::generic_command_queue::worker_thread, this ) );
    }

    virtual ~generic_command_queue()
    {
        {
            scoped_lock_t lock( queue_mutex_ );

            async_command_.push( cmd_terminate );
            async_semaphore_.post();
        }

        async_worker_thread_sp_->join();
    }

    inline const generic_device* get_device() const
    {
        return local_info_.device_ptr_;
    }

    inline const generic_context* get_context() const
    {
        return local_info_.context_ptr_;
    }

    inline cl_command_queue_properties get_properties() const
    {
        return local_info_.properties_;
    }

    virtual void flush() const = 0;
    virtual void finish() const = 0;

    inline void async_flush()
    {
        scoped_lock_t lock( queue_mutex_ );

        async_command_.push( cmd_flush );
        async_semaphore_.post();
    }

protected:
    std::queue<uint8_t> async_command_;
    boost::scoped_ptr< boost::thread > async_worker_thread_sp_;
    boost::interprocess::interprocess_mutex queue_mutex_;
    boost::interprocess::interprocess_semaphore async_semaphore_;

    enum queue_commands
    {
        cmd_invalid = 0,
        cmd_terminate = 1,
        cmd_flush = 2,
    };

    void worker_thread()
    {
        while( 1 )
        {
            async_semaphore_.wait();
            queue_commands command = cmd_invalid;

            {
                scoped_lock_t lock( queue_mutex_ );

                command = static_cast<queue_commands>( async_command_.front() );
                async_command_.pop();
            }

            if( command == cmd_terminate )
                break;

            switch( command )
            {
                case cmd_flush:
                    flush();
                    break;

                default:
                    dcl::library_exception( "Invalid queue_command" );
            }
        }
    }
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_COMMAND_QUEUE_H_
