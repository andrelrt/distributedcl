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
#include "icd_object_manager.h"

#define FILL_DISPATCH_ITEM(x) dispatch_table_. x = x
//-----------------------------------------------------------------------------
namespace dcl {
namespace icd {
//-----------------------------------------------------------------------------
icd_object_manager* icd_object_manager::instance_ptr_;
//-----------------------------------------------------------------------------
icd_object_manager::icd_object_manager()
{
    FILL_DISPATCH_ITEM( clBuildProgram );
    FILL_DISPATCH_ITEM( clCreateBuffer );
    FILL_DISPATCH_ITEM( clCreateCommandQueue );
    FILL_DISPATCH_ITEM( clCreateContext  );
    FILL_DISPATCH_ITEM( clCreateContextFromType );
    //FILL_DISPATCH_ITEM( clCreateFromGLBuffer );
    //FILL_DISPATCH_ITEM( clCreateFromGLRenderbuffer );
    //FILL_DISPATCH_ITEM( clCreateFromGLTexture2D );
    //FILL_DISPATCH_ITEM( clCreateFromGLTexture3D );
    FILL_DISPATCH_ITEM( clCreateImage2D );
    //FILL_DISPATCH_ITEM( clCreateImage3D );
    FILL_DISPATCH_ITEM( clCreateKernel );
    //FILL_DISPATCH_ITEM( clCreateKernelsInProgram );
    //FILL_DISPATCH_ITEM( clCreateProgramWithBinary );
    FILL_DISPATCH_ITEM( clCreateProgramWithSource );
    //FILL_DISPATCH_ITEM( clCreateSampler );
    //FILL_DISPATCH_ITEM( clCreateSubBuffer );
    //FILL_DISPATCH_ITEM( clCreateUserEvent );
    //FILL_DISPATCH_ITEM( clEnqueueAcquireGLObjects );
    //FILL_DISPATCH_ITEM( clEnqueueBarrier );
    //FILL_DISPATCH_ITEM( clEnqueueCopyBuffer );
    //FILL_DISPATCH_ITEM( clEnqueueCopyBufferRect );
    //FILL_DISPATCH_ITEM( clEnqueueCopyBufferToImage );
    //FILL_DISPATCH_ITEM( clEnqueueCopyImage );
    //FILL_DISPATCH_ITEM( clEnqueueCopyImageToBuffer );
    //FILL_DISPATCH_ITEM( clEnqueueMapBuffer );
    //FILL_DISPATCH_ITEM( clEnqueueMapImage );
    //FILL_DISPATCH_ITEM( clEnqueueMarker );
    FILL_DISPATCH_ITEM( clEnqueueNDRangeKernel );
    //FILL_DISPATCH_ITEM( clEnqueueNativeKernel );
    FILL_DISPATCH_ITEM( clEnqueueReadBuffer );
    //FILL_DISPATCH_ITEM( clEnqueueReadBufferRect );
    //FILL_DISPATCH_ITEM( clEnqueueReadImage );
    //FILL_DISPATCH_ITEM( clEnqueueReleaseGLObjects );
    //FILL_DISPATCH_ITEM( clEnqueueTask );
    //FILL_DISPATCH_ITEM( clEnqueueUnmapMemObject );
    //FILL_DISPATCH_ITEM( clEnqueueWaitForEvents );
    FILL_DISPATCH_ITEM( clEnqueueWriteBuffer );
    //FILL_DISPATCH_ITEM( clEnqueueWriteBufferRect );
    //FILL_DISPATCH_ITEM( clEnqueueWriteImage );
    FILL_DISPATCH_ITEM( clFinish );
    FILL_DISPATCH_ITEM( clFlush );
    //FILL_DISPATCH_ITEM( clGetCommandQueueInfo );
    FILL_DISPATCH_ITEM( clGetContextInfo );
    FILL_DISPATCH_ITEM( clGetDeviceIDs );
    FILL_DISPATCH_ITEM( clGetDeviceInfo );
    //FILL_DISPATCH_ITEM( clGetEventInfo );
    //FILL_DISPATCH_ITEM( clGetEventProfilingInfo );
    //FILL_DISPATCH_ITEM( clGetExtensionFunctionAddress );
    //FILL_DISPATCH_ITEM( clGetGLObjectInfo );
    //FILL_DISPATCH_ITEM( clGetGLTextureInfo );
    //FILL_DISPATCH_ITEM( clGetImageInfo );
    //FILL_DISPATCH_ITEM( clGetKernelInfo );
    FILL_DISPATCH_ITEM( clGetKernelWorkGroupInfo );
    FILL_DISPATCH_ITEM( clGetMemObjectInfo );
    FILL_DISPATCH_ITEM( clGetPlatformIDs );
    FILL_DISPATCH_ITEM( clGetPlatformInfo );
    FILL_DISPATCH_ITEM( clGetProgramBuildInfo );
    FILL_DISPATCH_ITEM( clGetProgramInfo );
    //FILL_DISPATCH_ITEM( clGetSamplerInfo );
    //FILL_DISPATCH_ITEM( clGetSupportedImageFormats );
    FILL_DISPATCH_ITEM( clReleaseCommandQueue );
    FILL_DISPATCH_ITEM( clReleaseContext );
    FILL_DISPATCH_ITEM( clReleaseEvent );
    FILL_DISPATCH_ITEM( clReleaseKernel );
    FILL_DISPATCH_ITEM( clReleaseMemObject );
    FILL_DISPATCH_ITEM( clReleaseProgram );
    //FILL_DISPATCH_ITEM( clReleaseSampler );
    FILL_DISPATCH_ITEM( clRetainCommandQueue );
    FILL_DISPATCH_ITEM( clRetainContext );
    FILL_DISPATCH_ITEM( clRetainEvent );
    FILL_DISPATCH_ITEM( clRetainKernel );
    FILL_DISPATCH_ITEM( clRetainMemObject );
    FILL_DISPATCH_ITEM( clRetainProgram );
    //FILL_DISPATCH_ITEM( clRetainSampler );
    //FILL_DISPATCH_ITEM( clSetCommandQueueProperty );
    //FILL_DISPATCH_ITEM( clSetEventCallback );
    FILL_DISPATCH_ITEM( clSetKernelArg );
    //FILL_DISPATCH_ITEM( clSetMemObjectDestructorCallback );
    //FILL_DISPATCH_ITEM( clSetUserEventStatus );
    FILL_DISPATCH_ITEM( clUnloadCompiler );
    FILL_DISPATCH_ITEM( clWaitForEvents );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::icd
//-----------------------------------------------------------------------------
