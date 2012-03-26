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
#include "composite/composite_program.h"
#include "composite/composite_kernel.h"
#include "composite/composite_memory.h"
#include "composite/composite_device.h"
#include "composite/composite_command_queue.h"
#include "composite/composite_event.h"
using dcl::info::ndrange;
using dcl::info::generic_event;
using dcl::info::kernel_group_info;
using dcl::icd::icd_object_manager;
using dcl::composite::composite_program;
using dcl::composite::composite_kernel;
using dcl::composite::composite_memory;
using dcl::composite::composite_device;
using dcl::composite::composite_command_queue;
using dcl::composite::composite_event;
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_kernel CL_API_CALL
clCreateKernel( cl_program program, const char* kernel_name,
                cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_0
{
    if( kernel_name == NULL )
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

        composite_program* program_ptr = icd.get_object_ptr< composite_program >( program );
        composite_kernel* kernel_ptr =
            reinterpret_cast< composite_kernel* >( program_ptr->create_kernel( kernel_name ) );

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }

        return icd.get_cl_id< composite_kernel >( kernel_ptr );
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
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clCreateKernelsInProgram( cl_program program, cl_uint num_kernels, cl_kernel* kernels, 
//                          cl_uint* num_kernels_ret ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clRetainKernel( cl_kernel kernel ) CL_API_SUFFIX__VERSION_1_1
{
    return retain_object< composite_kernel >( kernel );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clReleaseKernel( cl_kernel kernel ) CL_API_SUFFIX__VERSION_1_1
{
    return release_object< composite_kernel >( kernel );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clSetKernelArg( cl_kernel kernel, cl_uint arg_index, size_t arg_size,
                const void* arg_value ) CL_API_SUFFIX__VERSION_1_1
{
    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_kernel* kernel_ptr = icd.get_object_ptr< composite_kernel >( kernel );

        if( (arg_value != NULL) && 
            (arg_size == sizeof(void*)) )
        {
            cl_mem memory = *(reinterpret_cast<const cl_mem*>( arg_value ));

            if( icd.has_object< composite_memory >( memory ) )
            {
                composite_memory* memory_ptr = icd.get_object_ptr< composite_memory >( memory );

                kernel_ptr->set_argument( arg_index, memory_ptr );
                return CL_SUCCESS;
            }
        }

        kernel_ptr->set_argument( arg_index, arg_size, arg_value );

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clGetKernelInfo( cl_kernel kernel, cl_kernel_info param_name,
//                 size_t param_value_size, void* param_value,
//                 size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetKernelWorkGroupInfo( cl_kernel kernel, cl_device_id device,
                          cl_kernel_work_group_info param_name,
                          size_t param_value_size, void* param_value,
                          size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_1
{
    try
    {
        composite_kernel* kernel_ptr = 
            get_info_check_parameters< composite_kernel >( kernel, param_value_size, 
                                                           param_value, param_value_size_ret );

        icd_object_manager& icd = icd_object_manager::get_instance();
        composite_device* device_ptr = icd.get_object_ptr< composite_device >( device );

        const kernel_group_info& info = kernel_ptr->get_group_info( device_ptr );

        size_t size;
        const void* data;

        switch( param_name )
        {
            case CL_KERNEL_WORK_GROUP_SIZE:
                size = sizeof(size_t);
                data = &info.work_group_size_;
                break;

            case CL_KERNEL_COMPILE_WORK_GROUP_SIZE:
                size = 3*sizeof(size_t);
                data = info.compile_work_group_size_;
                break;

            case CL_KERNEL_LOCAL_MEM_SIZE:
                size = sizeof(cl_ulong);
                data = &info.local_mem_size_;
                break;

            case CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE:
                size = sizeof(size_t);
                data = &info.preferred_work_group_size_multiple_;
                break;

            case CL_KERNEL_PRIVATE_MEM_SIZE:
                size = sizeof(cl_ulong);
                data = &info.private_mem_size_;
                break;

            default:
                return CL_INVALID_VALUE;
        }

        if( param_value_size_ret != NULL )
            *param_value_size_ret = size;

        if( (param_value != NULL) && (param_value_size < size) )
            return CL_INVALID_VALUE;

        memcpy( param_value, data, size );

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNDRangeKernel( cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim,
                        const size_t* global_work_offset, const size_t* global_work_size,
                        const size_t* local_work_size, cl_uint num_events_in_wait_list,
                        const cl_event* event_wait_list, cl_event* event ) CL_API_SUFFIX__VERSION_1_1
{
    if( (work_dim < 1) || (work_dim > 3) )
    {
        return CL_INVALID_WORK_DIMENSION;
    }

    if( global_work_size == NULL )
    {
        return CL_INVALID_GLOBAL_WORK_SIZE;
    }

    if( ((event_wait_list != NULL) && (num_events_in_wait_list == 0)) ||
        ((event_wait_list != NULL) && (num_events_in_wait_list == 0)) )
    {
        return CL_INVALID_EVENT_WAIT_LIST;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_kernel* kernel_ptr = icd.get_object_ptr< composite_kernel >( kernel );
        composite_command_queue* queue_ptr = icd.get_object_ptr< composite_command_queue >( command_queue );

        ndrange local( work_dim, local_work_size );
        ndrange global( work_dim, global_work_size );
        ndrange offset( work_dim, global_work_offset );

        dcl::events_t events;

        if( event_wait_list != NULL )
        {
            for( uint32_t i = 0; i < num_events_in_wait_list; i++ )
            {
                events.push_back( icd.get_object_ptr< composite_event >( event_wait_list[ i ] ) );
            }
        }

        composite_event* event_ptr = NULL;

        kernel_ptr->execute( queue_ptr, offset, global, local, events,
                             (event != NULL) ? reinterpret_cast<generic_event**>( &event_ptr )
                                             : NULL );

        if( event != NULL )
        {
            *event = icd.get_cl_id< composite_event >( event_ptr );
        }

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_KERNEL;
    }

    // Dummy
    return CL_INVALID_KERNEL;
}
//-----------------------------------------------------------------------------
