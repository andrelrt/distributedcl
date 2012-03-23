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
#include "server_kernel.h"
#include "server_platform.h"
#include "message/msg_kernel.h"
#include "composite/composite_program.h"
#include "composite/composite_kernel.h"
#include "composite/composite_command_queue.h"
#include "composite/composite_memory.h"
using dcl::composite::composite_program;
using dcl::composite::composite_kernel;
using dcl::composite::composite_command_queue;
using dcl::composite::composite_memory;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
void CreateKernel_command::execute()
{
    server_platform& server = server_platform::get_instance();

    remote_id_t program_id = message_.get_program_id();

    composite_program* program_ptr = server.get_program_manager().get( program_id );

    composite_kernel* kernel_ptr = 
        reinterpret_cast< composite_kernel* >( program_ptr->create_kernel( message_.get_name() ) );

    remote_id_t id = server.get_kernel_manager().add( kernel_ptr );

    message_.set_remote_id( id );
}
//-----------------------------------------------------------------------------
void EnqueueNDRangeKernel_command::execute()
{
    server_platform& server = server_platform::get_instance();

    composite_command_queue* queue_ptr = 
        server.get_command_queue_manager().get( message_.get_command_queue_id() );

    composite_kernel* kernel_ptr = 
        server.get_kernel_manager().get( message_.get_kernel_id() );

    kernel_ptr->execute( queue_ptr, message_.get_offset(), 
                         message_.get_global(), message_.get_local() );
}
//-----------------------------------------------------------------------------
void SetKernelArg_command::execute()
{
    server_platform& server = server_platform::get_instance();

    composite_kernel* kernel_ptr = 
        server.get_kernel_manager().get( message_.get_kernel_id() );

    if( message_.is_memory_object() )
    {
        composite_memory* memory_ptr =
            server.get_memory_manager().get( message_.get_memory_id() );

        kernel_ptr->set_argument( message_.get_index(), memory_ptr );
    }
    else
    {
        kernel_ptr->set_argument( message_.get_index(), 
                                  message_.get_buffer().size(), 
                                  message_.get_buffer().data() );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
