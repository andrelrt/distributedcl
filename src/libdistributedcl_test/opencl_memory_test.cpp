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
#include <gtest/gtest.h>
#include "single/opencl_library.h"
#include "single/opencl_single.h"
#include "single/context.h"
#include "single/command_queue.h"
#include "opencl_memory_test.h"

#if defined( WIN32 )
#define OPENCL_LIBRARY  "C:\\WINDOWS\\System32\\OpenCL.dll"
#else
#define OPENCL_LIBRARY  "/usr/lib/libOpenCL.so.1"
#endif

using dcl::platforms_t;
using dcl::devices_t;
using dcl::events_t;
using dcl::single::opencl_single;
using dcl::single::opencl_library;
using dcl::info::generic_context;
using dcl::info::generic_memory;
using dcl::info::generic_command_queue;
//-----------------------------------------------------------------------------
void opencl_memory_test::init()
{
    library_ptr_ = new opencl_library( OPENCL_LIBRARY );
    opencl_ptr_ = new opencl_single( *library_ptr_ );
}
//-----------------------------------------------------------------------------
void opencl_memory_test::finish()
{
    delete opencl_ptr_;
    delete library_ptr_;
}
//-----------------------------------------------------------------------------
void opencl_memory_test::create_context_and_queue( generic_context** context_pptr,
                                                   generic_command_queue** queue_pptr )
{
    const platforms_t& platforms = opencl_ptr_->get_platforms();
    const devices_t& devices = platforms[ 0 ]->get_devices();

    *context_pptr = platforms[ 0 ]->create_context();
    *queue_pptr = (*context_pptr)->create_command_queue( devices[ 0 ], 0 );
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
