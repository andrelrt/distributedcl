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
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_command_queue CL_API_CALL
clCreateCommandQueue( cl_context context, cl_device_id device, 
                      cl_command_queue_properties properties,
                      cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_1
{
    // Dummy
    if( errcode_ret != NULL )
    {
        *errcode_ret = CL_INVALID_VALUE;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clRetainCommandQueue( cl_command_queue command_queue ) CL_API_SUFFIX__VERSION_1_1
//{
//    //FIXME: Not implemented
//    return CL_INVALID_COMMAND_QUEUE;
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clReleaseCommandQueue( cl_command_queue command_queue ) CL_API_SUFFIX__VERSION_1_1
{
    //FIXME: Not implemented
    return CL_INVALID_COMMAND_QUEUE;
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
    //FIXME: Not implemented
    return CL_INVALID_COMMAND_QUEUE;
}
//-----------------------------------------------------------------------------
