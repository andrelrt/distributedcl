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
#include "server_command.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
// async_server
//-----------------------------------------------------------------------------
async_server* async_server::instance_ptr_ = NULL;
//-----------------------------------------------------------------------------
async_server::~async_server()
{
    stop_ = true;
    wait();
}
//-----------------------------------------------------------------------------
void async_server::enqueue( boost::shared_ptr< command > command_sp )
{
    scoped_lock_t lock( mutex_ );

    server_command_queue_.push( command_sp );

    //semaphore_.post();
}
//-----------------------------------------------------------------------------
void async_server::wait()
{
    semaphore_.post();
    while( 1 )
    {
        {
            scoped_lock_t lock( mutex_ );

            if( server_command_queue_.empty() )
                return;
        }

        boost::this_thread::sleep( boost::posix_time::milliseconds( 10 ) );
    }
}
//-----------------------------------------------------------------------------
void async_server::work_thread()
{
    while( 1 )
    {
        semaphore_.wait();

        if( stop_ )
            return;

        while( 1 )
        {
            boost::shared_ptr< command > running_cmd;
            {
                scoped_lock_t lock( mutex_ );

                if( server_command_queue_.empty() )
                    break;

                running_cmd.swap( server_command_queue_.front() );

                server_command_queue_.pop();
            }

            running_cmd->execute();
            running_cmd->enqueue_response();
        }
    }
}
//-----------------------------------------------------------------------------
// msg_flush_server_command
//-----------------------------------------------------------------------------
void msg_flush_server_command::execute()
{
    async_server::get_instance().wait();
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
