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
#include "distributedcl_internal.h"
#include "cl_utils.h"
#include "remote/remote_device.h"
#include "remote/remote_platform.h"
#include "remote/icd_object_manager.h"
using dcl::remote::remote_device;
using dcl::remote::remote_platform;
using dcl::remote::remote_devices_t;
using dcl::remote::icd_object_manager;
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetDeviceIDs( cl_platform_id platform, cl_device_type device_type, cl_uint num_entries,
                cl_device_id *devices, cl_uint *num_devices ) CL_API_SUFFIX__VERSION_1_1
{
    if( ( (num_entries == 0) && (devices != NULL) ) ||
        ( (devices == NULL) && (num_devices == NULL) ) )
    {
        return CL_INVALID_VALUE;
    }

    try
    {
        remote_platform* remote_platform_ptr = icd_object_manager::get_instance().get_object_ptr< remote_platform >( platform );

        const remote_devices_t& remote_devices_ref = remote_platform_ptr->get_devices();

        cl_uint count = static_cast< cl_uint >( remote_devices_ref.size() );

        if( num_devices != NULL )
        {
            *num_devices = count;
        }

        if( devices != NULL )
        {
            if( count > num_entries )
            {
                count = num_entries;
            }

            for( cl_uint i = 0; i < count; i++ )
            {
                devices[ i ] = reinterpret_cast< cl_device_id >( remote_devices_ref[ i ] );
            }
        }
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    return CL_SUCCESS;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetDeviceInfo( cl_device_id device, cl_device_info param_name, size_t param_value_size,
                 void *param_value, size_t *param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
{
    try
    {
        get_info< remote_device >( device, param_name, param_value_size, param_value, param_value_size_ret );
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    return CL_SUCCESS;
}
//-----------------------------------------------------------------------------
