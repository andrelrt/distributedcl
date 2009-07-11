/*
 * Copyright (c) 2009 André Tupinambá (andrelrt@gmail.com)
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include <CL/opencl.h>
//-----------------------------------------------------------------------------
#define HEADER "=========================================================================================\n%s\n"
#define FOOTER "\n"

#define DECLARE_LIBRARY_CALL(x) static x##_t g_clLib_##x=NULL
#define LOAD_FUNCTION(x) g_clLib_##x=(x##_t)load_function(#x)
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*logging_fn)(const char *, const void *, size_t, void *);

typedef struct
{
    logging_fn user_callback;
    void* user_data;

} context_userdata;

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetPlatformIDs_t)( cl_uint, cl_platform_id*, cl_uint* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetPlatformInfo_t)( cl_platform_id, cl_platform_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetDeviceIDs_t)( cl_platform_id, cl_device_type, cl_uint, cl_device_id*, cl_uint* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetDeviceInfo_t)( cl_device_id, cl_device_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_context CL_API_CALL
(*clCreateContext_t)( cl_context_properties*, cl_uint, const cl_device_id*, logging_fn, void*, cl_int* );

typedef CL_API_ENTRY cl_context CL_API_CALL
(*clCreateContextFromType_t)( cl_context_properties*, cl_device_type, logging_fn, void*, cl_int* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clRetainContext_t)( cl_context );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clReleaseContext_t)( cl_context );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetContextInfo_t)( cl_context, cl_context_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_command_queue CL_API_CALL
(*clCreateCommandQueue_t)( cl_context, cl_device_id, cl_command_queue_properties, cl_int* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clRetainCommandQueue_t)( cl_command_queue );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clReleaseCommandQueue_t)( cl_command_queue );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetCommandQueueInfo_t)( cl_command_queue, cl_command_queue_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clSetCommandQueueProperty_t)( cl_command_queue, cl_command_queue_properties, cl_bool, cl_command_queue_properties* );

typedef CL_API_ENTRY cl_mem CL_API_CALL
(*clCreateBuffer_t)( cl_context, cl_mem_flags, size_t, void*, cl_int* );

typedef CL_API_ENTRY cl_mem CL_API_CALL
(*clCreateImage2D_t)( cl_context, cl_mem_flags, const cl_image_format*, size_t, size_t, size_t, void*, cl_int* );

typedef CL_API_ENTRY cl_mem CL_API_CALL
(*clCreateImage3D_t)( cl_context, cl_mem_flags, const cl_image_format*, size_t, size_t, size_t, size_t, size_t, void*, cl_int* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clRetainMemObject_t)( cl_mem );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clReleaseMemObject_t)( cl_mem );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetSupportedImageFormats_t)( cl_context, cl_mem_flags, cl_mem_object_type, cl_uint, cl_image_format*, cl_uint* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetMemObjectInfo_t)( cl_mem, cl_mem_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetImageInfo_t)( cl_mem, cl_image_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_sampler CL_API_CALL
(*clCreateSampler_t)( cl_context, cl_bool, cl_addressing_mode, cl_filter_mode, cl_int* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clRetainSampler_t)( cl_sampler );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clReleaseSampler_t)( cl_sampler );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetSamplerInfo_t)( cl_sampler, cl_sampler_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_program CL_API_CALL
(*clCreateProgramWithSource_t)( cl_context, cl_uint, const char**, const size_t*, cl_int* );

typedef CL_API_ENTRY cl_program CL_API_CALL
(*clCreateProgramWithBinary_t)( cl_context, cl_uint, const cl_device_id*, const size_t*, const unsigned char**, cl_int*, cl_int* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clRetainProgram_t)( cl_program );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clReleaseProgram_t)( cl_program );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clBuildProgram_t)( cl_program, cl_uint, const cl_device_id*, const char*, void (*pfn_notify)( cl_program, void* ), void* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clUnloadCompiler_t)( void );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetProgramInfo_t)( cl_program, cl_program_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetProgramBuildInfo_t)( cl_program, cl_device_id, cl_program_build_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_kernel CL_API_CALL
(*clCreateKernel_t)( cl_program, const char*, cl_int* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clCreateKernelsInProgram_t)( cl_program, cl_uint, cl_kernel*, cl_uint* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clRetainKernel_t)( cl_kernel );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clReleaseKernel_t)( cl_kernel );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clSetKernelArg_t)( cl_kernel, cl_uint, size_t, const void* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetKernelInfo_t)( cl_kernel, cl_kernel_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetKernelWorkGroupInfo_t)( cl_kernel, cl_device_id, cl_kernel_work_group_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clWaitForEvents_t)( cl_uint, const cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetEventInfo_t)( cl_event, cl_event_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clRetainEvent_t)( cl_event );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clReleaseEvent_t)( cl_event );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clGetEventProfilingInfo_t)( cl_event, cl_profiling_info, size_t, void*, size_t* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clFlush_t)( cl_command_queue );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clFinish_t)( cl_command_queue );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueReadBuffer_t)( cl_command_queue, cl_mem, cl_bool, size_t, size_t, void*, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueWriteBuffer_t)( cl_command_queue, cl_mem, cl_bool, size_t, size_t, const void*, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueCopyBuffer_t)( cl_command_queue, cl_mem, cl_mem, size_t, size_t, size_t, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueReadImage_t)( cl_command_queue, cl_mem, cl_bool, const size_t*, const size_t*, size_t, size_t, void*, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueWriteImage_t)( cl_command_queue, cl_mem, cl_bool, const size_t*, const size_t*, size_t, size_t, const void*, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueCopyImage_t)( cl_command_queue, cl_mem, cl_mem, const size_t*, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueCopyImageToBuffer_t)( cl_command_queue, cl_mem, cl_mem, const size_t*, const size_t*, size_t, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueCopyBufferToImage_t)( cl_command_queue, cl_mem, cl_mem, size_t, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY void * CL_API_CALL
(*clEnqueueMapBuffer_t)( cl_command_queue, cl_mem, cl_bool, cl_map_flags, size_t, size_t, cl_uint, const cl_event*, cl_event*, cl_int* );

typedef CL_API_ENTRY void * CL_API_CALL
(*clEnqueueMapImage_t)( cl_command_queue, cl_mem, cl_bool, cl_map_flags, const size_t*, const size_t*,
                        size_t*, size_t*, cl_uint, const cl_event*, cl_event*, cl_int* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueUnmapMemObject_t)( cl_command_queue, cl_mem, void*, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueNDRangeKernel_t)( cl_command_queue, cl_kernel, cl_uint, const size_t*, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueTask_t)( cl_command_queue, cl_kernel, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueNativeKernel_t)( cl_command_queue, void (*user_func)(void *), void*, size_t, cl_uint, const cl_mem*, const void**, cl_uint, const cl_event*, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueMarker_t)( cl_command_queue, cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueWaitForEvents_t)( cl_command_queue, cl_uint, const cl_event* );

typedef CL_API_ENTRY cl_int CL_API_CALL
(*clEnqueueBarrier_t)( cl_command_queue );

//-----------------------------------------------------------------------------
static void *g_clLib = NULL;

DECLARE_LIBRARY_CALL(clGetPlatformIDs);
DECLARE_LIBRARY_CALL(clGetPlatformInfo);
DECLARE_LIBRARY_CALL(clGetDeviceIDs);
DECLARE_LIBRARY_CALL(clGetDeviceInfo);
DECLARE_LIBRARY_CALL(clCreateContext);
DECLARE_LIBRARY_CALL(clCreateContextFromType);
DECLARE_LIBRARY_CALL(clRetainContext);
DECLARE_LIBRARY_CALL(clReleaseContext);
DECLARE_LIBRARY_CALL(clGetContextInfo);
DECLARE_LIBRARY_CALL(clCreateCommandQueue);
DECLARE_LIBRARY_CALL(clRetainCommandQueue);
DECLARE_LIBRARY_CALL(clReleaseCommandQueue);
DECLARE_LIBRARY_CALL(clGetCommandQueueInfo);
DECLARE_LIBRARY_CALL(clSetCommandQueueProperty);
DECLARE_LIBRARY_CALL(clCreateBuffer);
DECLARE_LIBRARY_CALL(clCreateImage2D);
DECLARE_LIBRARY_CALL(clCreateImage3D);
DECLARE_LIBRARY_CALL(clRetainMemObject);
DECLARE_LIBRARY_CALL(clReleaseMemObject);
DECLARE_LIBRARY_CALL(clGetSupportedImageFormats);
DECLARE_LIBRARY_CALL(clGetMemObjectInfo);
DECLARE_LIBRARY_CALL(clGetImageInfo);
DECLARE_LIBRARY_CALL(clCreateSampler);
DECLARE_LIBRARY_CALL(clRetainSampler);
DECLARE_LIBRARY_CALL(clReleaseSampler);
DECLARE_LIBRARY_CALL(clGetSamplerInfo);
DECLARE_LIBRARY_CALL(clCreateProgramWithSource);
DECLARE_LIBRARY_CALL(clCreateProgramWithBinary);
DECLARE_LIBRARY_CALL(clRetainProgram);
DECLARE_LIBRARY_CALL(clReleaseProgram);
DECLARE_LIBRARY_CALL(clBuildProgram);
DECLARE_LIBRARY_CALL(clUnloadCompiler);
DECLARE_LIBRARY_CALL(clGetProgramInfo);
DECLARE_LIBRARY_CALL(clGetProgramBuildInfo);
DECLARE_LIBRARY_CALL(clCreateKernel);
DECLARE_LIBRARY_CALL(clCreateKernelsInProgram);
DECLARE_LIBRARY_CALL(clRetainKernel);
DECLARE_LIBRARY_CALL(clReleaseKernel);
DECLARE_LIBRARY_CALL(clSetKernelArg);
DECLARE_LIBRARY_CALL(clGetKernelInfo);
DECLARE_LIBRARY_CALL(clGetKernelWorkGroupInfo);
DECLARE_LIBRARY_CALL(clWaitForEvents);
DECLARE_LIBRARY_CALL(clGetEventInfo);
DECLARE_LIBRARY_CALL(clRetainEvent);
DECLARE_LIBRARY_CALL(clReleaseEvent);
DECLARE_LIBRARY_CALL(clGetEventProfilingInfo);
DECLARE_LIBRARY_CALL(clFlush);
DECLARE_LIBRARY_CALL(clFinish);
DECLARE_LIBRARY_CALL(clEnqueueReadBuffer);
DECLARE_LIBRARY_CALL(clEnqueueWriteBuffer);
DECLARE_LIBRARY_CALL(clEnqueueCopyBuffer);
DECLARE_LIBRARY_CALL(clEnqueueReadImage);
DECLARE_LIBRARY_CALL(clEnqueueWriteImage);
DECLARE_LIBRARY_CALL(clEnqueueCopyImage);
DECLARE_LIBRARY_CALL(clEnqueueCopyImageToBuffer);
DECLARE_LIBRARY_CALL(clEnqueueCopyBufferToImage);
DECLARE_LIBRARY_CALL(clEnqueueMapBuffer);
DECLARE_LIBRARY_CALL(clEnqueueMapImage);
DECLARE_LIBRARY_CALL(clEnqueueUnmapMemObject);
DECLARE_LIBRARY_CALL(clEnqueueNDRangeKernel);
DECLARE_LIBRARY_CALL(clEnqueueTask);
DECLARE_LIBRARY_CALL(clEnqueueNativeKernel);
DECLARE_LIBRARY_CALL(clEnqueueMarker);
DECLARE_LIBRARY_CALL(clEnqueueWaitForEvents);
DECLARE_LIBRARY_CALL(clEnqueueBarrier);

//-----------------------------------------------------------------------------
static void *
load_function( const char* function )
{
    void *ret = dlsym( g_clLib, function );

    const char *error;

    if( (error = dlerror()) != NULL )
    {
        fputs( error, stderr );
        exit( 1 );
    }

    return( ret );
}
//-----------------------------------------------------------------------------
static void
init_logger()
{
    g_clLib = dlopen( "/usr/local/lib/libOpenCL.nvidia.so.1.1", RTLD_LAZY );

    if( g_clLib == NULL )
    {
        fputs( dlerror(), stderr );
        exit( 1 );
    }

    LOAD_FUNCTION(clGetPlatformIDs);
    LOAD_FUNCTION(clGetPlatformInfo);

    LOAD_FUNCTION(clGetDeviceIDs);
    LOAD_FUNCTION(clGetDeviceInfo);

    LOAD_FUNCTION(clCreateContext);
    LOAD_FUNCTION(clCreateContextFromType);
    LOAD_FUNCTION(clRetainContext);
    LOAD_FUNCTION(clReleaseContext);
    LOAD_FUNCTION(clGetContextInfo);

    LOAD_FUNCTION(clCreateCommandQueue);
    LOAD_FUNCTION(clRetainCommandQueue);
    LOAD_FUNCTION(clReleaseCommandQueue);
    LOAD_FUNCTION(clGetCommandQueueInfo);
    LOAD_FUNCTION(clSetCommandQueueProperty);
    LOAD_FUNCTION(clCreateBuffer);
    LOAD_FUNCTION(clCreateImage2D);
    LOAD_FUNCTION(clCreateImage3D);
    LOAD_FUNCTION(clRetainMemObject);
    LOAD_FUNCTION(clReleaseMemObject);
    LOAD_FUNCTION(clGetSupportedImageFormats);
    LOAD_FUNCTION(clGetMemObjectInfo);
    LOAD_FUNCTION(clGetImageInfo);
    LOAD_FUNCTION(clCreateSampler);
    LOAD_FUNCTION(clRetainSampler);
    LOAD_FUNCTION(clReleaseSampler);
    LOAD_FUNCTION(clGetSamplerInfo);
    LOAD_FUNCTION(clCreateProgramWithSource);
    LOAD_FUNCTION(clCreateProgramWithBinary);
    LOAD_FUNCTION(clRetainProgram);
    LOAD_FUNCTION(clReleaseProgram);
    LOAD_FUNCTION(clBuildProgram);
    LOAD_FUNCTION(clUnloadCompiler);
    LOAD_FUNCTION(clGetProgramInfo);
    LOAD_FUNCTION(clGetProgramBuildInfo);
    LOAD_FUNCTION(clCreateKernel);
    LOAD_FUNCTION(clCreateKernelsInProgram);
    LOAD_FUNCTION(clRetainKernel);
    LOAD_FUNCTION(clReleaseKernel);
    LOAD_FUNCTION(clSetKernelArg);
    LOAD_FUNCTION(clGetKernelInfo);
    LOAD_FUNCTION(clGetKernelWorkGroupInfo);
    LOAD_FUNCTION(clWaitForEvents);
    LOAD_FUNCTION(clGetEventInfo);
    LOAD_FUNCTION(clRetainEvent);
    LOAD_FUNCTION(clReleaseEvent);
    LOAD_FUNCTION(clGetEventProfilingInfo);
    LOAD_FUNCTION(clFlush);
    LOAD_FUNCTION(clFinish);
    LOAD_FUNCTION(clEnqueueReadBuffer);
    LOAD_FUNCTION(clEnqueueWriteBuffer);
    LOAD_FUNCTION(clEnqueueCopyBuffer);
    LOAD_FUNCTION(clEnqueueReadImage);
    LOAD_FUNCTION(clEnqueueWriteImage);
    LOAD_FUNCTION(clEnqueueCopyImage);
    LOAD_FUNCTION(clEnqueueCopyImageToBuffer);
    LOAD_FUNCTION(clEnqueueCopyBufferToImage);
    LOAD_FUNCTION(clEnqueueMapBuffer);
    LOAD_FUNCTION(clEnqueueMapImage);
    LOAD_FUNCTION(clEnqueueUnmapMemObject);
    LOAD_FUNCTION(clEnqueueNDRangeKernel);
    LOAD_FUNCTION(clEnqueueTask);
    LOAD_FUNCTION(clEnqueueNativeKernel);
    LOAD_FUNCTION(clEnqueueMarker);
    LOAD_FUNCTION(clEnqueueWaitForEvents);
    LOAD_FUNCTION(clEnqueueBarrier);
}

//-----------------------------------------------------------------------------
void
dump( FILE* fd, cl_uint ident, const unsigned char* data, cl_uint size )
{
    char* str_ident = static_cast<char*>( malloc( ident + 1 ) );
    memset( str_ident, ' ', ident );
    str_ident[ident] = 0;

    cl_uint last_line = size & ~15;
    cl_uint item = 0;
    char ascii[17];
    ascii[16] = 0;
    cl_uint row;
    cl_uint column;

    fprintf( fd, "%sData lentgh: %u bytes\n", str_ident, size );
    for( row = 0; row < last_line; row += 16 )
    {
        fprintf( fd, "%s%08X  ", str_ident, row );

        for( column = 0; column < 16; column ++ )
        {
            fprintf( fd, "%02X ", data[item] );
            ascii[column] = (0x20 <= data[item] && data[item] <= 0x7f)? data[item] : '.';

            item++;
        }

        fprintf( fd, " %s\n", ascii );
    }

    last_line = size & 15;

    if( last_line != 0 )
    {
        fprintf( fd, "%s%08X  ", str_ident, row );

        for( column = 0; column < last_line; column ++ )
        {
            fprintf( fd, "%02X ", data[item] );
            ascii[column] = (0x20 <= data[item] && data[item] <= 0x7f)? data[item] : '.';

            item++;
        }

        ascii[column] = 0;

        for( ; column < 16; column ++ )
        {
            fprintf( fd, "   " );
        }

        fprintf( fd, " %s\n", ascii );
    }

    free( str_ident );
}

//-----------------------------------------------------------------------------
typedef struct
{
    cl_int code;
    const char* string;

} code_to_string_t;

code_to_string_t error_codes[] = {
    { CL_SUCCESS, "CL_SUCCESS" },
    { CL_DEVICE_NOT_FOUND, "CL_DEVICE_NOT_FOUND" },
    { CL_DEVICE_NOT_AVAILABLE, "CL_DEVICE_NOT_AVAILABLE" },
    { CL_DEVICE_COMPILER_NOT_AVAILABLE, "CL_DEVICE_COMPILER_NOT_AVAILABLE" },
    { CL_MEM_OBJECT_ALLOCATION_FAILURE, "CL_MEM_OBJECT_ALLOCATION_FAILURE" },
    { CL_OUT_OF_RESOURCES, "CL_OUT_OF_RESOURCES" },
    { CL_OUT_OF_HOST_MEMORY, "CL_OUT_OF_HOST_MEMORY" },
    { CL_PROFILING_INFO_NOT_AVAILABLE, "CL_PROFILING_INFO_NOT_AVAILABLE" },
    { CL_MEM_COPY_OVERLAP, "CL_MEM_COPY_OVERLAP" },
    { CL_IMAGE_FORMAT_MISMATCH, "CL_IMAGE_FORMAT_MISMATCH" },
    { CL_IMAGE_FORMAT_NOT_SUPPORTED, "CL_IMAGE_FORMAT_NOT_SUPPORTED" },
    { CL_BUILD_PROGRAM_FAILURE, "CL_BUILD_PROGRAM_FAILURE" },
    { CL_MAP_FAILURE, "CL_MAP_FAILURE" },

    { CL_INVALID_VALUE, "CL_INVALID_VALUE" },
    { CL_INVALID_DEVICE_TYPE, "CL_INVALID_DEVICE_TYPE" },
    { CL_INVALID_PLATFORM, "CL_INVALID_PLATFORM" },
    { CL_INVALID_DEVICE, "CL_INVALID_DEVICE" },
    { CL_INVALID_CONTEXT, "CL_INVALID_CONTEXT" },
    { CL_INVALID_QUEUE_PROPERTIES, "CL_INVALID_QUEUE_PROPERTIES" },
    { CL_INVALID_COMMAND_QUEUE, "CL_INVALID_COMMAND_QUEUE" },
    { CL_INVALID_HOST_PTR, "CL_INVALID_HOST_PTR" },
    { CL_INVALID_MEM_OBJECT, "CL_INVALID_MEM_OBJECT" },
    { CL_INVALID_IMAGE_FORMAT_DESCRIPTOR, "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR" },
    { CL_INVALID_IMAGE_SIZE, "CL_INVALID_IMAGE_SIZE" },
    { CL_INVALID_SAMPLER, "CL_INVALID_SAMPLER" },
    { CL_INVALID_BINARY, "CL_INVALID_BINARY" },
    { CL_INVALID_BUILD_OPTIONS, "CL_INVALID_BUILD_OPTIONS" },
    { CL_INVALID_PROGRAM, "CL_INVALID_PROGRAM" },
    { CL_INVALID_PROGRAM_EXECUTABLE, "CL_INVALID_PROGRAM_EXECUTABLE" },
    { CL_INVALID_KERNEL_NAME, "CL_INVALID_KERNEL_NAME" },
    { CL_INVALID_KERNEL_DEFINITION, "CL_INVALID_KERNEL_DEFINITION" },
    { CL_INVALID_KERNEL, "CL_INVALID_KERNEL" },
    { CL_INVALID_ARG_INDEX, "CL_INVALID_ARG_INDEX" },
    { CL_INVALID_ARG_VALUE, "CL_INVALID_ARG_VALUE" },
    { CL_INVALID_ARG_SIZE, "CL_INVALID_ARG_SIZE" },
    { CL_INVALID_KERNEL_ARGS, "CL_INVALID_KERNEL_ARGS" },
    { CL_INVALID_WORK_DIMENSION, "CL_INVALID_WORK_DIMENSION" },
    { CL_INVALID_WORK_GROUP_SIZE, "CL_INVALID_WORK_GROUP_SIZE" },
    { CL_INVALID_WORK_ITEM_SIZE, "CL_INVALID_WORK_ITEM_SIZE" },
    { CL_INVALID_GLOBAL_OFFSET, "CL_INVALID_GLOBAL_OFFSET" },
    { CL_INVALID_EVENT_WAIT_LIST, "CL_INVALID_EVENT_WAIT_LIST" },
    { CL_INVALID_EVENT, "CL_INVALID_EVENT" },
    { CL_INVALID_OPERATION, "CL_INVALID_OPERATION" },
    { CL_INVALID_GL_OBJECT, "CL_INVALID_GL_OBJECT" },
    { CL_INVALID_BUFFER_SIZE, "CL_INVALID_BUFFER_SIZE" },
    { CL_INVALID_MIP_LEVEL, "CL_INVALID_MIP_LEVEL" },
    { 0, NULL } };

code_to_string_t platform_info_codes[] = {
    { CL_PLATFORM_PROFILE, "CL_PLATFORM_PROFILE" },
    { CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION" },
    { CL_PLATFORM_NAME, "CL_PLATFORM_NAME" },
    { CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR" },
    { 0, NULL } };

code_to_string_t device_type_all = { CL_DEVICE_TYPE_ALL, "CL_DEVICE_TYPE_ALL" };
code_to_string_t device_type_codes[] = {
    { CL_DEVICE_TYPE_DEFAULT, "CL_DEVICE_TYPE_DEFAULT" },
    { CL_DEVICE_TYPE_CPU, "CL_DEVICE_TYPE_CPU" },
    { CL_DEVICE_TYPE_GPU, "CL_DEVICE_TYPE_GPU" },
    { CL_DEVICE_TYPE_ACCELERATOR, "CL_DEVICE_TYPE_ACCELERATOR" },
    { 0, NULL } };

code_to_string_t device_info_codes[] = {
	{ CL_DEVICE_TYPE, "CL_DEVICE_TYPE" },
	{ CL_DEVICE_VENDOR_ID, "CL_DEVICE_VENDOR_ID" },
	{ CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_MAX_COMPUTE_UNITS" },
	{ CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS" },
	{ CL_DEVICE_MAX_WORK_GROUP_SIZE, "CL_DEVICE_MAX_WORK_GROUP_SIZE" },
	{ CL_DEVICE_MAX_WORK_ITEM_SIZES, "CL_DEVICE_MAX_WORK_ITEM_SIZES" },
	{ CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR" },
	{ CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT" },
	{ CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT" },
	{ CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG" },
	{ CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT" },
	{ CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE" },
	{ CL_DEVICE_MAX_CLOCK_FREQUENCY, "CL_DEVICE_MAX_CLOCK_FREQUENCY" },
	{ CL_DEVICE_ADDRESS_BITS, "CL_DEVICE_ADDRESS_BITS" },
	{ CL_DEVICE_MAX_READ_IMAGE_ARGS, "CL_DEVICE_MAX_READ_IMAGE_ARGS" },
	{ CL_DEVICE_MAX_WRITE_IMAGE_ARGS, "CL_DEVICE_MAX_WRITE_IMAGE_ARGS" },
	{ CL_DEVICE_MAX_MEM_ALLOC_SIZE, "CL_DEVICE_MAX_MEM_ALLOC_SIZE" },
	{ CL_DEVICE_IMAGE2D_MAX_WIDTH, "CL_DEVICE_IMAGE2D_MAX_WIDTH" },
	{ CL_DEVICE_IMAGE2D_MAX_HEIGHT, "CL_DEVICE_IMAGE2D_MAX_HEIGHT" },
	{ CL_DEVICE_IMAGE3D_MAX_WIDTH, "CL_DEVICE_IMAGE3D_MAX_WIDTH" },
	{ CL_DEVICE_IMAGE3D_MAX_HEIGHT, "CL_DEVICE_IMAGE3D_MAX_HEIGHT" },
	{ CL_DEVICE_IMAGE3D_MAX_DEPTH, "CL_DEVICE_IMAGE3D_MAX_DEPTH" },
	{ CL_DEVICE_IMAGE_SUPPORT, "CL_DEVICE_IMAGE_SUPPORT" },
	{ CL_DEVICE_MAX_PARAMETER_SIZE, "CL_DEVICE_MAX_PARAMETER_SIZE" },
	{ CL_DEVICE_MAX_SAMPLERS, "CL_DEVICE_MAX_SAMPLERS" },
	{ CL_DEVICE_MEM_BASE_ADDR_ALIGN, "CL_DEVICE_MEM_BASE_ADDR_ALIGN" },
	{ CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, "CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE" },
	{ CL_DEVICE_SINGLE_FP_CONFIG, "CL_DEVICE_SINGLE_FP_CONFIG" },
	{ CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, "CL_DEVICE_GLOBAL_MEM_CACHE_TYPE" },
	{ CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE" },
	{ CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, "CL_DEVICE_GLOBAL_MEM_CACHE_SIZE" },
	{ CL_DEVICE_GLOBAL_MEM_SIZE, "CL_DEVICE_GLOBAL_MEM_SIZE" },
	{ CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE" },
	{ CL_DEVICE_MAX_CONSTANT_ARGS, "CL_DEVICE_MAX_CONSTANT_ARGS" },
	{ CL_DEVICE_LOCAL_MEM_TYPE, "CL_DEVICE_LOCAL_MEM_TYPE" },
	{ CL_DEVICE_LOCAL_MEM_SIZE, "CL_DEVICE_LOCAL_MEM_SIZE" },
	{ CL_DEVICE_ERROR_CORRECTION_SUPPORT, "CL_DEVICE_ERROR_CORRECTION_SUPPORT" },
	{ CL_DEVICE_PROFILING_TIMER_RESOLUTION, "CL_DEVICE_PROFILING_TIMER_RESOLUTION" },
	{ CL_DEVICE_ENDIAN_LITTLE, "CL_DEVICE_ENDIAN_LITTLE" },
	{ CL_DEVICE_AVAILABLE, "CL_DEVICE_AVAILABLE" },
	{ CL_DEVICE_COMPILER_AVAILABLE, "CL_DEVICE_COMPILER_AVAILABLE" },
	{ CL_DEVICE_EXECUTION_CAPABILITIES, "CL_DEVICE_EXECUTION_CAPABILITIES" },
	{ CL_DEVICE_QUEUE_PROPERTIES, "CL_DEVICE_QUEUE_PROPERTIES" },
	{ CL_DEVICE_NAME, "CL_DEVICE_NAME" },
	{ CL_DEVICE_VENDOR, "CL_DEVICE_VENDOR" },
	{ CL_DRIVER_VERSION, "CL_DRIVER_VERSION" },
	{ CL_DEVICE_PROFILE, "CL_DEVICE_PROFILE" },
	{ CL_DEVICE_VERSION, "CL_DEVICE_VERSION" },
	{ CL_DEVICE_EXTENSIONS, "CL_DEVICE_EXTENSIONS" },
	{ CL_DEVICE_PLATFORM, "CL_DEVICE_PLATFORM" },
    { 0, NULL } };

code_to_string_t device_address_info_codes[] = {
    { CL_DEVICE_ADDRESS_32_BITS, "CL_DEVICE_ADDRESS_32_BITS" },
    { CL_DEVICE_ADDRESS_64_BITS, "CL_DEVICE_ADDRESS_64_BITS" },
    { 0, NULL } };

code_to_string_t device_fp_config_codes[] = {
    { CL_FP_DENORM, "CL_FP_DENORM" },
    { CL_FP_INF_NAN, "CL_FP_INF_NAN" },
    { CL_FP_ROUND_TO_NEAREST, "CL_FP_ROUND_TO_NEAREST" },
    { CL_FP_ROUND_TO_ZERO, "CL_FP_ROUND_TO_ZERO" },
    { CL_FP_ROUND_TO_INF, "CL_FP_ROUND_TO_INF" },
    { CL_FP_FMA, "CL_FP_FMA" },
    { 0, NULL } };

code_to_string_t device_mem_cache_type_codes[] = {
    { CL_NONE, "CL_NONE" },
    { CL_READ_ONLY_CACHE, "CL_READ_ONLY_CACHE" },
    { CL_READ_WRITE_CACHE, "CL_READ_WRITE_CACHE" },
    { 0, NULL } };

code_to_string_t device_local_mem_type_codes[] = {
    { CL_LOCAL, "CL_LOCAL" },
    { CL_GLOBAL, "CL_GLOBAL" },
    { 0, NULL } };

code_to_string_t device_exec_capabilities_codes[] = {
    { CL_EXEC_KERNEL, "CL_EXEC_KERNEL" },
    { CL_EXEC_NATIVE_KERNEL, "CL_EXEC_NATIVE_KERNEL" },
    { 0, NULL } };

code_to_string_t command_queue_properties_codes[] = {
    { CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE" },
    { CL_QUEUE_PROFILING_ENABLE, "CL_QUEUE_PROFILING_ENABLE" },
    { 0, NULL } };

code_to_string_t command_queue_info_codes[] = {
    { CL_QUEUE_CONTEXT, "CL_QUEUE_CONTEXT" },
    { CL_QUEUE_DEVICE, "CL_QUEUE_DEVICE" },
    { CL_QUEUE_REFERENCE_COUNT, "CL_QUEUE_REFERENCE_COUNT" },
    { CL_QUEUE_PROPERTIES, "CL_QUEUE_PROPERTIES" },
    { 0, NULL } };

code_to_string_t context_info_codes[] = {
    { CL_CONTEXT_REFERENCE_COUNT, "CL_CONTEXT_REFERENCE_COUNT" },
    { CL_CONTEXT_NUM_DEVICES, "CL_CONTEXT_NUM_DEVICES" },
    { CL_CONTEXT_DEVICES, "CL_CONTEXT_DEVICES" },
    { CL_CONTEXT_PROPERTIES, "CL_CONTEXT_PROPERTIES" },
    { CL_CONTEXT_PLATFORM, "CL_CONTEXT_PLATFORM" },
    { 0, NULL } };

code_to_string_t mem_flags_codes[] = {
    { CL_MEM_READ_WRITE, "CL_MEM_READ_WRITE" },
    { CL_MEM_WRITE_ONLY, "CL_MEM_WRITE_ONLY" },
    { CL_MEM_READ_ONLY, "CL_MEM_READ_ONLY" },
    { CL_MEM_USE_HOST_PTR, "CL_MEM_USE_HOST_PTR" },
    { CL_MEM_ALLOC_HOST_PTR, "CL_MEM_ALLOC_HOST_PTR" },
    { CL_MEM_COPY_HOST_PTR, "CL_MEM_COPY_HOST_PTR" },
    { 0, NULL } };

code_to_string_t channel_order_codes[] = {
    { CL_R, "CL_R" },
    { CL_A, "CL_A" },
    { CL_RG, "CL_RG" },
    { CL_RA, "CL_RA" },
    { CL_RGB, "CL_RGB" },
    { CL_RGBA, "CL_RGBA" },
    { CL_BGRA, "CL_BGRA" },
    { CL_ARGB, "CL_ARGB" },
    { CL_INTENSITY, "CL_INTENSITY" },
    { CL_LUMINANCE, "CL_LUMINANCE" },
    { 0, NULL } };

code_to_string_t channel_type_codes[] = {
    { CL_SNORM_INT8, "CL_SNORM_INT8" },
    { CL_SNORM_INT16, "CL_SNORM_INT16" },
    { CL_UNORM_INT8, "CL_UNORM_INT8" },
    { CL_UNORM_INT16, "CL_UNORM_INT16" },
    { CL_UNORM_SHORT_565, "CL_UNORM_SHORT_565" },
    { CL_UNORM_SHORT_555, "CL_UNORM_SHORT_555" },
    { CL_UNORM_INT_101010, "CL_UNORM_INT_101010" },
    { CL_SIGNED_INT8, "CL_SIGNED_INT8" },
    { CL_SIGNED_INT16, "CL_SIGNED_INT16" },
    { CL_SIGNED_INT32, "CL_SIGNED_INT32" },
    { CL_UNSIGNED_INT8, "CL_UNSIGNED_INT8" },
    { CL_UNSIGNED_INT16, "CL_UNSIGNED_INT16" },
    { CL_UNSIGNED_INT32, "CL_UNSIGNED_INT32" },
    { CL_HALF_FLOAT, "CL_HALF_FLOAT" },
    { CL_FLOAT, "CL_FLOAT" },
    { 0, NULL } };

code_to_string_t mem_object_type_codes[] = {
    { CL_MEM_OBJECT_BUFFER, "CL_MEM_OBJECT_BUFFER" },
    { CL_MEM_OBJECT_IMAGE2D, "CL_MEM_OBJECT_IMAGE2D" },
    { CL_MEM_OBJECT_IMAGE3D, "CL_MEM_OBJECT_IMAGE3D" },
    { 0, NULL } };

code_to_string_t mem_info_codes[] = {
    { CL_MEM_TYPE, "CL_MEM_TYPE" },
    { CL_MEM_FLAGS, "CL_MEM_FLAGS" },
    { CL_MEM_SIZE, "CL_MEM_SIZE" },
    { CL_MEM_HOST_PTR, "CL_MEM_HOST_PTR" },
    { CL_MEM_MAP_COUNT, "CL_MEM_MAP_COUNT" },
    { CL_MEM_REFERENCE_COUNT, "CL_MEM_REFERENCE_COUNT" },
    { CL_MEM_CONTEXT, "CL_MEM_CONTEXT" },
    { 0, NULL } };

code_to_string_t image_info_codes[] = {
    { CL_IMAGE_FORMAT, "CL_IMAGE_FORMAT" },
    { CL_IMAGE_ELEMENT_SIZE, "CL_IMAGE_ELEMENT_SIZE" },
    { CL_IMAGE_ROW_PITCH, "CL_IMAGE_ROW_PITCH" },
    { CL_IMAGE_SLICE_PITCH, "CL_IMAGE_SLICE_PITCH" },
    { CL_IMAGE_WIDTH, "CL_IMAGE_WIDTH" },
    { CL_IMAGE_HEIGHT, "CL_IMAGE_HEIGHT" },
    { CL_IMAGE_DEPTH, "CL_IMAGE_DEPTH" },
    { 0, NULL } };

code_to_string_t addressing_mode_codes[] = {
    { CL_ADDRESS_NONE, "CL_ADDRESS_NONE" },
    { CL_ADDRESS_CLAMP_TO_EDGE, "CL_ADDRESS_CLAMP_TO_EDGE" },
    { CL_ADDRESS_CLAMP, "CL_ADDRESS_CLAMP" },
    { CL_ADDRESS_REPEAT, "CL_ADDRESS_REPEAT" },
    { 0, NULL } };

code_to_string_t filter_mode_codes[] = {
    { CL_FILTER_NEAREST, "CL_FILTER_NEAREST" },
    { CL_FILTER_LINEAR, "CL_FILTER_LINEAR" },
    { 0, NULL } };

code_to_string_t sampler_info_codes[] = {
    { CL_SAMPLER_REFERENCE_COUNT, "CL_SAMPLER_REFERENCE_COUNT" },
    { CL_SAMPLER_CONTEXT, "CL_SAMPLER_CONTEXT" },
    { CL_SAMPLER_NORMALIZED_COORDS, "CL_SAMPLER_NORMALIZED_COORDS" },
    { CL_SAMPLER_ADDRESSING_MODE, "CL_SAMPLER_ADDRESSING_MODE" },
    { CL_SAMPLER_FILTER_MODE, "CL_SAMPLER_FILTER_MODE" },
    { 0, NULL } };

code_to_string_t map_flags_codes[] = {
    { CL_MAP_READ, "CL_MAP_READ" },
    { CL_MAP_WRITE, "CL_MAP_WRITE" },
    { 0, NULL } };

code_to_string_t program_info_codes[] = {
    { CL_PROGRAM_REFERENCE_COUNT, "CL_PROGRAM_REFERENCE_COUNT" },
    { CL_PROGRAM_CONTEXT, "CL_PROGRAM_CONTEXT" },
    { CL_PROGRAM_NUM_DEVICES, "CL_PROGRAM_NUM_DEVICES" },
    { CL_PROGRAM_DEVICES, "CL_PROGRAM_DEVICES" },
    { CL_PROGRAM_SOURCE, "CL_PROGRAM_SOURCE" },
    { CL_PROGRAM_BINARY_SIZES, "CL_PROGRAM_BINARY_SIZES" },
    { CL_PROGRAM_BINARIES, "CL_PROGRAM_BINARIES" },
    { 0, NULL } };

code_to_string_t program_build_info_codes[] = {
    { CL_PROGRAM_BUILD_STATUS, "CL_PROGRAM_BUILD_STATUS" },
    { CL_PROGRAM_BUILD_OPTIONS, "CL_PROGRAM_BUILD_OPTIONS" },
    { CL_PROGRAM_BUILD_LOG, "CL_PROGRAM_BUILD_LOG" },
    { 0, NULL } };

code_to_string_t build_status_codes[] = {
    { CL_BUILD_SUCCESS, "CL_BUILD_SUCCESS" },
    { CL_BUILD_NONE, "CL_BUILD_NONE" },
    { CL_BUILD_ERROR, "CL_BUILD_ERROR" },
    { CL_BUILD_IN_PROGRESS, "CL_BUILD_IN_PROGRESS" },
    { 0, NULL } };

code_to_string_t kernel_info_codes[] = {
    { CL_KERNEL_FUNCTION_NAME, "CL_KERNEL_FUNCTION_NAME" },
    { CL_KERNEL_NUM_ARGS, "CL_KERNEL_NUM_ARGS" },
    { CL_KERNEL_REFERENCE_COUNT, "CL_KERNEL_REFERENCE_COUNT" },
    { CL_KERNEL_CONTEXT, "CL_KERNEL_CONTEXT" },
    { CL_KERNEL_PROGRAM, "CL_KERNEL_PROGRAM" },
    { 0, NULL } };

code_to_string_t kernel_work_group_info_codes[] = {
    { CL_KERNEL_WORK_GROUP_SIZE, "CL_KERNEL_WORK_GROUP_SIZE" },
    { CL_KERNEL_COMPILE_WORK_GROUP_SIZE, "CL_KERNEL_COMPILE_WORK_GROUP_SIZE" },
    { CL_KERNEL_LOCAL_MEM_SIZE, "CL_KERNEL_LOCAL_MEM_SIZE" },
    { 0, NULL } };

code_to_string_t event_info_codes[] = {
    { CL_EVENT_COMMAND_QUEUE, "CL_EVENT_COMMAND_QUEUE" },
    { CL_EVENT_COMMAND_TYPE, "CL_EVENT_COMMAND_TYPE" },
    { CL_EVENT_REFERENCE_COUNT, "CL_EVENT_REFERENCE_COUNT" },
    { CL_EVENT_COMMAND_EXECUTION_STATUS, "CL_EVENT_COMMAND_EXECUTION_STATUS" },
    { 0, NULL } };

code_to_string_t command_type_codes[] = {
    { CL_COMMAND_NDRANGE_KERNEL, "CL_COMMAND_NDRANGE_KERNEL" },
    { CL_COMMAND_TASK, "CL_COMMAND_TASK" },
    { CL_COMMAND_NATIVE_KERNEL, "CL_COMMAND_NATIVE_KERNEL" },
    { CL_COMMAND_READ_BUFFER, "CL_COMMAND_READ_BUFFER" },
    { CL_COMMAND_WRITE_BUFFER, "CL_COMMAND_WRITE_BUFFER" },
    { CL_COMMAND_COPY_BUFFER, "CL_COMMAND_COPY_BUFFER" },
    { CL_COMMAND_READ_IMAGE, "CL_COMMAND_READ_IMAGE" },
    { CL_COMMAND_WRITE_IMAGE, "CL_COMMAND_WRITE_IMAGE" },
    { CL_COMMAND_COPY_IMAGE, "CL_COMMAND_COPY_IMAGE" },
    { CL_COMMAND_COPY_IMAGE_TO_BUFFER, "CL_COMMAND_COPY_IMAGE_TO_BUFFER" },
    { CL_COMMAND_COPY_BUFFER_TO_IMAGE, "CL_COMMAND_COPY_BUFFER_TO_IMAGE" },
    { CL_COMMAND_MAP_BUFFER, "CL_COMMAND_MAP_BUFFER" },
    { CL_COMMAND_MAP_IMAGE, "CL_COMMAND_MAP_IMAGE" },
    { CL_COMMAND_UNMAP_MEM_OBJECT, "CL_COMMAND_UNMAP_MEM_OBJECT" },
    { CL_COMMAND_MARKER, "CL_COMMAND_MARKER" },
    { CL_COMMAND_WAIT_FOR_EVENTS, "CL_COMMAND_WAIT_FOR_EVENTS" },
    { CL_COMMAND_BARRIER, "CL_COMMAND_BARRIER" },
    { CL_COMMAND_ACQUIRE_GL_OBJECTS, "CL_COMMAND_ACQUIRE_GL_OBJECTS" },
    { CL_COMMAND_RELEASE_GL_OBJECTS, "CL_COMMAND_RELEASE_GL_OBJECTS" },
    { 0, NULL } };

code_to_string_t command_execution_status_codes[] = {
    { CL_COMPLETE, "CL_COMPLETE" },
    { CL_RUNNING, "CL_RUNNING" },
    { CL_SUBMITTED, "CL_SUBMITTED" },
    { CL_QUEUED, "CL_QUEUED" },
    { 0, NULL } };

code_to_string_t profiling_info_codes[] = {
    { CL_PROFILING_COMMAND_QUEUED, "CL_PROFILING_COMMAND_QUEUED" },
    { CL_PROFILING_COMMAND_SUBMIT, "CL_PROFILING_COMMAND_SUBMIT" },
    { CL_PROFILING_COMMAND_START, "CL_PROFILING_COMMAND_START" },
    { CL_PROFILING_COMMAND_END, "CL_PROFILING_COMMAND_END" },
    { 0, NULL } };

#define bool_to_string(x) ((x)?"CT_TRUE":"CT_FALSE")
#define error_to_string(x) code_to_string((x),error_codes,"Unknow error")
#define platform_info_to_string(x) code_to_string((x),platform_info_codes,"Unknow platform info")
#define device_type_to_string(x) bitfield_to_string((x),device_type_codes,&device_type_all,"Unknow device type")
#define device_info_to_string(x) code_to_string((x),device_info_codes,"Unknow device info")
#define context_info_to_string(x) code_to_string((x),context_info_codes,"Unknow context info")
#define command_queue_properties_to_string(x) bitfield_to_string((x),command_queue_properties_codes,NULL,"Unknow command queue properties")
#define command_queue_info_to_string(x) code_to_string((x),command_queue_info_codes,"Unknow command queue info")
#define mem_flags_to_string(x) bitfield_to_string((x),mem_flags_codes,NULL,"Unknow mem flags")
#define channel_order_to_string(x) code_to_string((x),channel_order_codes,"Unknow channel order")
#define channel_type_to_string(x) code_to_string((x),channel_type_codes,"Unknow channel type")
#define mem_object_type_to_string(x) code_to_string((x),mem_object_type_codes,"Unknow mem object type")
#define mem_info_to_string(x) code_to_string((x),mem_info_codes,"Unknow mem info")
#define image_info_to_string(x) code_to_string((x),image_info_codes,"Unknow image info")
#define addressing_mode_to_string(x) code_to_string((x),addressing_mode_codes,"Unknow addressing mode")
#define filter_mode_to_string(x) code_to_string((x),filter_mode_codes,"Unknow filter mode")
#define sampler_info_to_string(x) code_to_string((x),sampler_info_codes,"Unknow sampler info")
#define map_flags_to_string(x) bitfield_to_string((x),map_flags_codes,NULL,"Unknow map flags")
#define program_info_to_string(x) code_to_string((x),program_info_codes,"Unknow program info")
#define program_build_to_string(x) code_to_string((x),program_build_codes,"Unknow program build")
#define program_build_info_to_string(x) code_to_string((x),program_build_info_codes,"Unknow program build info")
#define build_status_to_string(x) code_to_string((x),build_status_codes,"Unknow build status")
#define kernel_info_to_string(x) code_to_string((x),kernel_info_codes,"Unknow kernel info")
#define kernel_work_group_info_to_string(x) code_to_string((x),kernel_work_group_info_codes,"Unknow kernel work group info")
#define event_info_to_string(x) code_to_string((x),event_info_codes,"Unknow event info")
#define command_type_to_string(x) code_to_string((x),command_type_codes,"Unknow command type")
#define command_execution_status_to_string(x) code_to_string((x),command_execution_status_codes,"Unknow command execution status")
#define profiling_info_to_string(x) code_to_string((x),profiling_info_codes,"Unknow profiling info")
#define device_address_info_to_string(x) bitfield_to_string((x),device_address_info_codes,NULL,"Unknow device address info")
#define device_fp_config_to_string(x) bitfield_to_string((x),device_fp_config_codes,NULL,"Unknow device fp config")
#define device_mem_cache_type_to_string(x) code_to_string((x),device_mem_cache_type_codes,"Unknow device mem cache type")
#define device_local_mem_type_to_string(x) code_to_string((x),device_local_mem_type_codes,"Unknow device local mem type")
#define device_exec_capabilities_to_string(x) bitfield_to_string((x),device_exec_capabilities_codes,NULL,"Unknow device exec capabilities")


static const char*
code_to_string( cl_int code, code_to_string_t* code_table, const char* default_string )
{
    code_to_string_t* code_item = code_table;

    while( code_item->string != NULL )
    {
        if( code_item->code == code )
            return code_item->string;

        code_item++;
    }

    return( default_string );
}

static const char*
bitfield_to_string( cl_int bitfield, code_to_string_t* code_table, code_to_string_t* all, const char* default_string )
{
    if( (all != NULL) &&
        (bitfield == all->code) )
    {
        return( all->string );
    }

    code_to_string_t* code_item = code_table;
    static char ret[2048];
    ret[0] = 0;

    while( code_item->string != NULL )
    {
        if( code_item->code & bitfield )
        {
            if( ret[0] != 0 )
                strcat( ret, ", " );

            strcat( ret, code_item->string );
        }

        code_item++;
    }

    return( (ret[0] != 0)? ret : default_string );
}

//-----------------------------------------------------------------------------
// Platform API
//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetPlatformIDs( cl_uint         num_entries,
                  cl_platform_id* platforms,
                  cl_uint*        num_platforms ) CL_API_SUFFIX__VERSION_1_0
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    printf( "  num_entries: %u\n", num_entries );

	ret = g_clLib_clGetPlatformIDs( num_entries, platforms, num_platforms );

	if( ret == CL_SUCCESS )
	{
		if( platforms == NULL )
		{
			printf( "  platforms: NULL\n" );
		}
		else
		{
			printf( "  platforms:" );
			if( num_platforms != NULL )
			{
				for( cl_uint i = 0; i < *num_platforms; i++ )
				{
					printf( " %p", platforms[i] );
				}
			}
			printf( "\n" );
		}
		
		if( num_platforms == NULL )
		{
			printf( "  num_platforms: NULL\n" );
		}
		else
		{
			printf( "  num_platforms: %u\n", *num_platforms );
		}
	}

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

CL_API_ENTRY cl_int CL_API_CALL
clGetPlatformInfo( cl_platform_id   platform, 
                   cl_platform_info param_name,
                   size_t           param_value_size,
                   void*            param_value,
                   size_t*          param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    printf( "  platform: %p\n", platform );
    printf( "  param_name: %s\n", platform_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetPlatformInfo( platform, param_name, param_value_size,
                                     param_value, param_value_size_ret );

    printf( "  param_value: %s\n", (param_value == NULL) ? "NULL"
                                                         : static_cast<char*>( param_value ) );

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Device APIs
//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetDeviceIDs( cl_platform_id platform,
                cl_device_type device_type,
                cl_uint        num_entries,
                cl_device_id*  devices,
                cl_uint*       num_devices )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    printf( "  platform: %p\n", platform );
    printf( "  device_type: %s\n", device_type_to_string( device_type ) );
    printf( "  num_entries: %u\n", num_entries );

    ret = g_clLib_clGetDeviceIDs( platform, device_type, num_entries, devices, num_devices );

    if( devices == NULL )
        printf( "  devices: NULL\n" );
    else
    {
        printf( "  devices:\n" );

        if( ret == CL_SUCCESS )
	    {
			if( num_devices != NULL )
	        {
	            for( cl_uint i = 0; i < *num_devices; i++ )
	            {
	                printf( "    [%d]: %p\n", i, devices[i] );
	            }
	        }
	    }
    }

    if( num_devices == NULL )
        printf( "  num_devices: NULL\n" );
    else
        printf( "  num_devices: %u\n", *num_devices );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetDeviceInfo( cl_device_id   device,
                 cl_device_info param_name,
                 size_t         param_value_size,
                 void*          param_value,
                 size_t*        param_value_size_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( device == NULL )
        printf( "  device: NULL\n" );
    else
        printf( "  device: %p\n", device );

    ret = g_clLib_clGetDeviceInfo( device, param_name, param_value_size,
                                   param_value, param_value_size_ret );


    printf( "  param_name: %s\n", device_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        switch( param_name )
        {
            case CL_DEVICE_TYPE:
                printf( "  param_value: %s\n", device_type_to_string( *(static_cast<cl_device_type*>( param_value )) ) );
                break;

            case CL_DEVICE_MAX_WORK_ITEM_SIZES:
                printf( "  param_value: %lu x %lu x %lu\n", (static_cast<size_t*>( param_value ))[0],
                                                            (static_cast<size_t*>( param_value ))[1],
                                                            (static_cast<size_t*>( param_value ))[2] );
                break;

            case CL_DEVICE_MAX_MEM_ALLOC_SIZE:
            case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE:
            case CL_DEVICE_GLOBAL_MEM_SIZE:
            case CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:
            case CL_DEVICE_LOCAL_MEM_SIZE:
                printf( "  param_value: %lu\n", *(static_cast<cl_ulong*>( param_value )) );
                break;

            case CL_DEVICE_IMAGE_SUPPORT:
            case CL_DEVICE_ERROR_CORRECTION_SUPPORT:
            case CL_DEVICE_ENDIAN_LITTLE:
            case CL_DEVICE_AVAILABLE:
            case CL_DEVICE_COMPILER_AVAILABLE:
                printf( "  param_value: %s\n", bool_to_string( *(static_cast<cl_bool*>( param_value )) ) );
                break;

            case CL_DEVICE_SINGLE_FP_CONFIG:
                printf( "  param_value: %s\n", device_fp_config_to_string( *(static_cast<cl_device_fp_config*>( param_value )) ) );
                break;

            case CL_DEVICE_VENDOR_ID:
            case CL_DEVICE_MAX_COMPUTE_UNITS:
            case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:
            case CL_DEVICE_MAX_WORK_GROUP_SIZE:
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR:
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT:
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT:
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG:
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT:
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE:
            case CL_DEVICE_MAX_CLOCK_FREQUENCY:
            case CL_DEVICE_ADDRESS_BITS:
            case CL_DEVICE_MAX_READ_IMAGE_ARGS:
            case CL_DEVICE_MAX_WRITE_IMAGE_ARGS:
            case CL_DEVICE_IMAGE2D_MAX_WIDTH:
            case CL_DEVICE_IMAGE2D_MAX_HEIGHT:
            case CL_DEVICE_IMAGE3D_MAX_WIDTH:
            case CL_DEVICE_IMAGE3D_MAX_HEIGHT:
            case CL_DEVICE_IMAGE3D_MAX_DEPTH:
            case CL_DEVICE_MAX_SAMPLERS:
            case CL_DEVICE_MAX_PARAMETER_SIZE:
            case CL_DEVICE_MEM_BASE_ADDR_ALIGN:
            case CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE:
            case CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE:
            case CL_DEVICE_MAX_CONSTANT_ARGS:
            case CL_DEVICE_PROFILING_TIMER_RESOLUTION:
                printf( "  param_value: %u\n", *(static_cast<cl_uint*>( param_value )) );
                break;

            case CL_DEVICE_GLOBAL_MEM_CACHE_TYPE:
                printf( "  param_value: %s\n", device_mem_cache_type_to_string( *(static_cast<cl_device_mem_cache_type*>( param_value )) ) );
                break;

            case CL_DEVICE_LOCAL_MEM_TYPE:
                printf( "  param_value: %s\n", device_local_mem_type_to_string( *(static_cast<cl_device_local_mem_type*>( param_value )) ) );
                break;

            case CL_DEVICE_EXECUTION_CAPABILITIES:
                printf( "  param_value: %s\n", device_exec_capabilities_to_string( *(static_cast<cl_device_exec_capabilities*>( param_value )) ) );
                break;

            case CL_DEVICE_QUEUE_PROPERTIES:
                printf( "  param_value: %s\n", command_queue_properties_to_string( *(static_cast<cl_command_queue_properties*>( param_value )) ) );
                break;

            case CL_DEVICE_NAME:
            case CL_DEVICE_VENDOR:
            case CL_DRIVER_VERSION:
            case CL_DEVICE_PROFILE:
            case CL_DEVICE_VERSION:
            case CL_DEVICE_EXTENSIONS:
                printf( "  param_value: %s\n", static_cast<char*>( param_value ) );
                break;
                
            case CL_DEVICE_PLATFORM:
            	printf( "  param_value: %p\n", *(static_cast<cl_platform_id*>( param_value )) );

            default:
                printf( "  param_name: Unknow device info\n" );
                break;
            }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Context APIs
//-----------------------------------------------------------------------------
void
context_callback( const char* errinfo,
                  const void* private_info,
                  size_t      cb,
                  void* user_data )
{
    printf( "  Callback:\n" );
    printf( "    errinfo: %s\n", errinfo );
    printf( "    private_info:\n" );
    
    dump( stdout, 6, static_cast<const unsigned char*>( private_info ), cb );

    context_userdata* data = (context_userdata*) user_data;

    if( data->user_callback != NULL )
        data->user_callback( errinfo, private_info, cb, data->user_data );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_context CL_API_CALL
clCreateContext( cl_context_properties * properties,
                 cl_uint                 num_devices,
                 const cl_device_id *    devices,
                 logging_fn              pfn_notify,
                 void *                  user_data,
                 cl_int *                errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_context ret;

    if( properties == NULL )
        printf( "  properties: NULL\n" );
    else
        printf( "  properties: %p\n", properties );

    printf( "  num_devices: %u\n", num_devices );

    if( devices == NULL )
        printf( "  devices: NULL\n" );
    else
    {
        printf( "  devices (%p): ", devices );

        for( cl_uint i = 0; i < num_devices; i++ )
            printf( "%p ", devices[i] );

        printf( "\n" );
    }

    if( pfn_notify == NULL )
        printf( "  pfn_notify: NULL\n" );
    else
        printf( "  pfn_notify: %p\n", pfn_notify );

    if( user_data == NULL )
        printf( "  user_data: NULL\n" );
    else
        printf( "  user_data: %p\n", user_data );

    context_userdata ctx_data;

    ctx_data.user_callback = pfn_notify;
    ctx_data.user_data = user_data;

    ret = g_clLib_clCreateContext( properties, num_devices, devices,
                                   context_callback, &ctx_data, errcode_ret );

    printf( "  context: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_context CL_API_CALL
clCreateContextFromType( cl_context_properties * properties,
                         cl_device_type          device_type,
                         logging_fn              pfn_notify,
                         void *                  user_data,
                         cl_int *                errcode_ret)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_context ret;

    if( properties == NULL )
        printf( "  properties: NULL\n" );
    else
        printf( "  properties: %p\n", properties );

    printf( "  device_type: %s\n", device_type_to_string( device_type ) );

    if( pfn_notify == NULL )
        printf( "  pfn_notify: NULL\n" );
    else
        printf( "  pfn_notify: %p\n", pfn_notify );

    if( user_data == NULL )
        printf( "  user_data: NULL\n" );
    else
        printf( "  user_data: %p\n", user_data );

    context_userdata ctx_data;

    ctx_data.user_callback = pfn_notify;
    ctx_data.user_data = user_data;

    ret = g_clLib_clCreateContextFromType( properties, device_type,
                                           context_callback, &ctx_data, errcode_ret );

    printf( "  context: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clRetainContext(cl_context context)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    ret = g_clLib_clRetainContext( context );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clReleaseContext(cl_context context)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    ret = g_clLib_clReleaseContext( context );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetContextInfo(cl_context         context,
                 cl_context_info    param_name,
                 size_t             param_value_size,
                 void *             param_value,
                 size_t *           param_value_size_ret)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    printf( "  param_name: %s\n", context_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetContextInfo( context, param_name, param_value_size,
                                    param_value, param_value_size_ret );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        switch( param_name )
        {
            case CL_CONTEXT_REFERENCE_COUNT:
                printf( "  param_value: %u\n", *(static_cast<cl_uint*>( param_value )) );
                break;

            case CL_CONTEXT_DEVICES:
                printf( "  param_value:" );

                if( param_value_size && param_value_size_ret != NULL )
                {
                    cl_uint num_devices = (*param_value_size_ret) / sizeof(cl_device_id);

                    for( cl_uint i = 0; i < num_devices; i++ )
                        printf( " %p", (static_cast<cl_device_id*>( param_value ))[i] );
                }

                printf( "\n" );
                break;

            case CL_CONTEXT_PLATFORM:
            	printf( "  param_value: %p\n", *(static_cast<cl_platform_id*>( param_value )) );
            	break;

            case CL_CONTEXT_PROPERTIES:
            default:
                printf( "  param_value: %p\n", param_value );
                break;
        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Command Queue APIs
//-----------------------------------------------------------------------------
CL_API_ENTRY cl_command_queue CL_API_CALL
clCreateCommandQueue( cl_context                  context,
                      cl_device_id                device,
                      cl_command_queue_properties properties,
                      cl_int *                    errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_command_queue ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    if( device == NULL )
        printf( "  device: NULL\n" );
    else
        printf( "  device: %p\n", device );

    printf( "  properties: %s\n", command_queue_properties_to_string( properties ) );

    ret = g_clLib_clCreateCommandQueue( context, device, properties, errcode_ret );

    printf( "  command_queue: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clRetainCommandQueue(cl_command_queue command_queue)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    ret = g_clLib_clRetainCommandQueue( command_queue );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clReleaseCommandQueue(cl_command_queue command_queue)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    ret = g_clLib_clReleaseCommandQueue( command_queue );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetCommandQueueInfo(cl_command_queue      command_queue,
                      cl_command_queue_info param_name,
                      size_t                param_value_size,
                      void *                param_value,
                      size_t *              param_value_size_ret)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  param_name: %s\n", command_queue_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetCommandQueueInfo( command_queue, param_name,
                                         param_value_size, param_value,
                                         param_value_size_ret );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        switch( param_name )
        {
            case CL_QUEUE_CONTEXT:
                printf( "  param_value: %p\n", *(static_cast<cl_context*>( param_value )) );
                break;

            case CL_QUEUE_DEVICE:
                printf( "  param_value: %p\n", *(static_cast<cl_device_id*>( param_value )) );
                break;

            case CL_QUEUE_REFERENCE_COUNT:
                printf( "  param_value: %u\n", *(static_cast<cl_uint*>( param_value )) );
                break;

            case CL_QUEUE_PROPERTIES:
                printf( "  param_value: %s\n", command_queue_properties_to_string( *(static_cast<cl_command_queue_properties*>( param_value )) ) );
                break;

            default:
                printf( "  param_value: Unknow (%p)\n", param_value );
                break;
        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clSetCommandQueueProperty(cl_command_queue              command_queue,
                          cl_command_queue_properties   properties,
                          cl_bool                       enable,
                          cl_command_queue_properties * old_properties)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  properties: %s\n", command_queue_properties_to_string( properties ) );
    printf( "  enable: %s\n", bool_to_string( enable ) );

    ret = g_clLib_clSetCommandQueueProperty( command_queue, properties,
                                             enable, old_properties );

    if( old_properties == NULL )
        printf( "  old_properties: NULL\n" );
    else
        printf( "  old_properties: %s\n", command_queue_properties_to_string( *old_properties ) );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Memory Object APIs
//-----------------------------------------------------------------------------
CL_API_ENTRY cl_mem CL_API_CALL
clCreateBuffer( cl_context   context,
                cl_mem_flags flags,
                size_t       size,
                void *       host_ptr,
                cl_int *     errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_mem ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    printf( "  flags: %s\n", mem_flags_to_string( flags ) );
    printf( "  size: %lu\n", size );

    if( host_ptr == NULL )
        printf( "  host_ptr: NULL\n" );
    else
        printf( "  host_ptr: %p\n", host_ptr );

    ret = g_clLib_clCreateBuffer( context, flags, size, host_ptr, errcode_ret );

    if( ret == NULL )
        printf( "  Returned buffer object: NULL\n" );
    else
        printf( "  Returned buffer object: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_mem CL_API_CALL
clCreateImage2D( cl_context              context,
                 cl_mem_flags            flags,
                 const cl_image_format * image_format,
                 size_t                  image_width,
                 size_t                  image_height,
                 size_t                  image_row_pitch,
                 void *                  host_ptr,
                 cl_int *                errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_mem ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    printf( "  flags: %s\n", mem_flags_to_string( flags ) );

    if( image_format == NULL )
        printf( "  image_format: NULL\n" );
    else
    {
        printf( "  image_format:\n" );
        printf( "    image_channel_order: %s\n", channel_order_to_string( image_format->image_channel_order ) );
        printf( "    image_channel_data_type: %s\n", channel_type_to_string( image_format->image_channel_data_type ) );
    }

    printf( "  image_width: %lu\n", image_width );
    printf( "  image_height: %lu\n", image_height );
    printf( "  image_row_pitch: %lu\n", image_row_pitch );

    if( host_ptr == NULL )
        printf( "  host_ptr: NULL\n" );
    else
        printf( "  host_ptr: %p\n", host_ptr );

    ret = g_clLib_clCreateImage2D( context, flags, image_format, image_width,
                                   image_height, image_row_pitch, host_ptr, errcode_ret );

    if( ret == NULL )
        printf( "  Returned image object: NULL\n" );
    else
        printf( "  Returned image object: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_mem CL_API_CALL
clCreateImage3D( cl_context              context,
                 cl_mem_flags            flags,
                 const cl_image_format * image_format,
                 size_t                  image_width,
                 size_t                  image_height,
                 size_t                  image_depth,
                 size_t                  image_row_pitch,
                 size_t                  image_slice_pitch,
                 void *                  host_ptr,
                 cl_int *                errcode_ret)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_mem ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    printf( "  flags: %s\n", mem_flags_to_string( flags ) );

    if( image_format == NULL )
        printf( "  image_format: NULL\n" );
    else
    {
        printf( "  image_format:\n" );
        printf( "    image_channel_order: %s\n", channel_order_to_string( image_format->image_channel_order ) );
        printf( "    image_channel_data_type: %s\n", channel_type_to_string( image_format->image_channel_data_type ) );
    }

    printf( "  image_width: %lu\n", image_width );
    printf( "  image_height: %lu\n", image_height );
    printf( "  image_depth: %lu\n", image_depth );
    printf( "  image_row_pitch: %lu\n", image_row_pitch );
    printf( "  image_slice_pitch: %lu\n", image_slice_pitch );

    if( host_ptr == NULL )
        printf( "  host_ptr: NULL\n" );
    else
        printf( "  host_ptr: %p\n", host_ptr );

    ret = g_clLib_clCreateImage3D( context, flags, image_format, image_width,
                                   image_height, image_depth, image_row_pitch,
                                   image_slice_pitch, host_ptr, errcode_ret );

    if( ret == NULL )
        printf( "  Returned image object: NULL\n" );
    else
        printf( "  Returned image object: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clRetainMemObject( cl_mem memobj )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( memobj == NULL )
        printf( "  memobj: NULL\n" );
    else
        printf( "  memobj: %p\n", memobj );

    ret = g_clLib_clRetainMemObject( memobj );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clReleaseMemObject( cl_mem memobj )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( memobj == NULL )
        printf( "  memobj: NULL\n" );
    else
        printf( "  memobj: %p\n", memobj );

    ret = g_clLib_clReleaseMemObject( memobj );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetSupportedImageFormats(cl_context           context,
                           cl_mem_flags         flags,
                           cl_mem_object_type   image_type,
                           cl_uint              num_entries,
                           cl_image_format *    image_formats,
                           cl_uint *            num_image_formats)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    printf( "  flags: %s\n", mem_flags_to_string( flags ) );
    printf( "  image_type: %s\n", mem_object_type_to_string( image_type ) );
    printf( "  num_entries: %u\n", num_entries );

    ret = g_clLib_clGetSupportedImageFormats( context, flags, image_type, num_entries,
                                              image_formats, num_image_formats );

    if( image_formats == NULL )
        printf( "  image_formats: NULL\n" );
    else
    {
        if( num_image_formats != NULL )
        {
            for( cl_uint i = 0; i < *num_image_formats; i++ )
            {
                printf( "    [%u]: %s - %s\n", i,
                        channel_order_to_string( image_formats[i].image_channel_order ),
                        channel_type_to_string( image_formats[i].image_channel_data_type ) );
            }
        }
    }

    if( num_image_formats == NULL )
        printf( "  num_image_formats: NULL\n" );
    else
        printf( "  num_image_formats: %u\n", *num_image_formats );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetMemObjectInfo(cl_mem           memobj,
                   cl_mem_info      param_name,
                   size_t           param_value_size,
                   void *           param_value,
                   size_t *         param_value_size_ret)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( memobj == NULL )
        printf( "  memobj: NULL\n" );
    else
        printf( "  memobj: %p\n", memobj );

    printf( "  param_name: %s\n", mem_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetMemObjectInfo( memobj, param_name, param_value_size,
                                      param_value, param_value_size_ret );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        switch( param_name )
        {
            case CL_MEM_TYPE:
                printf( "  param_value: %s\n",
                        mem_object_type_to_string( *(static_cast<cl_mem_object_type*>( param_value )) ) );
                break;

            case CL_MEM_FLAGS:
                printf( "  param_value: %s\n",
                        mem_flags_to_string( *(static_cast<cl_mem_flags*>( param_value )) ) );
                break;

            case CL_MEM_SIZE:
                printf( "  param_value: %lu\n", *(static_cast<size_t*>( param_value )) );
                break;

            case CL_MEM_HOST_PTR:
                printf( "  param_value: %p\n", *(static_cast<void**>( param_value )) );
                break;

            case CL_MEM_MAP_COUNT:
            case CL_MEM_REFERENCE_COUNT:
                printf( "  param_value: %u\n", *(static_cast<cl_uint*>( param_value )) );
                break;

            case CL_MEM_CONTEXT:
                printf( "  param_value: %p\n", *(static_cast<cl_context*>( param_value )) );
                break;
        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetImageInfo(cl_mem           image,
               cl_image_info    param_name,
               size_t           param_value_size,
               void *           param_value,
               size_t *         param_value_size_ret)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( image == NULL )
        printf( "  image: NULL\n" );
    else
        printf( "  image: %p\n", image );

    printf( "  param_name: %s\n", image_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetImageInfo( image, param_name, param_value_size,
                                  param_value, param_value_size_ret );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        switch( param_name )
        {
            case CL_IMAGE_FORMAT:
                printf( "  param_value: %s - %s\n",
                        channel_order_to_string( (static_cast<cl_image_format*>( param_value ))->image_channel_order ),
                        channel_type_to_string( (static_cast<cl_image_format*>( param_value ))->image_channel_data_type ) );
                break;

            case CL_IMAGE_ELEMENT_SIZE:
            case CL_IMAGE_ROW_PITCH:
            case CL_IMAGE_SLICE_PITCH:
            case CL_IMAGE_WIDTH:
            case CL_IMAGE_HEIGHT:
            case CL_IMAGE_DEPTH:
                printf( "  param_value: %lu\n", *(static_cast<size_t*>( param_value )) );
                break;
        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Sampler APIs
//-----------------------------------------------------------------------------
CL_API_ENTRY cl_sampler CL_API_CALL
clCreateSampler( cl_context          context,
                 cl_bool             normalized_coords,
                 cl_addressing_mode  addressing_mode,
                 cl_filter_mode      filter_mode,
                 cl_int *            errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_sampler ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    printf( "  normalized_coords: %s\n", bool_to_string( normalized_coords ) );
    printf( "  addressing_mode: %s\n", addressing_mode_to_string( addressing_mode ) );
    printf( "  filter_mode: %s\n", filter_mode_to_string( filter_mode ) );

    ret = g_clLib_clCreateSampler( context, normalized_coords, addressing_mode,
                                   filter_mode, errcode_ret );

    if( ret == NULL )
        printf( "  Returned sampler: NULL\n" );
    else
        printf( "  Returned sampler: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clRetainSampler( cl_sampler sampler )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( sampler == NULL )
        printf( "  sampler: NULL\n" );
    else
        printf( "  sampler: %p\n", sampler );

    ret = g_clLib_clRetainSampler( sampler );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clReleaseSampler( cl_sampler sampler )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( sampler == NULL )
        printf( "  sampler: NULL\n" );
    else
        printf( "  sampler: %p\n", sampler );

    ret = g_clLib_clReleaseSampler( sampler );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetSamplerInfo( cl_sampler         sampler,
                  cl_sampler_info    param_name,
                  size_t             param_value_size,
                  void *             param_value,
                  size_t *           param_value_size_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( sampler == NULL )
        printf( "  sampler: NULL\n" );
    else
        printf( "  sampler: %p\n", sampler );

    printf( "  param_name: %s\n", sampler_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetSamplerInfo( sampler, param_name, param_value_size,
                                    param_value, param_value_size_ret );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        switch( param_name )
        {
            case CL_SAMPLER_REFERENCE_COUNT:
                printf( "  param_value: %u\n", *(static_cast<cl_uint*>( param_value )) );
                break;

            case CL_SAMPLER_CONTEXT:
                printf( "  param_value: %p\n", *(static_cast<cl_context*>( param_value )) );
                break;

            case CL_SAMPLER_ADDRESSING_MODE:
                printf( "  param_value: %s\n",
                        addressing_mode_to_string( *(static_cast<cl_uint*>( param_value )) ) );
                break;

            case CL_SAMPLER_FILTER_MODE:
                printf( "  param_value: %s\n",
                        filter_mode_to_string( *(static_cast<cl_uint*>( param_value )) ) );
                break;

            case CL_SAMPLER_NORMALIZED_COORDS:
                printf( "  param_value: %s\n",
                        bool_to_string( *(static_cast<cl_uint*>( param_value )) ) );
                break;
        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Program Object APIs
//-----------------------------------------------------------------------------
CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithSource( cl_context        context,
                           cl_uint           count,
                           const char **     strings,
                           const size_t *    lengths,
                           cl_int *          errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_program ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    printf( "  count: %u\n", count );

    if( strings == NULL )
        printf( "  strings: NULL\n" );
    else
    {
        printf( "  strings:\n" );

        for( cl_uint i = 0; i < count; i++ )
        {
            if( (lengths != NULL) && (lengths[i] != 0) )
            {
                char* str = static_cast<char*>( malloc( lengths[i] +1 ) );
                strncpy( str, strings[i], lengths[i] );

                printf( "    [%u]: %s\n", i, str );

                free( str );
            }
            else
                printf( "    [%u]: %s\n", i, strings[i] );
        }
    }

    if( lengths == NULL )
        printf( "  lengths: NULL\n" );
    else
    {
        printf( "  lengths:\n" );

        for( cl_uint i = 0; i < count; i++ )
        {
            printf( "    [%u]: %lu\n", i, lengths[i] );
        }
    }

    ret = g_clLib_clCreateProgramWithSource( context, count, strings,
                                             lengths, errcode_ret );

    if( ret == NULL )
        printf( "  Returned program: NULL\n" );
    else
        printf( "  Returned program: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBinary( cl_context            context,
                           cl_uint               num_devices,
                           const cl_device_id*   device_list,
                           const size_t*         lengths,
                           const unsigned char** binaries,
                           cl_int *              binary_status,
                           cl_int *              errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_program ret;

    if( context == NULL )
        printf( "  context: NULL\n" );
    else
        printf( "  context: %p\n", context );

    printf( "  num_devices: %u\n", num_devices );

    if( device_list == NULL )
        printf( "  device_list: NULL\n" );
    else
    {
        printf( "  device_list:\n" );

        for( cl_uint i = 0; i < num_devices; i++ )
        {
            printf( "    [%u]: %p\n", i, device_list[i] );
        }
    }

    if( lengths == NULL )
        printf( "  lengths: NULL\n" );
    else
    {
        printf( "  lengths:\n" );

        for( cl_uint i = 0; i < num_devices; i++ )
        {
            printf( "    [%u]: %lu\n", i, lengths[i] );
        }
    }

    if( binaries == NULL )
        printf( "  binaries: NULL\n" );
    else
    {
        printf( "  binaries:\n" );

        for( cl_uint i = 0; i < num_devices; i++ )
        {
            printf( "    [%u]:\n", i );

            dump( stdout, 6, static_cast<const unsigned char*>( binaries[i] ), lengths[i] );
        }
    }

    ret = g_clLib_clCreateProgramWithBinary( context, num_devices, device_list,
                                             lengths, binaries, binary_status,
                                             errcode_ret );

    if( binary_status == NULL )
        printf( "  binary_status: NULL\n" );
    else
    {
        printf( "  binary_status:\n" );
        for( cl_uint i = 0; i < num_devices; i++ )
        {
            printf( "    [%u]: %s\n", i, error_to_string( binary_status[i] ) );
        }
    }

    if( ret == NULL )
        printf( "  Returned program: NULL\n" );
    else
        printf( "  Returned program: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clRetainProgram( cl_program program )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( program == NULL )
        printf( "  program: NULL\n" );
    else
        printf( "  program: %p\n", program );

    ret = g_clLib_clRetainProgram( program );
    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clReleaseProgram( cl_program program )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( program == NULL )
        printf( "  program: NULL\n" );
    else
        printf( "  program: %p\n", program );

    ret = g_clLib_clReleaseProgram( program );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clBuildProgram( cl_program           program,
                cl_uint              num_devices,
                const cl_device_id * device_list,
                const char *         options,
                void (*pfn_notify)(cl_program program, void * user_data),
                void *               user_data )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( program == NULL )
        printf( "  program: NULL\n" );
    else
        printf( "  program: %p\n", program );

    printf( "  num_devices: %u\n", num_devices );

    if( device_list == NULL )
        printf( "  device_list: NULL\n" );
    else
    {
        printf( "  device_list:\n" );

        for( cl_uint i = 0; i < num_devices; i++ )
        {
            printf( "    [%u]: %p\n", i, device_list[i] );
        }
    }

    if( options == NULL )
        printf( "  options: NULL\n" );
    else
        printf( "  options: %s\n", options );

    if( pfn_notify == NULL )
        printf( "  pfn_notify: NULL\n" );
    else
        printf( "  pfn_notify: %p\n", pfn_notify );

    if( user_data == NULL )
        printf( "  user_data: NULL\n" );
    else
        printf( "  user_data: %p\n", user_data );

    ret = g_clLib_clBuildProgram( program, num_devices, device_list,
                                  options, pfn_notify, user_data );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clUnloadCompiler( void )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    ret = g_clLib_clUnloadCompiler();

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetProgramInfo( cl_program         program,
                  cl_program_info    param_name,
                  size_t             param_value_size,
                  void *             param_value,
                  size_t *           param_value_size_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( program == NULL )
        printf( "  program: NULL\n" );
    else
        printf( "  program: %p\n", program );

    printf( "  param_name: %s\n", program_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetProgramInfo( program, param_name, param_value_size,
                                    param_value, param_value_size_ret );
    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        if( ret == CL_SUCCESS )
        {
            switch( param_name )
            {
                case CL_PROGRAM_REFERENCE_COUNT:
                case CL_PROGRAM_NUM_DEVICES:
                    printf( "  param_name: %u\n", *(static_cast<cl_uint*>( param_value )) );
                    break;

                case CL_PROGRAM_CONTEXT:
                    printf( "  param_name: %p\n", *(static_cast<cl_context*>( param_value )) );
                    break;

                case CL_PROGRAM_DEVICES:
                    if( param_value_size_ret == NULL )
                        printf( "  param_name: Uncountable devices\n" );
                    else
                    {
                        printf( "  param_name:\n" );

                        cl_uint num_devices = *param_value_size_ret / sizeof( cl_device_id );

                        for( cl_uint i = 0; i < num_devices; i++ )
                            printf( "    [%u]: %p\n", i, (static_cast<cl_device_id*>( param_value ))[ i ] );

                        break;
                    }

                case CL_PROGRAM_SOURCE:
                    printf( "  param_name: %s\n", static_cast<char*>( param_value ) );
                    break;

                case CL_PROGRAM_BINARY_SIZES:
                    if( param_value_size_ret == NULL )
                        printf( "  param_name: Uncountable binaries sizes\n" );
                    else
                    {
                        printf( "  param_name:\n" );

                        cl_uint num_devices = *param_value_size_ret / sizeof( size_t );

                        for( cl_uint i = 0; i < num_devices; i++ )
                            printf( "    [%u]: %lu\n", i, (static_cast<size_t*>( param_value ))[ i ] );

                        break;
                    }
                    break;

                case CL_PROGRAM_BINARIES:
                    printf( "  param_name: Uncountable binaries\n" );
                    break;

                default:
                    printf( "  param_name: Unknow parameter\n" );
                    break;
            }
        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetProgramBuildInfo(cl_program            program,
                      cl_device_id          device,
                      cl_program_build_info param_name,
                      size_t                param_value_size,
                      void *                param_value,
                      size_t *              param_value_size_ret)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( program == NULL )
        printf( "  program: NULL\n" );
    else
        printf( "  program: %p\n", program );

    if( device == NULL )
        printf( "  device: NULL\n" );
    else
        printf( "  device: %p\n", device );

    printf( "  param_name: %s\n", program_build_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetProgramBuildInfo( program, device, param_name,
                                         param_value_size, param_value,
                                         param_value_size_ret );
    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        if( ret == CL_SUCCESS )
        {
            switch( param_name )
            {
                case CL_PROGRAM_BUILD_STATUS:
                    printf( "  param_value: %s\n",
                            build_status_to_string( *(static_cast<cl_build_status*>( param_value )) ) );
                    break;

                case CL_PROGRAM_BUILD_OPTIONS:
                case CL_PROGRAM_BUILD_LOG:
                    printf( "  param_value: %s\n", static_cast<char*>( param_value ) );
                    break;
            }
        }
    }

    printf( "  Return Code: %s\n", error_to_string( ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Kernel Object APIs
//-----------------------------------------------------------------------------
CL_API_ENTRY cl_kernel CL_API_CALL
clCreateKernel( cl_program      program,
                const char *    kernel_name,
                cl_int *        errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_kernel ret;

    if( program == NULL )
        printf( "  program: NULL\n" );
    else
        printf( "  program: %p\n", program );

    if( kernel_name == NULL )
        printf( "  kernel_name: NULL\n" );
    else
        printf( "  kernel_name: %s\n", kernel_name );

    ret = g_clLib_clCreateKernel( program, kernel_name, errcode_ret );

    if( ret == NULL )
        printf( "  Returned kernel: NULL\n" );
    else
        printf( "  Returned kernel: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clCreateKernelsInProgram( cl_program     program,
                          cl_uint        num_kernels,
                          cl_kernel *    kernels,
                          cl_uint *      num_kernels_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( program == NULL )
        printf( "  program: NULL\n" );
    else
        printf( "  program: %p\n", program );

    printf( "  num_kernels: %u\n", num_kernels );

    ret = g_clLib_clCreateKernelsInProgram( program, num_kernels, kernels, num_kernels_ret );

    if( kernels == NULL )
        printf( "  kernels: NULL\n" );
    else
        if( ret == CL_SUCCESS )
        {
            {
                printf( "  kernels:" );

                for( cl_uint i=0; i<num_kernels; i++ )
                {
                    printf( " %p", kernels[i] );
                }

                printf( "\n" );
            }
        }

    if( num_kernels_ret == NULL )
        printf( "  num_kernels_ret: NULL\n" );
    else
        printf( "  num_kernels_ret: %u\n", *num_kernels_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clRetainKernel( cl_kernel kernel )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( kernel == NULL )
        printf( "  kernel: NULL\n" );
    else
        printf( "  kernel: %p\n", kernel );

    ret = g_clLib_clRetainKernel( kernel );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clReleaseKernel( cl_kernel kernel )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( kernel == NULL )
        printf( "  kernel: NULL\n" );
    else
        printf( "  kernel: %p\n", kernel );

    ret = g_clLib_clReleaseKernel( kernel );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clSetKernelArg( cl_kernel    kernel,
                cl_uint      arg_index,
                size_t       arg_size,
                const void * arg_value )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( kernel == NULL )
        printf( "  kernel: NULL\n" );
    else
        printf( "  kernel: %p\n", kernel );

    printf( "  arg_index: %u\n", arg_index );
    printf( "  arg_size: %lu\n", arg_size );

    if( arg_value == NULL )
        printf( "  arg_value: NULL\n" );
    else
    {
        printf( "  arg_value:\n" );
        dump( stdout, 4, static_cast<const unsigned char*>( arg_value ), arg_size );
    }

    ret = g_clLib_clSetKernelArg( kernel, arg_index, arg_size, arg_value );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetKernelInfo(cl_kernel       kernel,
                cl_kernel_info  param_name,
                size_t          param_value_size,
                void *          param_value,
                size_t *        param_value_size_ret)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( kernel == NULL )
        printf( "  kernel: NULL\n" );
    else
        printf( "  kernel: %p\n", kernel );

    printf( "  param_name: %s\n", kernel_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetKernelInfo( kernel, param_name, param_value_size,
                                   param_value, param_value_size_ret );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        if( ret == CL_SUCCESS )
        {
            switch( param_name )
            {
                case CL_KERNEL_FUNCTION_NAME:
                    printf( "  param_value: %s\n", static_cast<char*>( param_value ) );
                    break;

                case CL_KERNEL_NUM_ARGS:
                case CL_KERNEL_REFERENCE_COUNT:
                    printf( "  param_value: %u\n", *(static_cast<cl_uint*>( param_value )) );
                    break;

                case CL_KERNEL_CONTEXT:
                    printf( "  param_value: %p\n", *(static_cast<cl_context*>( param_value )) );
                    break;

                case CL_KERNEL_PROGRAM:
                    printf( "  param_value: %p\n", *(static_cast<cl_program*>( param_value )) );
                    break;
            }
        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
CL_API_ENTRY cl_int CL_API_CALL
clGetKernelWorkGroupInfo(cl_kernel                  kernel,
                         cl_device_id               device,
                         cl_kernel_work_group_info  param_name,
                         size_t                     param_value_size,
                         void *                     param_value,
                         size_t *                   param_value_size_ret)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( kernel == NULL )
        printf( "  kernel: NULL\n" );
    else
        printf( "  kernel: %p\n", kernel );

    printf( "  device: %p\n", device );
    printf( "  param_name: %s\n", kernel_work_group_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetKernelWorkGroupInfo( kernel, device, param_name,
                                            param_value_size, param_value,
                                            param_value_size_ret );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        if( ret == CL_SUCCESS )
        {
            switch( param_name )
            {
                case CL_KERNEL_WORK_GROUP_SIZE:
                    printf( "  param_value: %lu\n", *(static_cast<size_t*>( param_value )) );
                    break;

                case CL_KERNEL_COMPILE_WORK_GROUP_SIZE:
                    printf( "  param_value: (%lu, %lu, %lu)\n",
                            (static_cast<size_t*>( param_value ))[0],
                            (static_cast<size_t*>( param_value ))[1],
                            (static_cast<size_t*>( param_value ))[2] );
                    break;
            }
        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Event Object APIs
//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clWaitForEvents( cl_uint           num_events,
                 const cl_event*   event_list )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    printf( "  num_events: %u\n", num_events );

    if( event_list == NULL )
        printf( "  event_list: NULL\n" );
    else
    {
        printf( "  event_list:" );

        for( cl_uint i = 0; i < num_events; i++ )
        {
            printf( " %p", event_list[i] );
        }
        printf( "\n" );
    }

    ret = g_clLib_clWaitForEvents( num_events, event_list );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clGetEventInfo( cl_event         event,
                cl_event_info    param_name,
                size_t           param_value_size,
                void *           param_value,
                size_t *         param_value_size_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", event );

    printf( "  param_name: %s\n", event_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetEventInfo( event, param_name, param_value_size,
                                  param_value, param_value_size_ret );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        if( ret == CL_SUCCESS )
        {
            switch( param_name )
            {
                case CL_EVENT_COMMAND_QUEUE:
                    printf( "  param_value: %p\n", *(static_cast<cl_command_queue*>( param_value )) );
                    break;

                case CL_EVENT_COMMAND_TYPE:
                    printf( "  param_value: %s\n", command_type_to_string( *(static_cast<cl_command_type*>( param_value )) ) );
                    break;

                case CL_EVENT_COMMAND_EXECUTION_STATUS:
                    printf( "  param_value: %d\n", *(static_cast<cl_int*>( param_value )) );
                    break;

                case CL_EVENT_REFERENCE_COUNT:
                    printf( "  param_value: %u\n", *(static_cast<cl_uint*>( param_value )) );
                    break;
            }
        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clRetainEvent( cl_event event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", event );

    ret = g_clLib_clRetainEvent( event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clReleaseEvent( cl_event event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", event );

    ret = g_clLib_clReleaseEvent( event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Profiling APIs
//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clGetEventProfilingInfo( cl_event            event,
                         cl_profiling_info   param_name,
                         size_t              param_value_size,
                         void *              param_value,
                         size_t *            param_value_size_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", event );

    printf( "  param_name: %s\n", profiling_info_to_string( param_name ) );
    printf( "  param_value_size: %lu\n", param_value_size );

    ret = g_clLib_clGetEventProfilingInfo( event, param_name, param_value_size,
                                           param_value, param_value_size_ret );

    if( param_value == NULL )
        printf( "  param_value: NULL\n" );
    else
    {
        if( ret == CL_SUCCESS )
        {
            switch( param_name )
            {
                case CL_PROFILING_COMMAND_QUEUED:
                case CL_PROFILING_COMMAND_SUBMIT:
                case CL_PROFILING_COMMAND_START:
                case CL_PROFILING_COMMAND_END:
                    printf( "  param_value: %lu\n", *(static_cast<cl_ulong*>( param_value )) );
                    break;

                default:
                    printf( "  param_value: Unknow Profiling Info\n" );
            }

        }
    }

    if( param_value_size_ret == NULL )
        printf( "  param_value_size_ret: NULL\n" );
    else
        printf( "  param_value_size_ret: %lu\n", *param_value_size_ret );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Flush and Finish APIs
//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clFlush( cl_command_queue command_queue )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    ret = g_clLib_clFlush( command_queue );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clFinish( cl_command_queue command_queue )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    ret = g_clLib_clFinish( command_queue );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
// Enqueued Commands APIs
//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueReadBuffer( cl_command_queue    command_queue,
                     cl_mem              buffer,
                     cl_bool             blocking_read,
                     size_t              offset,
                     size_t              cb,
                     void *              ptr,
                     cl_uint             num_events_in_wait_list,
                     const cl_event *    event_wait_list,
                     cl_event *          event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    if( buffer == NULL )
        printf( "  buffer: NULL\n" );
    else
        printf( "  buffer: %p\n", buffer );

    printf( "  blocking_read: %s\n", bool_to_string( blocking_read ) );
    printf( "  offset: %lu\n", offset );
    printf( "  cb: %lu\n", cb );
    printf( "  ptr: %p\n", ptr );
    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueReadBuffer( command_queue, buffer, blocking_read,
                                       offset, cb, ptr, num_events_in_wait_list,
                                       event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWriteBuffer( cl_command_queue   command_queue,
                      cl_mem             buffer,
                      cl_bool            blocking_write,
                      size_t             offset,
                      size_t             cb,
                      const void *       ptr,
                      cl_uint            num_events_in_wait_list,
                      const cl_event *   event_wait_list,
                      cl_event *         event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    if( buffer == NULL )
        printf( "  buffer: NULL\n" );
    else
        printf( "  buffer: %p\n", buffer );

    printf( "  blocking_write: %s\n", bool_to_string( blocking_write ) );
    printf( "  offset: %lu\n", offset );
    printf( "  cb: %lu\n", cb );
    printf( "  ptr: %p\n", ptr );
    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueWriteBuffer( command_queue, buffer, blocking_write,
                                        offset, cb, ptr, num_events_in_wait_list,
                                        event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyBuffer(cl_command_queue    command_queue,
                    cl_mem              src_buffer,
                    cl_mem              dst_buffer,
                    size_t              src_offset,
                    size_t              dst_offset,
                    size_t              cb,
                    cl_uint             num_events_in_wait_list,
                    const cl_event *    event_wait_list,
                    cl_event *          event)
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  src_buffer: %p\n", src_buffer );
    printf( "  dst_buffer: %p\n", dst_buffer );
    printf( "  src_offset: %lu\n", src_offset );
    printf( "  dst_offset: %lu\n", dst_offset );
    printf( "  cb: %lu\n", cb );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueCopyBuffer( command_queue, src_buffer, dst_buffer,
                                       src_offset, dst_offset, cb, num_events_in_wait_list,
                                       event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueReadImage( cl_command_queue     command_queue,
                    cl_mem               image,
                    cl_bool              blocking_read,
                    const size_t *       origin,
                    const size_t *       region,
                    size_t               row_pitch,
                    size_t               slice_pitch,
                    void *               ptr,
                    cl_uint              num_events_in_wait_list,
                    const cl_event *     event_wait_list,
                    cl_event *           event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  image: %p\n", image );
    printf( "  blocking_read: %s\n", bool_to_string( blocking_read ) );

    if( origin == NULL )
        printf( "  origin: NULL\n" );
    else
        printf( "  origin: (%lu, %lu, %lu)\n", origin[0], origin[1], origin[2] );

    if( region == NULL )
        printf( "  region: NULL\n" );
    else
        printf( "  region: (%lu, %lu, %lu)\n", region[0], region[1], region[2] );

    printf( "  row_pitch: %lu\n", row_pitch );
    printf( "  slice_pitch:  %lu\n", slice_pitch );
    printf( "  ptr: %p\n", ptr );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueReadImage( command_queue, image, blocking_read,
                                      origin, region, row_pitch, slice_pitch, ptr,
                                      num_events_in_wait_list, event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWriteImage( cl_command_queue    command_queue,
                     cl_mem              image,
                     cl_bool             blocking_write,
                     const size_t *      origin,
                     const size_t *      region,
                     size_t              input_row_pitch,
                     size_t              input_slice_pitch,
                     const void *        ptr,
                     cl_uint             num_events_in_wait_list,
                     const cl_event *    event_wait_list,
                     cl_event *          event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  image: %p\n", image );
    printf( "  blocking_write: %s\n", bool_to_string( blocking_write ) );

    if( origin == NULL )
        printf( "  origin: NULL\n" );
    else
        printf( "  origin: (%lu, %lu, %lu)\n", origin[0], origin[1], origin[2] );

    if( region == NULL )
        printf( "  region: NULL\n" );
    else
        printf( "  region: (%lu, %lu, %lu)\n", region[0], region[1], region[2] );

    printf( "  input_row_pitch: %lu\n", input_row_pitch );
    printf( "  input_slice_pitch:  %lu\n", input_slice_pitch );
    printf( "  ptr: %p\n", ptr );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueWriteImage( command_queue, image, blocking_write, origin,
                                       region, input_row_pitch, input_slice_pitch, ptr,
                                       num_events_in_wait_list, event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyImage( cl_command_queue     command_queue,
                    cl_mem               src_image,
                    cl_mem               dst_image,
                    const size_t *       src_origin,
                    const size_t *       dst_origin,
                    const size_t *       region,
                    cl_uint              num_events_in_wait_list,
                    const cl_event *     event_wait_list,
                    cl_event *           event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  src_image: %p\n", src_image );
    printf( "  dst_image: %p\n", dst_image );

    if( src_origin == NULL )
        printf( "  src_origin: NULL\n" );
    else
        printf( "  src_origin: (%lu, %lu, %lu)\n", src_origin[0], src_origin[1], src_origin[2] );

    if( dst_origin == NULL )
        printf( "  dst_origin: NULL\n" );
    else
        printf( "  dst_origin: (%lu, %lu, %lu)\n", dst_origin[0], dst_origin[1], dst_origin[2] );

    if( region == NULL )
        printf( "  region: NULL\n" );
    else
        printf( "  region: (%lu, %lu, %lu)\n", region[0], region[1], region[2] );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueCopyImage( command_queue, src_image, dst_image, src_origin,
                                      dst_origin, region, num_events_in_wait_list,
                                      event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyImageToBuffer( cl_command_queue command_queue,
                            cl_mem           src_image,
                            cl_mem           dst_buffer,
                            const size_t *   src_origin,
                            const size_t *   region,
                            size_t           dst_offset,
                            cl_uint          num_events_in_wait_list,
                            const cl_event * event_wait_list,
                            cl_event *       event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  src_image: %p\n", src_image );
    printf( "  dst_buffer: %p\n", dst_buffer );

    if( src_origin == NULL )
        printf( "  src_origin: NULL\n" );
    else
        printf( "  src_origin: (%lu, %lu, %lu)\n", src_origin[0], src_origin[1], src_origin[2] );

    if( region == NULL )
        printf( "  region: NULL\n" );
    else
        printf( "  region: (%lu, %lu, %lu)\n", region[0], region[1], region[2] );

    printf( "  dst_offset: %lu\n", dst_offset );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueCopyImageToBuffer( command_queue, src_image, dst_buffer, src_origin, region,
                                              dst_offset, num_events_in_wait_list, event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyBufferToImage( cl_command_queue command_queue,
                            cl_mem           src_buffer,
                            cl_mem           dst_image,
                            size_t           src_offset,
                            const size_t *   dst_origin,
                            const size_t *   region,
                            cl_uint          num_events_in_wait_list,
                            const cl_event * event_wait_list,
                            cl_event *       event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  src_buffer: %p\n", src_buffer );
    printf( "  dst_image: %p\n", dst_image );
    printf( "  src_offset: %lu\n", src_offset );

    if( dst_origin == NULL )
        printf( "  dst_origin: NULL\n" );
    else
        printf( "  dst_origin: (%lu, %lu, %lu)\n", dst_origin[0], dst_origin[1], dst_origin[2] );

    if( region == NULL )
        printf( "  region: NULL\n" );
    else
        printf( "  region: (%lu, %lu, %lu)\n", region[0], region[1], region[2] );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueCopyBufferToImage( command_queue, src_buffer, dst_image, src_offset, dst_origin,
                                              region, num_events_in_wait_list, event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY void * CL_API_CALL
clEnqueueMapBuffer( cl_command_queue command_queue,
                    cl_mem           buffer,
                    cl_bool          blocking_map,
                    cl_map_flags     map_flags,
                    size_t           offset,
                    size_t           cb,
                    cl_uint          num_events_in_wait_list,
                    const cl_event * event_wait_list,
                    cl_event *       event,
                    cl_int *         errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    void* ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  buffer: %p\n", buffer );
    printf( "  blocking_map: %s\n", bool_to_string( blocking_map ) );
    printf( "  map_flags: %s\n", map_flags_to_string( map_flags ) );
    printf( "  offset: %lu\n", offset );
    printf( "  cp: %lu\n", cb );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueMapBuffer( command_queue, buffer, blocking_map, map_flags, 
                                      offset, cb, num_events_in_wait_list, 
                                      event_wait_list, event, errcode_ret );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    if( ret == NULL )
        printf( "  Returned buffer: NULL\n" );
    else
        printf( "  Returned buffer: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY void * CL_API_CALL
clEnqueueMapImage( cl_command_queue  command_queue,
                   cl_mem            image,
                   cl_bool           blocking_map,
                   cl_map_flags      map_flags,
                   const size_t *    origin,
                   const size_t *    region,
                   size_t *          image_row_pitch,
                   size_t *          image_slice_pitch,
                   cl_uint           num_events_in_wait_list,
                   const cl_event *  event_wait_list,
                   cl_event *        event,
                   cl_int *          errcode_ret )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    void* ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  image: %p\n", image );
    printf( "  blocking_map: %s\n", bool_to_string( blocking_map ) );
    printf( "  map_flags: %s\n", map_flags_to_string( map_flags ) );

    if( origin == NULL )
        printf( "  origin: NULL\n" );
    else
        printf( "  origin: (%lu, %lu, %lu)\n", origin[0], origin[1], origin[2] );
    
    if( region == NULL )
        printf( "  region: NULL\n" );
    else
        printf( "  region: (%lu, %lu, %lu)\n", region[0], region[1], region[2] );

    if( image_row_pitch == NULL )
        printf( "  image_row_pitch: NULL\n" );
    else
        printf( "  image_row_pitch: %lu\n", *image_row_pitch );

    if( image_row_pitch == NULL )
        printf( "  image_slice_pitch: NULL\n" );
    else
        printf( "  image_slice_pitch: %lu\n", *image_slice_pitch );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueMapImage( command_queue, image, blocking_map,
                                     map_flags, origin, region, image_row_pitch, 
                                     image_slice_pitch, num_events_in_wait_list, 
                                     event_wait_list, event, errcode_ret );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    if( ret == NULL )
        printf( "  Returned image: NULL\n" );
    else
        printf( "  Returned image: %p\n", ret );

    if( errcode_ret != NULL )
        printf( "  Return Code: %s\n", error_to_string( *errcode_ret ) );

    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueUnmapMemObject( cl_command_queue  command_queue,
                         cl_mem            memobj,
                         void *            mapped_ptr,
                         cl_uint           num_events_in_wait_list,
                         const cl_event *  event_wait_list,
                         cl_event *        event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  memobj: %p\n", memobj );
    printf( "  mapped_ptr: %p\n", mapped_ptr );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueUnmapMemObject( command_queue, memobj, mapped_ptr, 
                                           num_events_in_wait_list, event_wait_list, 
                                           event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNDRangeKernel( cl_command_queue command_queue,
                        cl_kernel        kernel,
                        cl_uint          work_dim,
                        const size_t *   global_work_offset,
                        const size_t *   global_work_size,
                        const size_t *   local_work_size,
                        cl_uint          num_events_in_wait_list,
                        const cl_event * event_wait_list,
                        cl_event *       event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  kernel: %p\n", kernel );
    printf( "  work_dim: %u\n", work_dim );
    
    if( global_work_offset == NULL )
        printf( "  global_work_offset: NULL\n" );
    else
        printf( "  global_work_offset: %lu\n", *global_work_offset );

    if( global_work_size == NULL )
        printf( "  global_work_size: NULL\n" );
    else
        printf( "  global_work_size: %lu\n", *global_work_size );

    if( local_work_size == NULL )
        printf( "  local_work_size: NULL\n" );
    else
        printf( "  local_work_size: %lu\n", *local_work_size );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueNDRangeKernel( command_queue, kernel, work_dim, 
                                          global_work_offset, global_work_size, 
                                          local_work_size, num_events_in_wait_list, 
                                          event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueTask( cl_command_queue  command_queue,
               cl_kernel         kernel,
               cl_uint           num_events_in_wait_list,
               const cl_event *  event_wait_list,
               cl_event *        event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  kernel: %p\n", kernel );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueTask( command_queue, kernel, num_events_in_wait_list, 
                                 event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNativeKernel( cl_command_queue  command_queue,
                       void (*user_func)(void *),
                       void *            args,
                       size_t            cb_args,
                       cl_uint           num_mem_objects,
                       const cl_mem *    mem_list,
                       const void **     args_mem_loc,
                       cl_uint           num_events_in_wait_list,
                       const cl_event *  event_wait_list,
                       cl_event *        event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  user_func: %p\n", user_func );
    printf( "  args: %p\n", args );
    printf( "  cb_args: %lu\n", cb_args );
    printf( "  num_mem_objects: %u\n", num_mem_objects );

    if( mem_list == NULL )
        printf( "  mem_list: NULL\n" );
    else
    {
        printf( "  mem_list:" );

        for( cl_uint i = 0; i < num_mem_objects; i++ )
        {
            printf( " %p", mem_list[i] );
        }

        printf( "\n" );
    }

    printf( "  args_mem_loc: %p\n", args_mem_loc );

    printf( "  num_events_in_wait_list: %u\n", num_events_in_wait_list );

    if( event_wait_list == NULL )
        printf( "  event_wait_list: NULL\n" );
    else
    {
        printf( "  event_wait_list:" );

        for( cl_uint i = 0; i < num_events_in_wait_list; i++ )
        {
            printf( " %p", event_wait_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueNativeKernel( command_queue, user_func, args, cb_args, 
                                         num_mem_objects, mem_list, args_mem_loc, 
                                         num_events_in_wait_list, event_wait_list, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueMarker( cl_command_queue    command_queue,
                 cl_event *          event )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    ret = g_clLib_clEnqueueMarker( command_queue, event );

    if( event == NULL )
        printf( "  event: NULL\n" );
    else
        printf( "  event: %p\n", *event );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWaitForEvents( cl_command_queue command_queue,
                        cl_uint          num_events,
                        const cl_event * event_list )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    printf( "  num_events: %u\n", num_events );

    if( event_list == NULL )
        printf( "  event_list: NULL\n" );
    else
    {
        printf( "  event_list:" );

        for( cl_uint i = 0; i < num_events; i++ )
        {
            printf( " %p", event_list[i] );
        }

        printf( "\n" );
    }

    ret = g_clLib_clEnqueueWaitForEvents( command_queue, num_events, event_list );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueBarrier( cl_command_queue command_queue )
{
    if( g_clLib == NULL )
        init_logger();

    printf( HEADER, __FUNCTION__ );

    cl_int ret;

    if( command_queue == NULL )
        printf( "  command_queue: NULL\n" );
    else
        printf( "  command_queue: %p\n", command_queue );

    ret = g_clLib_clEnqueueBarrier( command_queue );

    printf( "  Return Code: %s\n", error_to_string( ret ) );
    printf( FOOTER );

    return( ret );
}

//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
