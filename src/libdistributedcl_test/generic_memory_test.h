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
#ifndef __GENERIC_MEMORY_TEST_H__
#define __GENERIC_MEMORY_TEST_H__

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/portability/Stream.h>
#include "distributedcl_internal.h"
#include "info/memory_info.h"
//-----------------------------------------------------------------------------
class generic_memory_test : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( generic_memory_test );
    CPPUNIT_TEST( testReset );
    CPPUNIT_TEST( testResetShouldFail );
    CPPUNIT_TEST_SUITE_END();

    dcl::info::generic_memory *memory_ptr_;

protected:
    virtual dcl::info::generic_memory* create_memory() = 0;
    virtual void release_memory( dcl::info::generic_memory* memory_ptr ) = 0;

public:
    generic_memory_test();
    void setUp();
    void tearDown();


};
//-----------------------------------------------------------------------------
#endif //  __GENERIC_MEMORY_TEST_H__
