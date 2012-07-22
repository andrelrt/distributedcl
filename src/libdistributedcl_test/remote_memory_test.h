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
#ifndef __REMOTE_MEMORY_TEST_H__
#define __REMOTE_MEMORY_TEST_H__

#include "distributedcl_internal.h"
#include "info/context_info.h"
#include "info/command_queue_info.h"
#include "remote/remote_opencl.h"
//-----------------------------------------------------------------------------
class remote_memory_test
{
protected:
    void init();
    void finish();

    void create_context_and_queue( dcl::info::generic_context** context_pptr,
                                   dcl::info::generic_command_queue** queue_pptr );

private:
    dcl::remote::remote_opencl* remote_ptr_;
};
//-----------------------------------------------------------------------------
#endif //  __REMOTE_MEMORY_TEST_H__
