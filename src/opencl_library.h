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
#include <string>
#include <CL/opencl.h>
//-----------------------------------------------------------------------------

namespace ocg 
{

//-----------------------------------------------------------------------------
extern "C" {

typedef void (*logging_fn)(const char *, const void *, size_t, void *);

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

} // extern "C"
//-----------------------------------------------------------------------------
#define DECLARE_METHOD(x) x##_t x
//-----------------------------------------------------------------------------
class opencl_library
{
public:
	opencl_library( const std::string& libpath );
	~opencl_library();
	
	void load();
	void free();

    DECLARE_METHOD(clGetPlatformIDs);
    DECLARE_METHOD(clGetPlatformInfo);

    DECLARE_METHOD(clGetDeviceIDs);
    DECLARE_METHOD(clGetDeviceInfo);

    DECLARE_METHOD(clCreateContext);
    DECLARE_METHOD(clCreateContextFromType);
    DECLARE_METHOD(clRetainContext);
    DECLARE_METHOD(clReleaseContext);
    DECLARE_METHOD(clGetContextInfo);

    DECLARE_METHOD(clCreateCommandQueue);
    DECLARE_METHOD(clRetainCommandQueue);
    DECLARE_METHOD(clReleaseCommandQueue);
    DECLARE_METHOD(clGetCommandQueueInfo);
    DECLARE_METHOD(clSetCommandQueueProperty);

    DECLARE_METHOD(clCreateBuffer);
    DECLARE_METHOD(clCreateImage2D);
    DECLARE_METHOD(clCreateImage3D);
    DECLARE_METHOD(clRetainMemObject);
    DECLARE_METHOD(clReleaseMemObject);
    DECLARE_METHOD(clGetSupportedImageFormats);
    DECLARE_METHOD(clGetMemObjectInfo);
    DECLARE_METHOD(clGetImageInfo);

    DECLARE_METHOD(clCreateSampler);
    DECLARE_METHOD(clRetainSampler);
    DECLARE_METHOD(clReleaseSampler);
    DECLARE_METHOD(clGetSamplerInfo);

    DECLARE_METHOD(clCreateProgramWithSource);
    DECLARE_METHOD(clCreateProgramWithBinary);
    DECLARE_METHOD(clRetainProgram);
    DECLARE_METHOD(clReleaseProgram);
    DECLARE_METHOD(clBuildProgram);
    DECLARE_METHOD(clUnloadCompiler);
    DECLARE_METHOD(clGetProgramInfo);
    DECLARE_METHOD(clGetProgramBuildInfo);

    DECLARE_METHOD(clCreateKernel);
    DECLARE_METHOD(clCreateKernelsInProgram);
    DECLARE_METHOD(clRetainKernel);
    DECLARE_METHOD(clReleaseKernel);
    DECLARE_METHOD(clSetKernelArg);
    DECLARE_METHOD(clGetKernelInfo);
    DECLARE_METHOD(clGetKernelWorkGroupInfo);

    DECLARE_METHOD(clWaitForEvents);
    DECLARE_METHOD(clGetEventInfo);
    DECLARE_METHOD(clRetainEvent);
    DECLARE_METHOD(clReleaseEvent);
    DECLARE_METHOD(clGetEventProfilingInfo);

    DECLARE_METHOD(clFlush);
    DECLARE_METHOD(clFinish);

    DECLARE_METHOD(clEnqueueReadBuffer);
    DECLARE_METHOD(clEnqueueWriteBuffer);
    DECLARE_METHOD(clEnqueueCopyBuffer);
    DECLARE_METHOD(clEnqueueReadImage);
    DECLARE_METHOD(clEnqueueWriteImage);
    DECLARE_METHOD(clEnqueueCopyImage);
    DECLARE_METHOD(clEnqueueCopyImageToBuffer);
    DECLARE_METHOD(clEnqueueCopyBufferToImage);
    DECLARE_METHOD(clEnqueueMapBuffer);
    DECLARE_METHOD(clEnqueueMapImage);
    DECLARE_METHOD(clEnqueueUnmapMemObject);
    DECLARE_METHOD(clEnqueueNDRangeKernel);
    DECLARE_METHOD(clEnqueueTask);
    DECLARE_METHOD(clEnqueueNativeKernel);
    DECLARE_METHOD(clEnqueueMarker);
    DECLARE_METHOD(clEnqueueWaitForEvents);
    DECLARE_METHOD(clEnqueueBarrier);

private:
	void* clLib_;
	std::string libpath_;

	void* load_function( const char* function );
};

//-----------------------------------------------------------------------------
} // namespace ocg
