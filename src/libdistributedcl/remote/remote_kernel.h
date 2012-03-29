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
#ifndef _DCL_REMOTE_KERNEL_H_
#define _DCL_REMOTE_KERNEL_H_

#include "distributedcl_internal.h"
#include "remote_object.h"
#include "remote_context.h"
#include "info/kernel_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_kernel :
    public dcl::info::generic_kernel,
    public remote_object< remote_kernel >
{
public:
    remote_kernel( const remote_context& context_ref, const std::string& name ) :
        dcl::info::generic_kernel( name ), 
        remote_object< remote_kernel >( context_ref.get_session() ) {}

    ~remote_kernel(){}

    virtual void execute( const dcl::info::generic_command_queue* queue_ptr, 
                          const dcl::info::ndrange& offset, 
                          const dcl::info::ndrange& global, 
                          const dcl::info::ndrange& local,
                          events_t& wait_events, dcl::info::generic_event** event_ptr = NULL );

    virtual void set_argument( uint32_t arg_index, const dcl::info::generic_memory* memory_ptr );
    virtual void set_argument( uint32_t arg_index, size_t arg_size, const void* arg_value );
    virtual const dcl::info::kernel_group_info& get_group_info( const dcl::info::generic_device* device_ptr );
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_KERNEL_H_
