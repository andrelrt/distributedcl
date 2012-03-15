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
#include "message.h"
#include "distributedcl_internal.h"
#include "msg_device.h"
#include "msg_context.h"
#include "msg_program.h"
#include "msg_kernel.h"
#include "msg_internal.h"
#include "msg_command_queue.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
#define THROW_IF(b,ex) if(b) throw dcl::library_exception(ex)
    
base_message* base_message::parse_message( uint8_t* msg_buffer_ptr, std::size_t length )
{
    const message_header* header_ptr = reinterpret_cast< const message_header* >( msg_buffer_ptr );

    uint32_t message_len = network_to_host( header_ptr->length );

    THROW_IF( (length < sizeof( message_header )), "Invalid base_message size" );
    THROW_IF( (length < message_len), "Message size mismatch" );
    THROW_IF( (header_ptr->version != message_v1_0), "Invalid base_message version" );

    base_message* ret_ptr = NULL;

    switch( header_ptr->type )
    {
        // TODO: Create the base_message objects

        // Internal base_messages [1-20)
        case msg_invalid_message:
            throw dcl::library_exception( "Not implemented" );
            break;

        case msg_error_message:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msg_error_message >() );
            break;

        // OpenCL base_messages [20-128)
        case msgGetPlatformIDs:
        case msgGetPlatformInfo:
            throw dcl::library_exception( "Not implemented" );
            break;

        case msgGetDeviceIDs:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgGetDeviceIDs >() );
            break;

        case msgGetDeviceInfo:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgGetDeviceInfo >() );
            break;

        case msgCreateContext:
            throw dcl::library_exception( "Not implemented" );
            break;

        case msgCreateContextFromType:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgCreateContextFromType >() );
            break;

        case msgRetainContext:
        case msgReleaseContext:
            throw dcl::library_exception( "Not implemented" );
            break;

        case msgGetContextInfo:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgGetContextInfo >() );
            break;

        case msgCreateCommandQueue:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgCreateCommandQueue >() );
            break;

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
            throw dcl::library_exception( "Not implemented" );
            break;

        case msgCreateProgramWithSource:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgCreateProgramWithSource >() );
            break;

        case msgCreateProgramWithBinary:
        case msgRetainProgram:
        case msgReleaseProgram:
            throw dcl::library_exception( "Not implemented" );
            break;

        case msgBuildProgram:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgBuildProgram >() );
            break;

        case msgUnloadCompiler:
        case msgGetProgramInfo:
        case msgGetProgramBuildInfo:
            throw dcl::library_exception( "Not implemented" );
            break;

        case msgCreateKernel:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgCreateKernel >() );
            break;

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
            throw dcl::library_exception( "Not implemented" );
            break;

        case msgFinish:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgFinish >() );
            break;

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
            throw dcl::library_exception( "Not implemented" );
            break;

        case msgEnqueueNDRangeKernel:
            ret_ptr = reinterpret_cast< base_message* >( new dcl_message< msgEnqueueNDRangeKernel >() );
            break;

        case msgEnqueueTask:
        case msgEnqueueNativeKernel:
        case msgEnqueueMarker:
        case msgEnqueueWaitForEvents:
        case msgEnqueueBarrier:

        // OpenCL Extension base_messages [128-255]
        case msgExtension0:
        case msgExtension1:
            throw dcl::library_exception( "Not implemented" );
            break;

        default: 
            throw dcl::library_exception( "Not implemented" );
    }

    ret_ptr->set_buffer( msg_buffer_ptr, length );

    return ret_ptr;
}

#undef THROW_IF
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
