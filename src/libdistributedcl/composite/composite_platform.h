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
#ifndef _DCL_COMPOSITE_PLATFORM_H_
#define _DCL_COMPOSITE_PLATFORM_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <set>
#include "distributedcl_internal.h"
#include "info/platform_info.h"
#include "info/context_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
class composite_platform :
    public dcl::info::generic_platform
{
public:
    composite_platform()
    {
        //create_icd_obj( this );
    }
    ~composite_platform(){}

    inline void load_info(){}

    const devices_t& get_devices() const
    { 
        return devices_;
    }

    void get_devices( devices_t& devices, cl_device_type device_type ) const;

    dcl::info::generic_context* create_context( const dcl::devices_t& devices ) const;
    dcl::info::generic_context* create_context( cl_device_type device_type = CL_DEVICE_TYPE_ALL ) const;


    void add_platform( dcl::info::generic_platform* platform_ptr )
    {
        platforms_.push_back( platform_ptr );

        const devices_t& devs = platform_ptr->get_devices();

        devices_.insert( devices_.end(), devs.begin(), devs.end() );
    }

    //inline void add_devices( const dcl::single::devices_t& devices_ref )
    //{
    //    devices_.insert( devices_.end(), devices_ref.begin(), devices_ref.end() );
    //}

    //inline const dcl::single::platforms_t& get_platforms() const
    //{
    //    return platforms_;
    //}

    //inline const dcl::single::devices_t& get_devices() const
    //{
    //    return devices_;
    //}

private:
    platforms_t platforms_;
    devices_t devices_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
#endif //_DCL_COMPOSITE_PLATFORM_H_
