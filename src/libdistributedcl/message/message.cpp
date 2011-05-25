/*
 * Copyright (c) 2009-2010 André Tupinambá (andrelrt@gmail.com)
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
#include "message.h"
#include "distributedcl_internal.h"
#include "msg_device.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
#define THROW_IF(b,ex) if(b) throw dcl::library_exception(ex,-1)
    
message* message::parse_message( uint8_t* msg_buffer_ptr, 
                                 std::size_t length )
{
    const message_header* header_ptr = reinterpret_cast< const message_header* >( msg_buffer_ptr );

    uint32_t message_len = ntohl( header_ptr->length );

    THROW_IF( (length < sizeof( message_header )), "Invalid message size" );
    THROW_IF( (length < message_len), "Message size mismatch" );
    THROW_IF( (header_ptr->version != message_v1_0), "Invalid message version" );

    message* ret_ptr = NULL;

    switch( header_ptr->type )
    {
        // TODO: Create the message objects

        // Internal Messages [1-20)
        case msg_invalid_message:
        case msg_error_message:
            throw dcl::library_exception( "Not implemented", -1 );
            break;

        // OpenCL Messages [20-128)
        case msgGetPlatformIDs:
        case msgGetPlatformInfo:
            throw dcl::library_exception( "Not implemented", -1 );
            break;

        case msgGetDeviceIDs:
            ret_ptr = reinterpret_cast< message* >( new dcl_message< msgGetPlatformIDs >() );
            break;

        case msgGetDeviceInfo:
            throw dcl::library_exception( "Not implemented", -1 );
            break;

        case msgCreateContext:
        case msgCreateContextFromType:
        case msgRetainContext:
        case msgReleaseContext:
        case msgGetContextInfo:

        case msgCreateCommandQueue:
        case msgRetainCommandQueue:
        case msgReleaseCommandQueue:
        case msgGetCommandQueueInfo:
        case msgSetCommandQueueProperty:

        case msgCreateBuffer:
        case msgCreateImage2D:
        case msgCreateImage3D:
        case msgRetainMemObject:
        case msgReleaseMemObject:
        case msgGetSupportedImageFormats:
        case msgGetMemObjectInfo:
        case msgGetImageInfo:

        case msgCreateSampler:
        case msgRetainSampler:
        case msgReleaseSampler:
        case msgGetSamplerInfo:

        case msgCreateProgramWithSource:
        case msgCreateProgramWithBinary:
        case msgRetainProgram:
        case msgReleaseProgram:
        case msgBuildProgram:
        case msgUnloadCompiler:
        case msgGetProgramInfo:
        case msgGetProgramBuildInfo:

        case msgCreateKernel:
        case msgCreateKernelsInProgram:
        case msgRetainKernel:
        case msgReleaseKernel:
        case msgSetKernelArg:
        case msgGetKernelInfo:
        case msgGetKernelWorkGroupInfo:

        case msgWaitForEvents:
        case msgGetEventInfo:
        case msgRetainEvent:
        case msgReleaseEvent:
        case msgGetEventProfilingInfo:

        case msgFlush:
        case msgFinish:

        case msgEnqueueReadBuffer:
        case msgEnqueueWriteBuffer:
        case msgEnqueueCopyBuffer:
        case msgEnqueueReadImage:
        case msgEnqueueWriteImage:
        case msgEnqueueCopyImage:
        case msgEnqueueCopyImageToBuffer:
        case msgEnqueueCopyBufferToImage:
        case msgEnqueueMapBuffer:
        case msgEnqueueMapImage:
        case msgEnqueueUnmapMemObject:
        case msgEnqueueNDRangeKernel:
        case msgEnqueueTask:
        case msgEnqueueNativeKernel:
        case msgEnqueueMarker:
        case msgEnqueueWaitForEvents:
        case msgEnqueueBarrier:

        // OpenCL Extension Messages [128-255]
        case msgExtension0:
        case msgExtension1:
            throw dcl::library_exception( "Not implemented", -1 );
            break;

        default: 
            throw dcl::library_exception( "Not implemented", -1 );
    }

    ret_ptr->set_buffer( msg_buffer_ptr, length );

    return ret_ptr;
}

#undef THROW_IF
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
