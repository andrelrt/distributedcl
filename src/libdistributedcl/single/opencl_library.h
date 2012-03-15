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
#ifndef _DCL_OPENCL_LIBRARY_H_
#define _DCL_OPENCL_LIBRARY_H_

#include <string>
#include "distributedcl_internal.h"

#if defined(WIN32)

#define LOAD_LIBRARY(x) LoadLibraryA( x )
#define FREE_LIBRARY(x) FreeLibrary( x )

typedef HMODULE library_handle_t;
static const library_handle_t null_library_handle = NULL;

#else // WIN32

#include <dlfcn.h>
#define LOAD_LIBRARY(x) dlopen( x, RTLD_LAZY )
#define FREE_LIBRARY(x) dlclose( x )

typedef void* library_handle_t;
static const library_handle_t null_library_handle = NULL;

#endif // WIN32

#include "info/opencl_functions.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
#define DECLARE_METHOD(x) dcl::info::x##_t x
//-----------------------------------------------------------------------------
class opencl_library
{
public:
	opencl_library();
	opencl_library( const std::string& libpath );
	opencl_library( const opencl_library& opencl );
	~opencl_library();
	
    inline bool operator==( const opencl_library& opencl ) const
    {
        return( clLib_ == opencl.clLib_ );
    }

	void load();
	void free();

    inline bool loaded() const
    {
        return( clLib_ != null_library_handle );
    }

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

    DECLARE_METHOD(clCreateSubBuffer);
    DECLARE_METHOD(clCreateUserEvent);
    DECLARE_METHOD(clEnqueueCopyBufferRect);
    DECLARE_METHOD(clEnqueueReadBufferRect);
    DECLARE_METHOD(clEnqueueWriteBufferRect);
    DECLARE_METHOD(clSetEventCallback);
    DECLARE_METHOD(clSetMemObjectDestructorCallback);
    DECLARE_METHOD(clSetUserEventStatus);

private:
	library_handle_t clLib_;
	std::string libpath_;

	void* load_function( const char* function );
};
//-----------------------------------------------------------------------------
template< typename CL_TYPE_T >
class opencl_object
{
public:
    opencl_object( opencl_object& obj ) : 
        id_( obj.get_id() ), opencl_( obj.get_opencl() ){}

    opencl_object( const opencl_library& opencl, CL_TYPE_T id = NULL ) : 
        id_( id ), opencl_( opencl ){}

    inline CL_TYPE_T get_id() const
    {
        return id_;
    }

    inline const opencl_library& get_opencl() const
    {
        return opencl_;
    }

protected:
    CL_TYPE_T id_;
    const opencl_library& opencl_;

    inline void set_id( CL_TYPE_T id )
    {
        id_ = id;
    }
};
//-----------------------------------------------------------------------------
template< typename CL_TYPE_T >
struct reference_wrapper
{
    static inline void retain( const opencl_library& opencl, CL_TYPE_T id ){}
    static inline void release( const opencl_library& opencl, CL_TYPE_T id ){}
};
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
#endif // _DCL_OPENCL_LIBRARY_H_
