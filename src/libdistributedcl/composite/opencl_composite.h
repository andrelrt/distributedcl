/*
 * Copyright (c) 2009-2011 André Tupinambá (andrelrt@gmail.com)
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
#ifndef _DCL_OPENCL_COMPOSITE_H_
#define _DCL_OPENCL_COMPOSITE_H_

#include <string>
#include <set>
#include "distributedcl_internal.h"
#include "composite_platform.h"
#include "composite_context.h"
//-----------------------------------------------------------------------------
namespace dcl { namespace single { 
class opencl_single;
class opencl_library;
}}
namespace dcl { namespace remote { 
class remote_opencl;
}}
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
class opencl_composite
{
public:
    static opencl_composite& get_instance()
    {
        return instance_;
    }

    ~opencl_composite()
    {
        free_all();
    }

    void add_library( const std::string& libpath );
    void add_remote( const std::string& connection_string );

    const composite_platform& get_platform() const
    {
        return comp_platform_;
    }

    void unload_compiler();
    void free_all();

private:
    opencl_composite(){}
    typedef std::set< dcl::single::opencl_single* > opencl_set_t;
    typedef std::set< dcl::single::opencl_library* > library_set_t;
    typedef std::set< const dcl::remote::remote_opencl* > remote_set_t;

    remote_set_t remote_set_;
    opencl_set_t opencl_set_;
    library_set_t library_set_;
    composite_platform comp_platform_;
    static opencl_composite instance_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
#endif // _DCL_OPENCL_COMPOSITE_H_
