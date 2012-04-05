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
#include <boost/scoped_array.hpp>
#include "platform.h"
#include "device.h"
#include "context.h"
#include "opencl_library.h"
#include "info/platform_info.h"
#include "info/context_info.h"
using dcl::info::generic_context;
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
platform::platform( const opencl_library& opencl, cl_platform_id platform_id )
    : opencl_object< cl_platform_id >( opencl, platform_id )
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

    cl_uint num_devices = 0;
    cl_int ret = opencl_.clGetDeviceIDs( id_, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices );

    if( ret != CL_SUCCESS )
    {
        throw library_exception( ret );
    }

    cl_uint num_entries = num_devices;
    boost::scoped_array<cl_device_id> deviceIds( new cl_device_id[ num_devices ] );

    ret = opencl_.clGetDeviceIDs( id_, CL_DEVICE_TYPE_ALL,
                                  num_entries, deviceIds.get(), &num_devices );
    if( ret != CL_SUCCESS )
    {
        throw new library_exception( ret );
    }

    for( cl_uint i = 0; i < num_entries; i++ )
    {
        add_device( new device( this, deviceIds[ i ] ) );
    }
}
//-----------------------------------------------------------------------------
void platform::load_string( cl_platform_info info, std::string& out )
{
    size_t value_size;

    out.clear();

    cl_int cl_error = opencl_.clGetPlatformInfo( id_, info, 0, NULL, &value_size );

    if( cl_error == CL_SUCCESS )
    {
        boost::scoped_array<char> param_value( new char[ value_size +1 ] );

        cl_error = opencl_.clGetPlatformInfo( id_, info, value_size, param_value.get(), &value_size );

        if( cl_error == CL_SUCCESS )
        {
            out.assign( param_value.get() );
        }
    }
}
//-----------------------------------------------------------------------------
generic_context* platform::create_context( const devices_t& devices ) const
{
    return new context( *this, devices );
}
//-----------------------------------------------------------------------------
generic_context* platform::create_context( cl_device_type device_type ) const
{
    return new context( *this, device_type );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
