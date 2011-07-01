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
#include "distributedcl_internal.h"
#include "cl_utils.h"
#include "info/device_info.h"
#include "composite/composite_platform.h"
#include "composite/opencl_composite.h"
#include "composite/composite_device.h"
using dcl::devices_t;
using dcl::info::generic_device;
using dcl::composite::icd_object_manager;
using dcl::composite::composite_platform;
using dcl::composite::opencl_composite;
using dcl::composite::composite_device;
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
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_platform* platform_ptr = icd.get_object_ptr< composite_platform >( platform );

        devices_t devs;
        platform_ptr->get_devices( devs, device_type );

        cl_uint count = static_cast< cl_uint >( devs.size() );

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
                devices[ i ] = icd.get_cl_id< composite_device >( reinterpret_cast< composite_device* >( devs[ i ] ) ); 
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
                 void *param_value, size_t *param_value_size_ret ) CL_API_SUFFIX__VERSION_1_1
{
    try
    {
        if( param_name != CL_DEVICE_PLATFORM )
        {
            get_info< composite_device >( device, param_name, param_value_size, param_value, param_value_size_ret );
        }
        else
        {
            get_info_check_parameters< composite_device >( device, param_value_size, param_value, param_value_size_ret );

            if( param_value != NULL )
            {
                if( param_value_size < sizeof(cl_platform_id) )
                {
                    throw dcl::library_exception( CL_INVALID_VALUE );
                }

                *(reinterpret_cast< cl_platform_id* >( param_value )) = opencl_composite::get_instance().get_platform().get_icd_obj();
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
