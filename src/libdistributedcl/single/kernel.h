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
#ifndef _DCL_KERNEL_H_
#define _DCL_KERNEL_H_

#include <map>
#include <string>
#include "distributedcl_internal.h"
#include "single_object.h"
#include "opencl_library.h"
#include "info/kernel_info.h"
#include "program.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
class kernel :
    public dcl::info::generic_kernel,
    public opencl_object< cl_kernel >,
    public context_object< kernel >
{
public:
    kernel( const program& program_ref, const std::string& name );
    ~kernel();

    virtual void execute( const dcl::info::generic_command_queue* queue_ptr, 
                          const dcl::info::ndrange& offset, 
                          const dcl::info::ndrange& global, 
                          const dcl::info::ndrange& local,
                          events_t& wait_events, dcl::info::generic_event** event_ptr = NULL );

    virtual void set_argument( uint32_t arg_index, const dcl::info::generic_memory_object* memory_ptr );
    virtual void set_argument( uint32_t arg_index, size_t arg_size, const void* arg_value );
    virtual const dcl::info::kernel_group_info& get_group_info( const dcl::info::generic_device* device_ptr );
};
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
#endif //_DCL_KERNEL_H_
