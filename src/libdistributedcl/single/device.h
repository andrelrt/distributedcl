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
#ifndef _DCL_DEVICE_H_
#define _DCL_DEVICE_H_

#include <boost/scoped_array.hpp>
#include "distributedcl_internal.h"
#include "opencl_single.h"
#include "info/device_info.h"
#include "opencl_library.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
class platform;
//-----------------------------------------------------------------------------
class device :
    public dcl::info::generic_device,
    public opencl_object< cl_device_id >
{
public:
    device( const platform* platform_ptr, cl_device_id id );
    virtual ~device(){}

private:
    virtual bool load_device_info();

    inline void load_info_data( cl_device_info info )
    {
        cl_int cl_error = opencl_.clGetDeviceInfo( id_, info, local_info_.get_info_size( info ), 
                                                   const_cast<void*>( local_info_.get_info_pointer( info ) ), NULL );

        if( cl_error != CL_SUCCESS )
            throw dcl::library_exception( cl_error );
    }

    inline void load_info_string( cl_device_info info, std::string& str )
    {
        size_t value_size;
        cl_int cl_error = opencl_.clGetDeviceInfo( id_, info, 0, NULL, &value_size );

        if( cl_error == CL_SUCCESS )
        {
            boost::scoped_array<char> param_value( new char[ value_size +1 ] );

            cl_error = opencl_.clGetDeviceInfo( id_, info, value_size, param_value.get(), &value_size );

            if( cl_error == CL_SUCCESS )
            {
                str.assign( param_value.get() );
            }
        }

        if( cl_error != CL_SUCCESS )
            throw dcl::library_exception( cl_error );
    }
};
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
#endif // _DCL_DEVICE_H_
