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
#include "distributedcl_internal.h"
#include "cl_utils.h"
#include "icd/icd_object_manager.h"
#include "composite/opencl_composite.h"
#include "composite/composite_device.h"
#include "composite/composite_context.h"
#include "composite/composite_platform.h"
using dcl::devices_t;
using dcl::icd::icd_object_manager;
using dcl::info::generic_device;
using dcl::composite::composite_platform;
using dcl::composite::opencl_composite;
using dcl::composite::composite_device;
using dcl::composite::composite_context;
//-----------------------------------------------------------------------------
static composite_platform* find_platform_in_properties( const cl_context_properties* properties )
{
    if( properties != NULL )
    {
        int index = 0;
        while( properties[ index ] != 0 )
        {
            if( properties[ index ] == CL_CONTEXT_PLATFORM )
            {
                cl_platform_id platform_id = reinterpret_cast< cl_platform_id >( properties[ index + 1 ] );

                return icd_object_manager::get_instance().get_object_ptr< composite_platform >( platform_id );
            }

            index += 2;
        }
    }

    throw dcl::library_exception( CL_INVALID_PROPERTY );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_context CL_API_CALL
clCreateContext( const cl_context_properties* properties, cl_uint num_devices, 
                 const cl_device_id* devices, 
                 void (CL_CALLBACK * pfn_notify)(const char*,const void*,size_t,void*), 
                 void* user_data, cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_1
{
    if( (num_devices == 0) || (devices == NULL) ||
        ((pfn_notify == NULL) && (user_data != NULL)) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    try
    {
        composite_platform* platform_ptr = find_platform_in_properties( properties );
        icd_object_manager& icd = icd_object_manager::get_instance();

        devices_t devs;
        devs.reserve( num_devices );

        for( cl_uint i = 0; i < num_devices; i++ )
        {
            composite_device* dev_ptr = icd.get_object_ptr< composite_device >( devices[ i ] );

            devs.push_back( reinterpret_cast< generic_device* >( dev_ptr ) );
        }

        composite_context* context_ptr = reinterpret_cast< composite_context* >( platform_ptr->create_context( devs ) );

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }

        return icd.get_cl_id( context_ptr );
    }
    catch( dcl::library_exception& ex )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = ex.get_error();
        }
        return NULL;
    }
    catch( ... )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    // Dummy
    if( errcode_ret != NULL )
    {
        *errcode_ret = CL_INVALID_VALUE;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_context CL_API_CALL
clCreateContextFromType( const cl_context_properties* properties, cl_device_type device_type, 
                         void (CL_CALLBACK * pfn_notify)(const char*,const void*,size_t,void*), 
                         void* user_data, cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_1
{
    if( ((pfn_notify == NULL) && (user_data != NULL)) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    try
    {
        composite_platform* platform_ptr = find_platform_in_properties( properties );

        composite_context* context_ptr = reinterpret_cast< composite_context* >( platform_ptr->create_context( device_type ) );

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }

        return icd_object_manager::get_instance().get_cl_id( context_ptr );
    }
    catch( dcl::library_exception& ex )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = ex.get_error();
        }
        return NULL;
    }
    catch( ... )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    // Dummy
    if( errcode_ret != NULL )
    {
        *errcode_ret = CL_INVALID_VALUE;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clRetainContext( cl_context context ) CL_API_SUFFIX__VERSION_1_1
{
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clReleaseContext( cl_context context ) CL_API_SUFFIX__VERSION_1_1
{
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetContextInfo( cl_context context, cl_context_info param_name, size_t param_value_size, 
                  void* param_value, size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_1
{
    try
    {
        composite_context* context_ptr =
            get_info_check_parameters< composite_context >( context, param_value_size, 
                                                            param_value, param_value_size_ret );

        size_t info_size = 0;

        if( param_name == CL_CONTEXT_NUM_DEVICES )
        {
            info_size = sizeof( cl_uint );

            if( param_value != NULL )
            {
                *(reinterpret_cast< cl_uint* >( param_value )) =
                    static_cast<cl_uint>( context_ptr->get_devices().size() );
            }
        }
        else if( param_name == CL_CONTEXT_DEVICES )
        {
            const devices_t& devices_ref = context_ptr->get_devices();

            info_size = sizeof( cl_device_id ) * devices_ref.size();

            if( param_value != NULL )
            {
                cl_device_id* cl_devices_ptr = reinterpret_cast< cl_device_id* >( param_value );

                for( devices_t::const_iterator it = devices_ref.begin(); it != devices_ref.end(); it++ )
                {
                    *cl_devices_ptr = icd_object_manager::get_instance().get_cl_id< composite_device >( *it );

                    cl_devices_ptr++;
                }
            }
        }

        if( param_value_size_ret != NULL )
        {
            *param_value_size_ret = info_size;
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
