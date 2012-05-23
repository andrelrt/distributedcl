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
#ifndef _DCL_OBJECTS_H_
#define _DCL_OBJECTS_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
template< typename CL_TYPE_T, typename CL_TYPE_INFO, int CL_TYPE_INVALID_VALUE >
class cl_object
{
public:
    typedef CL_TYPE_T cl_type_t;
    typedef CL_TYPE_INFO cl_type_info_t;
    static const cl_int invalid_error_value = CL_TYPE_INVALID_VALUE;
};
//-----------------------------------------------------------------------------
template< typename DCL_TYPE_INFO >
class dcl_object
{
public:
    typedef DCL_TYPE_INFO dcl_info_t;

    inline const dcl_info_t& get_info() const
    {
        return local_info_;
    }

protected:
    dcl_info_t local_info_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_OBJECTS_H_
