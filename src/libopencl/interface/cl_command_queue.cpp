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
#include "composite/composite_context.h"
#include "composite/composite_device.h"
#include "composite/composite_command_queue.h"
using dcl::icd::icd_object_manager;
using dcl::composite::composite_context;
using dcl::composite::composite_device;
using dcl::composite::composite_command_queue;
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_command_queue CL_API_CALL
clCreateCommandQueue( cl_context context, cl_device_id device, 
                      cl_command_queue_properties properties,
                      cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_1
{
    if( (properties & ~(CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE)) != 0 )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_context* context_ptr = icd.get_object_ptr< composite_context >( context );
        composite_device* device_ptr = icd.get_object_ptr< composite_device >( device );

        composite_command_queue* command_queue_ptr =
            reinterpret_cast< composite_command_queue* >( context_ptr->create_command_queue( device_ptr, properties ) );

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }

        return icd.get_cl_id< composite_command_queue >( command_queue_ptr );
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
clRetainCommandQueue( cl_command_queue command_queue ) CL_API_SUFFIX__VERSION_1_1
{
    return retain_object< composite_command_queue >( command_queue );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clReleaseCommandQueue( cl_command_queue command_queue ) CL_API_SUFFIX__VERSION_1_1
{
    return release_object< composite_command_queue >( command_queue );
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clGetCommandQueueInfo( cl_command_queue command_queue, cl_command_queue_info param_name,
//                       size_t param_value_size, void* param_value,
//                       size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_1
//{
//    //FIXME: Not implemented
//    return CL_INVALID_COMMAND_QUEUE;
//}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clFlush( cl_command_queue command_queue ) CL_API_SUFFIX__VERSION_1_1
//{
//    //FIXME: Not implemented
//    return CL_INVALID_COMMAND_QUEUE;
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clFinish( cl_command_queue command_queue ) CL_API_SUFFIX__VERSION_1_1
{
    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_command_queue* command_queue_ptr =
            icd.get_object_ptr< composite_command_queue >( command_queue );

        command_queue_ptr->finish();

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_COMMAND_QUEUE;
    }

    // Dummy
    return CL_INVALID_COMMAND_QUEUE;
}
//-----------------------------------------------------------------------------
