/*
 * Copyright (c) 2009-2010 André Tupinambá (andrelrt@gmail.com)
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
#include "platform.h"
#include <boost/scoped_array.hpp>
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
platform::platform( const opencl_library& opencl, cl_platform_id platform_id )
{
	load();
}
//-----------------------------------------------------------------------------
void platform::load()
{
    load_string( CL_PLATFORM_PROFILE, local_info_.profile_ );
	load_string( CL_PLATFORM_VERSION, local_info_.version_ );
	load_string( CL_PLATFORM_NAME, local_info_.name_ );
	load_string( CL_PLATFORM_VENDOR, local_info_.vendor_ );
	load_string( CL_PLATFORM_EXTENSIONS, local_info_.extensions_ );
}
//-----------------------------------------------------------------------------
void platform::load_string( cl_platform_info info, std::string& out )
{
    //size_t value_size;

    //out.clear();

    //cl_int cl_error = get_opencl().clGetPlatformInfo( get_id(), info, 0, NULL, &value_size );

    //if( cl_error == CL_SUCCESS )
    //{
    //    boost::scoped_array<char> param_value( new char[ value_size +1 ] );

    //    cl_error = get_opencl().clGetPlatformInfo( get_id(), info, value_size, param_value.get(), &value_size );

    //    if( cl_error == CL_SUCCESS )
    //    {
    //        out.assign( param_value.get() );
    //    }
    //}
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
