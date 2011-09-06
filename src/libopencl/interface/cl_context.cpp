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
#include "composite/opencl_composite.h"
#include "composite/composite_device.h"
#include "composite/composite_context.h"
#include "composite/composite_platform.h"
using dcl::devices_t;
using dcl::info::generic_device;
using dcl::composite::icd_object_manager;
using dcl::composite::composite_platform;
using dcl::composite::opencl_composite;
using dcl::composite::composite_device;
using dcl::composite::composite_context;
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

    if( properties == NULL )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_PROPERTY;
        }
        return NULL;
    }

    try
    {
        int index = 0;
        while( properties[ index ] != 0 )
        {
            if( properties[ index ] == CL_CONTEXT_PLATFORM )
            {
                icd_object_manager& icd = icd_object_manager::get_instance();

                cl_platform_id platform_id = reinterpret_cast< cl_platform_id >( properties[ index + 1 ] );
                composite_platform* platform_ptr = icd.get_object_ptr< composite_platform >( platform_id );

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

            index += 2;
        }

        // Property CL_CONTEXT_PLATFORM not found
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_PROPERTY;
        }
        return NULL;
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
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
