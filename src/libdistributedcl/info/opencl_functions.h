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
#ifndef _DCL_OPENCL_FUNCTIONS_H_
#define _DCL_OPENCL_FUNCTIONS_H_

#ifdef __APPLE__
#include <OpenCL/cl_gl.h>
#include <OpenCL/cl_gl_ext.h>
#else
#include <CL/cl_gl.h>
#include <CL/cl_gl_ext.h>
#endif

#ifdef WIN32
#define WINDOWS_MEAN_AND_LEAN
#include <Windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
extern "C" {

typedef void (CL_API_CALL *logging_fn)(const char *, const void *, size_t, void *);

typedef cl_int
(CL_API_CALL *clGetPlatformIDs_t)( cl_uint, cl_platform_id*, cl_uint* );

typedef cl_int
(CL_API_CALL *clGetPlatformInfo_t)( cl_platform_id, cl_platform_info, size_t, void*, size_t* );

typedef cl_int 
(CL_API_CALL *clGetDeviceIDs_t)( cl_platform_id, cl_device_type, cl_uint, cl_device_id*, cl_uint* );

typedef cl_int 
(CL_API_CALL *clGetDeviceInfo_t)( cl_device_id, cl_device_info, size_t, void*, size_t* );

typedef cl_context 
(CL_API_CALL *clCreateContext_t)( cl_context_properties*, cl_uint, const cl_device_id*, logging_fn, void*, cl_int* );

typedef cl_context 
(CL_API_CALL *clCreateContextFromType_t)( cl_context_properties*, cl_device_type, logging_fn, void*, cl_int* );

typedef cl_int 
(CL_API_CALL *clRetainContext_t)( cl_context );

typedef cl_int 
(CL_API_CALL *clReleaseContext_t)( cl_context );

typedef cl_int 
(CL_API_CALL *clGetContextInfo_t)( cl_context, cl_context_info, size_t, void*, size_t* );

typedef cl_command_queue 
(CL_API_CALL *clCreateCommandQueue_t)( cl_context, cl_device_id, cl_command_queue_properties, cl_int* );

typedef cl_int 
(CL_API_CALL *clRetainCommandQueue_t)( cl_command_queue );

typedef cl_int 
(CL_API_CALL *clReleaseCommandQueue_t)( cl_command_queue );

typedef cl_int 
(CL_API_CALL *clGetCommandQueueInfo_t)( cl_command_queue, cl_command_queue_info, size_t, void*, size_t* );

typedef cl_int 
(CL_API_CALL *clSetCommandQueueProperty_t)( cl_command_queue, cl_command_queue_properties, cl_bool, cl_command_queue_properties* );

typedef cl_mem 
(CL_API_CALL *clCreateBuffer_t)( cl_context, cl_mem_flags, size_t, void*, cl_int* );

typedef cl_mem 
(CL_API_CALL *clCreateImage2D_t)( cl_context, cl_mem_flags, const cl_image_format*, size_t, size_t, size_t, void*, cl_int* );

typedef cl_mem 
(CL_API_CALL *clCreateImage3D_t)( cl_context, cl_mem_flags, const cl_image_format*, size_t, size_t, size_t, size_t, size_t, void*, cl_int* );

typedef cl_int 
(CL_API_CALL *clRetainMemObject_t)( cl_mem );

typedef cl_int 
(CL_API_CALL *clReleaseMemObject_t)( cl_mem );

typedef cl_int 
(CL_API_CALL *clGetSupportedImageFormats_t)( cl_context, cl_mem_flags, cl_mem_object_type, cl_uint, cl_image_format*, cl_uint* );

typedef cl_int 
(CL_API_CALL *clGetMemObjectInfo_t)( cl_mem, cl_mem_info, size_t, void*, size_t* );

typedef cl_int 
(CL_API_CALL *clGetImageInfo_t)( cl_mem, cl_image_info, size_t, void*, size_t* );

typedef cl_sampler 
(CL_API_CALL *clCreateSampler_t)( cl_context, cl_bool, cl_addressing_mode, cl_filter_mode, cl_int* );

typedef cl_int 
(CL_API_CALL *clRetainSampler_t)( cl_sampler );

typedef cl_int 
(CL_API_CALL *clReleaseSampler_t)( cl_sampler );

typedef cl_int 
(CL_API_CALL *clGetSamplerInfo_t)( cl_sampler, cl_sampler_info, size_t, void*, size_t* );

typedef cl_program 
(CL_API_CALL *clCreateProgramWithSource_t)( cl_context, cl_uint, const char**, const size_t*, cl_int* );

typedef cl_program 
(CL_API_CALL *clCreateProgramWithBinary_t)( cl_context, cl_uint, const cl_device_id*, const size_t*, const unsigned char**, cl_int*, cl_int* );

typedef cl_int 
(CL_API_CALL *clRetainProgram_t)( cl_program );

typedef cl_int 
(CL_API_CALL *clReleaseProgram_t)( cl_program );

typedef cl_int 
(CL_API_CALL *clBuildProgram_t)( cl_program, cl_uint, const cl_device_id*, const char*, void (*pfn_notify)( cl_program, void* ), void* );

typedef cl_int 
(CL_API_CALL *clUnloadCompiler_t)( void );

typedef cl_int 
(CL_API_CALL *clGetProgramInfo_t)( cl_program, cl_program_info, size_t, void*, size_t* );

typedef cl_int 
(CL_API_CALL *clGetProgramBuildInfo_t)( cl_program, cl_device_id, cl_program_build_info, size_t, void*, size_t* );

typedef cl_kernel 
(CL_API_CALL *clCreateKernel_t)( cl_program, const char*, cl_int* );

typedef cl_int 
(CL_API_CALL *clCreateKernelsInProgram_t)( cl_program, cl_uint, cl_kernel*, cl_uint* );

typedef cl_int 
(CL_API_CALL *clRetainKernel_t)( cl_kernel );

typedef cl_int 
(CL_API_CALL *clReleaseKernel_t)( cl_kernel );

typedef cl_int 
(CL_API_CALL *clSetKernelArg_t)( cl_kernel, cl_uint, size_t, const void* );

typedef cl_int 
(CL_API_CALL *clGetKernelInfo_t)( cl_kernel, cl_kernel_info, size_t, void*, size_t* );

typedef cl_int 
(CL_API_CALL *clGetKernelWorkGroupInfo_t)( cl_kernel, cl_device_id, cl_kernel_work_group_info, size_t, void*, size_t* );

typedef cl_int 
(CL_API_CALL *clWaitForEvents_t)( cl_uint, const cl_event* );

typedef cl_int 
(CL_API_CALL *clGetEventInfo_t)( cl_event, cl_event_info, size_t, void*, size_t* );

typedef cl_int 
(CL_API_CALL *clRetainEvent_t)( cl_event );

typedef cl_int 
(CL_API_CALL *clReleaseEvent_t)( cl_event );

typedef cl_int 
(CL_API_CALL *clGetEventProfilingInfo_t)( cl_event, cl_profiling_info, size_t, void*, size_t* );

typedef cl_int 
(CL_API_CALL *clFlush_t)( cl_command_queue );

typedef cl_int 
(CL_API_CALL *clFinish_t)( cl_command_queue );

typedef cl_int 
(CL_API_CALL *clEnqueueReadBuffer_t)( cl_command_queue, cl_mem, cl_bool, size_t, size_t, void*, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueWriteBuffer_t)( cl_command_queue, cl_mem, cl_bool, size_t, size_t, const void*, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueCopyBuffer_t)( cl_command_queue, cl_mem, cl_mem, size_t, size_t, size_t, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueReadImage_t)( cl_command_queue, cl_mem, cl_bool, const size_t*, const size_t*, size_t, size_t, void*, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueWriteImage_t)( cl_command_queue, cl_mem, cl_bool, const size_t*, const size_t*, size_t, size_t, const void*, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueCopyImage_t)( cl_command_queue, cl_mem, cl_mem, const size_t*, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueCopyImageToBuffer_t)( cl_command_queue, cl_mem, cl_mem, const size_t*, const size_t*, size_t, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueCopyBufferToImage_t)( cl_command_queue, cl_mem, cl_mem, size_t, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event* );

typedef void * 
(CL_API_CALL *clEnqueueMapBuffer_t)( cl_command_queue, cl_mem, cl_bool, cl_map_flags, size_t, size_t, cl_uint, const cl_event*, cl_event*, cl_int* );

typedef void * 
(CL_API_CALL *clEnqueueMapImage_t)( cl_command_queue, cl_mem, cl_bool, cl_map_flags, const size_t*, const size_t*,
                                    size_t*, size_t*, cl_uint, const cl_event*, cl_event*, cl_int* );

typedef cl_int 
(CL_API_CALL *clEnqueueUnmapMemObject_t)( cl_command_queue, cl_mem, void*, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueNDRangeKernel_t)( cl_command_queue, cl_kernel, cl_uint, const size_t*, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueTask_t)( cl_command_queue, cl_kernel, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueNativeKernel_t)( cl_command_queue, void (*user_func)(void *), void*, size_t, cl_uint, const cl_mem*, const void**, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueMarker_t)( cl_command_queue, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueWaitForEvents_t)( cl_command_queue, cl_uint, const cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueBarrier_t)( cl_command_queue );

typedef cl_mem
(CL_API_CALL *clCreateSubBuffer_t)( cl_mem, cl_mem_flags, cl_buffer_create_type, const void*, cl_int* );

typedef cl_event
(CL_API_CALL *clCreateUserEvent_t)( cl_context, cl_int* );

typedef cl_int 
(CL_API_CALL *clEnqueueCopyBufferRect_t)( cl_command_queue, cl_mem, cl_mem, const size_t*, const size_t*, const size_t*, 
                                          size_t, size_t, size_t, size_t, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueReadBufferRect_t)( cl_command_queue, cl_mem, cl_bool, const size_t*, const size_t*, const size_t*,
                                          size_t, size_t, size_t, size_t, void*, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clEnqueueWriteBufferRect_t)( cl_command_queue, cl_mem, cl_bool, const size_t*, const size_t*, const size_t*,
                                           size_t, size_t, size_t, size_t, const void*, cl_uint, const cl_event*, cl_event* );

typedef cl_int 
(CL_API_CALL *clSetEventCallback_t)( cl_event, cl_int, void (CL_CALLBACK *)( cl_event, cl_int, void * ), void* );

typedef cl_int 
(CL_API_CALL *clSetMemObjectDestructorCallback_t)( cl_mem, void (CL_CALLBACK *)( cl_mem, void* ), void* );

typedef cl_int 
(CL_API_CALL *clSetUserEventStatus_t)( cl_event, cl_int );

typedef void*
(CL_API_CALL *clGetExtensionFunctionAddress_t)( const char* );

typedef cl_mem
(CL_API_CALL *clCreateFromGLBuffer_t)( cl_context, cl_mem_flags, GLuint, cl_int* );

typedef cl_mem
(CL_API_CALL *clCreateFromGLRenderbuffer_t)( cl_context, cl_mem_flags, GLuint, cl_int* );

typedef cl_mem
(CL_API_CALL *clCreateFromGLTexture2D_t)( cl_context, cl_mem_flags, GLenum, GLint, GLuint, cl_int* );

typedef cl_mem
(CL_API_CALL *clCreateFromGLTexture3D_t)( cl_context, cl_mem_flags, GLenum, GLint, GLuint, cl_int* );

typedef cl_int
(CL_API_CALL *clEnqueueAcquireGLObjects_t)( cl_command_queue, cl_uint, const cl_mem*, cl_uint, const cl_event*, cl_event* );

typedef cl_int
(CL_API_CALL *clEnqueueReleaseGLObjects_t)( cl_command_queue, cl_uint, const cl_mem*, cl_uint, const cl_event*, cl_event* );

typedef cl_int
(CL_API_CALL *clGetGLObjectInfo_t)( cl_mem, cl_gl_object_type*, GLuint* );

typedef cl_int
(CL_API_CALL *clGetGLTextureInfo_t)( cl_mem, cl_gl_texture_info, size_t, void*, size_t* );

typedef cl_mem
(CL_API_CALL *clCreateSubBuffer_t)( cl_mem, cl_mem_flags, cl_buffer_create_type, const void*, cl_int* );

typedef cl_event
(CL_API_CALL *clCreateUserEvent_t)( cl_context, cl_int* );

typedef cl_int
(CL_API_CALL *clEnqueueCopyBufferRect_t)( cl_command_queue, cl_mem, cl_mem, const size_t*, const size_t*, const size_t*, 
                                          size_t, size_t, size_t, size_t, cl_uint, const cl_event*, cl_event* );

typedef cl_int
(CL_API_CALL *clEnqueueReadBufferRect_t)( cl_command_queue, cl_mem, cl_bool, const size_t*, const size_t*, const size_t*,
                                          size_t, size_t, size_t, size_t, void*, cl_uint, const cl_event*, cl_event* );

typedef cl_int
(CL_API_CALL *clEnqueueWriteBufferRect_t)( cl_command_queue, cl_mem, cl_bool, const size_t*, const size_t*, const size_t*,
                                           size_t, size_t, size_t, size_t, const void*, cl_uint, const cl_event*, cl_event* );

typedef cl_int
(CL_API_CALL *clSetEventCallback_t)( cl_event, cl_int, void (CL_CALLBACK *pfn_notify)( cl_event, cl_int, void* ), void* );

typedef cl_int
(CL_API_CALL *clSetMemObjectDestructorCallback_t)( cl_mem, void (CL_CALLBACK *pfn_notify)( cl_mem, void* ), void* );

typedef cl_int
(CL_API_CALL *clSetUserEventStatus_t)( cl_event, cl_int );

} // extern "C"
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_OPENCL_FUNCTIONS_H_
