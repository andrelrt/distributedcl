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
#include "opencl_memory_test.h"
#include "single/opencl_library.h"
#include "single/memory.h"
using dcl::info::generic_memory;
//-----------------------------------------------------------------------------
opencl_memory_test::opencl_memory_test()
{
}
//-----------------------------------------------------------------------------
generic_memory* opencl_memory_test::create_memory()
{
    opencl_library* library_ptr = new opencl_library( libpath );
    opencl_single* opencl_ptr = new opencl_single( *library_ptr );
}
//-----------------------------------------------------------------------------
void opencl_memory_test::release_memory( generic_memory* memory_ptr )
{
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
