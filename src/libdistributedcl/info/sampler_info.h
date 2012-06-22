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
#ifndef _DCL_INFO_SAMPLER_H_
#define _DCL_INFO_SAMPLER_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string.h>
#include "distributedcl_internal.h"
#include "library_exception.h"
#include "dcl_objects.h"
#include "icd_object.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
struct sampler_info
{
public:
    cl_bool normalized_coords_;
    cl_addressing_mode addressing_mode_;
    cl_filter_mode filter_mode_;

    sampler_info()
    {
        memset( this, 0, sizeof(sampler_info) );
    }

    inline size_t get_info_size( cl_device_info info ) const
    {
        switch( info )
        {
            case CL_SAMPLER_NORMALIZED_COORDS:  return sizeof(cl_bool);
            case CL_SAMPLER_ADDRESSING_MODE:    return sizeof(cl_addressing_mode);
            case CL_SAMPLER_FILTER_MODE:        return sizeof(cl_filter_mode);

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }

    inline const void* get_info_pointer( cl_device_info info ) const
    {
        switch( info )
        {
            case CL_SAMPLER_NORMALIZED_COORDS:  return &normalized_coords_;
            case CL_SAMPLER_ADDRESSING_MODE:    return &addressing_mode_;
            case CL_SAMPLER_FILTER_MODE:        return &filter_mode_;

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }
};
//-----------------------------------------------------------------------------
class generic_sampler :
    public cl_object< cl_sampler, cl_sampler_info, CL_INVALID_SAMPLER >,
    public icd_object< cl_sampler, dcl_sampler_id >,
    public dcl_object< sampler_info >
{
public:
    virtual ~generic_sampler(){}

    generic_sampler( cl_bool normalized_coords, cl_addressing_mode addressing_mode,
                     cl_filter_mode filter_mode )
    {
        local_info_.normalized_coords_ = normalized_coords;
        local_info_.addressing_mode_ = addressing_mode;
        local_info_.filter_mode_ = filter_mode;
    }

    inline const sampler_info& get_info() const
    {
        return local_info_;
    }

protected:
    sampler_info local_info_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_KERNEL_H_

