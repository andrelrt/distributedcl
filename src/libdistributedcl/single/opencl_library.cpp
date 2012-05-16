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
#include "opencl_library.h"
#include "distributedcl_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
opencl_library::opencl_library() :
    clGetPlatformIDs( NULL ),
    clGetPlatformInfo( NULL ),
    clGetDeviceIDs( NULL ),
    clGetDeviceInfo( NULL ),
    clCreateContext( NULL ),
    clCreateContextFromType( NULL ),
    clRetainContext( NULL ),
    clReleaseContext( NULL ),
    clGetContextInfo( NULL ),
    clCreateCommandQueue( NULL ),
    clRetainCommandQueue( NULL ),
    clReleaseCommandQueue( NULL ),
    clGetCommandQueueInfo( NULL ),
    clSetCommandQueueProperty( NULL ),
    clCreateBuffer( NULL ),
    clCreateImage2D( NULL ),
    clCreateImage3D( NULL ),
    clRetainMemObject( NULL ),
    clReleaseMemObject( NULL ),
    clGetSupportedImageFormats( NULL ),
    clGetMemObjectInfo( NULL ),
    clGetImageInfo( NULL ),
    clCreateSampler( NULL ),
    clRetainSampler( NULL ),
    clReleaseSampler( NULL ),
    clGetSamplerInfo( NULL ),
    clCreateProgramWithSource( NULL ),
    clCreateProgramWithBinary( NULL ),
    clRetainProgram( NULL ),
    clReleaseProgram( NULL ),
    clBuildProgram( NULL ),
    clUnloadCompiler( NULL ),
    clGetProgramInfo( NULL ),
    clGetProgramBuildInfo( NULL ),
    clCreateKernel( NULL ),
    clCreateKernelsInProgram( NULL ),
    clRetainKernel( NULL ),
    clReleaseKernel( NULL ),
    clSetKernelArg( NULL ),
    clGetKernelInfo( NULL ),
    clGetKernelWorkGroupInfo( NULL ),
    clWaitForEvents( NULL ),
    clGetEventInfo( NULL ),
    clRetainEvent( NULL ),
    clReleaseEvent( NULL ),
    clGetEventProfilingInfo( NULL ),
    clFlush( NULL ),
    clFinish( NULL ),
    clEnqueueReadBuffer( NULL ),
    clEnqueueWriteBuffer( NULL ),
    clEnqueueCopyBuffer( NULL ),
    clEnqueueReadImage( NULL ),
    clEnqueueWriteImage( NULL ),
    clEnqueueCopyImage( NULL ),
    clEnqueueCopyImageToBuffer( NULL ),
    clEnqueueCopyBufferToImage( NULL ),
    clEnqueueMapBuffer( NULL ),
    clEnqueueMapImage( NULL ),
    clEnqueueUnmapMemObject( NULL ),
    clEnqueueNDRangeKernel( NULL ),
    clEnqueueTask( NULL ),
    clEnqueueNativeKernel( NULL ),
    clEnqueueMarker( NULL ),
    clEnqueueWaitForEvents( NULL ),
    clEnqueueBarrier( NULL ),
    clCreateSubBuffer( NULL ),
    clCreateUserEvent( NULL ),
    clEnqueueCopyBufferRect( NULL ),
    clEnqueueReadBufferRect( NULL ),
    clEnqueueWriteBufferRect( NULL ),
    clSetEventCallback( NULL ),
    clSetMemObjectDestructorCallback( NULL ),
    clSetUserEventStatus( NULL ),
	clLib_( NULL ),
	libpath_( "" )
{
}
//-----------------------------------------------------------------------------
opencl_library::opencl_library( const opencl_library& opencl ) :
    clGetPlatformIDs( NULL ),
    clGetPlatformInfo( NULL ),
    clGetDeviceIDs( NULL ),
    clGetDeviceInfo( NULL ),
    clCreateContext( NULL ),
    clCreateContextFromType( NULL ),
    clRetainContext( NULL ),
    clReleaseContext( NULL ),
    clGetContextInfo( NULL ),
    clCreateCommandQueue( NULL ),
    clRetainCommandQueue( NULL ),
    clReleaseCommandQueue( NULL ),
    clGetCommandQueueInfo( NULL ),
    clSetCommandQueueProperty( NULL ),
    clCreateBuffer( NULL ),
    clCreateImage2D( NULL ),
    clCreateImage3D( NULL ),
    clRetainMemObject( NULL ),
    clReleaseMemObject( NULL ),
    clGetSupportedImageFormats( NULL ),
    clGetMemObjectInfo( NULL ),
    clGetImageInfo( NULL ),
    clCreateSampler( NULL ),
    clRetainSampler( NULL ),
    clReleaseSampler( NULL ),
    clGetSamplerInfo( NULL ),
    clCreateProgramWithSource( NULL ),
    clCreateProgramWithBinary( NULL ),
    clRetainProgram( NULL ),
    clReleaseProgram( NULL ),
    clBuildProgram( NULL ),
    clUnloadCompiler( NULL ),
    clGetProgramInfo( NULL ),
    clGetProgramBuildInfo( NULL ),
    clCreateKernel( NULL ),
    clCreateKernelsInProgram( NULL ),
    clRetainKernel( NULL ),
    clReleaseKernel( NULL ),
    clSetKernelArg( NULL ),
    clGetKernelInfo( NULL ),
    clGetKernelWorkGroupInfo( NULL ),
    clWaitForEvents( NULL ),
    clGetEventInfo( NULL ),
    clRetainEvent( NULL ),
    clReleaseEvent( NULL ),
    clGetEventProfilingInfo( NULL ),
    clFlush( NULL ),
    clFinish( NULL ),
    clEnqueueReadBuffer( NULL ),
    clEnqueueWriteBuffer( NULL ),
    clEnqueueCopyBuffer( NULL ),
    clEnqueueReadImage( NULL ),
    clEnqueueWriteImage( NULL ),
    clEnqueueCopyImage( NULL ),
    clEnqueueCopyImageToBuffer( NULL ),
    clEnqueueCopyBufferToImage( NULL ),
    clEnqueueMapBuffer( NULL ),
    clEnqueueMapImage( NULL ),
    clEnqueueUnmapMemObject( NULL ),
    clEnqueueNDRangeKernel( NULL ),
    clEnqueueTask( NULL ),
    clEnqueueNativeKernel( NULL ),
    clEnqueueMarker( NULL ),
    clEnqueueWaitForEvents( NULL ),
    clEnqueueBarrier( NULL ),
    clCreateSubBuffer( NULL ),
    clCreateUserEvent( NULL ),
    clEnqueueCopyBufferRect( NULL ),
    clEnqueueReadBufferRect( NULL ),
    clEnqueueWriteBufferRect( NULL ),
    clSetEventCallback( NULL ),
    clSetMemObjectDestructorCallback( NULL ),
    clSetUserEventStatus( NULL ),
	clLib_( NULL ),
	libpath_( opencl.libpath_ )
{
	load();
}
//-----------------------------------------------------------------------------
opencl_library::opencl_library( const std::string& libpath ) :
    clGetPlatformIDs( NULL ),
    clGetPlatformInfo( NULL ),
    clGetDeviceIDs( NULL ),
    clGetDeviceInfo( NULL ),
    clCreateContext( NULL ),
    clCreateContextFromType( NULL ),
    clRetainContext( NULL ),
    clReleaseContext( NULL ),
    clGetContextInfo( NULL ),
    clCreateCommandQueue( NULL ),
    clRetainCommandQueue( NULL ),
    clReleaseCommandQueue( NULL ),
    clGetCommandQueueInfo( NULL ),
    clSetCommandQueueProperty( NULL ),
    clCreateBuffer( NULL ),
    clCreateImage2D( NULL ),
    clCreateImage3D( NULL ),
    clRetainMemObject( NULL ),
    clReleaseMemObject( NULL ),
    clGetSupportedImageFormats( NULL ),
    clGetMemObjectInfo( NULL ),
    clGetImageInfo( NULL ),
    clCreateSampler( NULL ),
    clRetainSampler( NULL ),
    clReleaseSampler( NULL ),
    clGetSamplerInfo( NULL ),
    clCreateProgramWithSource( NULL ),
    clCreateProgramWithBinary( NULL ),
    clRetainProgram( NULL ),
    clReleaseProgram( NULL ),
    clBuildProgram( NULL ),
    clUnloadCompiler( NULL ),
    clGetProgramInfo( NULL ),
    clGetProgramBuildInfo( NULL ),
    clCreateKernel( NULL ),
    clCreateKernelsInProgram( NULL ),
    clRetainKernel( NULL ),
    clReleaseKernel( NULL ),
    clSetKernelArg( NULL ),
    clGetKernelInfo( NULL ),
    clGetKernelWorkGroupInfo( NULL ),
    clWaitForEvents( NULL ),
    clGetEventInfo( NULL ),
    clRetainEvent( NULL ),
    clReleaseEvent( NULL ),
    clGetEventProfilingInfo( NULL ),
    clFlush( NULL ),
    clFinish( NULL ),
    clEnqueueReadBuffer( NULL ),
    clEnqueueWriteBuffer( NULL ),
    clEnqueueCopyBuffer( NULL ),
    clEnqueueReadImage( NULL ),
    clEnqueueWriteImage( NULL ),
    clEnqueueCopyImage( NULL ),
    clEnqueueCopyImageToBuffer( NULL ),
    clEnqueueCopyBufferToImage( NULL ),
    clEnqueueMapBuffer( NULL ),
    clEnqueueMapImage( NULL ),
    clEnqueueUnmapMemObject( NULL ),
    clEnqueueNDRangeKernel( NULL ),
    clEnqueueTask( NULL ),
    clEnqueueNativeKernel( NULL ),
    clEnqueueMarker( NULL ),
    clEnqueueWaitForEvents( NULL ),
    clEnqueueBarrier( NULL ),
    clCreateSubBuffer( NULL ),
    clCreateUserEvent( NULL ),
    clEnqueueCopyBufferRect( NULL ),
    clEnqueueReadBufferRect( NULL ),
    clEnqueueWriteBufferRect( NULL ),
    clSetEventCallback( NULL ),
    clSetMemObjectDestructorCallback( NULL ),
    clSetUserEventStatus( NULL ),
	clLib_( NULL ),
	libpath_( libpath )
{
    load();
}
//-----------------------------------------------------------------------------
opencl_library::~opencl_library()
{
	free();
}
//-----------------------------------------------------------------------------
#define LOAD_FUNCTION(x) x=(dcl::info::x##_t)load_function(#x)

void opencl_library::load()
{
    clLib_ = LOAD_LIBRARY( libpath_.c_str() );

    if( clLib_ == null_library_handle )
    {
#if defined(WIN32)
	    throw dcl::library_exception( "LoadLibrary error", GetLastError() );
#else
        throw dcl::library_exception( dlerror() );
#endif // WIN32
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

    // OpenCL 1.1
    //LOAD_FUNCTION(clCreateSubBuffer);
    //LOAD_FUNCTION(clCreateUserEvent);
    //LOAD_FUNCTION(clEnqueueCopyBufferRect);
    //LOAD_FUNCTION(clEnqueueReadBufferRect);
    //LOAD_FUNCTION(clEnqueueWriteBufferRect);
    //LOAD_FUNCTION(clSetEventCallback);
    //LOAD_FUNCTION(clSetMemObjectDestructorCallback);
    //LOAD_FUNCTION(clSetUserEventStatus);
}
#undef LOAD_FUNCTION
//-----------------------------------------------------------------------------
void opencl_library::free()
{
	if( clLib_ != null_library_handle )
	{
        FREE_LIBRARY( clLib_ );
		clLib_ = null_library_handle;
	}
}

//-----------------------------------------------------------------------------
#if defined(WIN32)

void* opencl_library::load_function( const char* function )
{
    void* ret = GetProcAddress( clLib_, function );

    if( ret == NULL )
    {
	    throw dcl::library_exception( "GetProcAddress error", GetLastError() );
    }

    return( ret );
}

#else // WIN32

void* opencl_library::load_function( const char* function )
{
    void *ret = dlsym( clLib_, function );

    const char *error;

    if( (error = dlerror()) != NULL )
    {
	    throw dcl::library_exception( error );
    }

    return( ret );
}
#endif // WIN32
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
