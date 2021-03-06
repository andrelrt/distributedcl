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
#ifndef __GENERIC_MEMORY_TEST_H__
#define __GENERIC_MEMORY_TEST_H__

#include <gtest/gtest.h>
#include "distributedcl_internal.h"
#include "info/context_info.h"
#include "info/command_queue_info.h"
//-----------------------------------------------------------------------------
template<typename T>
class generic_memory_test :
    public testing::Test,
    public T
{
protected:
    void SetUp()
    {
        T::init();
        T::create_context_and_queue( &context_ptr_, &queue_ptr_ );
    }

    void TearDown()
    {
        delete queue_ptr_;
        delete context_ptr_;

        T::finish();
    }

    dcl::info::generic_context* context_ptr_;
    dcl::info::generic_command_queue* queue_ptr_;
};
//-----------------------------------------------------------------------------
#endif //  __GENERIC_MEMORY_TEST_H__
