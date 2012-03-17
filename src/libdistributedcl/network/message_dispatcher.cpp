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
#include "message_dispatcher.h"
#include "message/message.h"
#include "server/server_platform.h"
#include "server/server_context.h"
#include "server/server_program.h"
#include "server/server_kernel.h"
#include "server/server_command_queue.h"
#include "server/server_memory.h"
using namespace dcl::network::message;
using namespace dcl::server;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace server {
//-----------------------------------------------------------------------------
void message_dispatcher::dispatch_messages( message_vector_t& messages )
{
    message_vector_t::iterator it;

    for( it = messages.begin(); it != messages.end(); it++ )
    {
        switch( (*it)->get_type() )
        {
            // TODO: Create the base_message objects

            // Internal base_messages [1-20)
            case msg_invalid_message:
            case msg_error_message:
                throw dcl::library_exception( "Not implemented" );
                break;

            // OpenCL base_messages [20-128)
            case msgGetPlatformIDs:
            case msgGetPlatformInfo:
                throw dcl::library_exception( "Not implemented" );
                break;

            case msgGetDeviceIDs:
            {
                GetDeviceIDs_command command( *it );

                command.execute();
                break;
            }

            case msgGetDeviceInfo:
            {
                GetDeviceInfo_command command( *it );

                command.execute();
                break;
            }

            case msgCreateContext:
                throw dcl::library_exception( "Not implemented" );
                break;

            case msgCreateContextFromType:
            {
                CreateContextFromType_command command( *it );

                command.execute();
                break;
            }

            case msgRetainContext:
            case msgReleaseContext:
                throw dcl::library_exception( "Not implemented" );
                break;

            case msgGetContextInfo:
            {
                GetContextInfo_command command( *it );

                command.execute();
                break;
            }

            case msgCreateCommandQueue:
            {
                CreateCommandQueue_command command( *it );

                command.execute();
                break;
            }

            case msgRetainCommandQueue:
            case msgReleaseCommandQueue:
            case msgGetCommandQueueInfo:
            case msgSetCommandQueueProperty:
                throw dcl::library_exception( "Not implemented" );
                break;

            case msgCreateBuffer:
            {
                CreateBuffer_command command( *it );

                command.execute();
                break;
            }

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
            {
                CreateProgramWithSource_command command( *it );

                command.execute();
                break;
            }

            case msgCreateProgramWithBinary:
            case msgRetainProgram:
            case msgReleaseProgram:
                throw dcl::library_exception( "Not implemented" );
                break;

            case msgBuildProgram:
            {
                BuildProgram_command command( *it );

                command.execute();
                break;
            }

            case msgUnloadCompiler:
            case msgGetProgramInfo:
                throw dcl::library_exception( "Not implemented" );
                break;

            case msgGetProgramBuildInfo:
            {
                GetProgramBuildInfo_command command( *it );

                command.execute();
                break;
            }

            case msgCreateKernel:
            {
                CreateKernel_command command( *it );

                command.execute();
                break;
            }

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
            {
                Finish_command command( *it );

                command.execute();
                break;
            }

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
            {
                EnqueueNDRangeKernel_command command( *it );

                command.execute();
                break;
            }

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
    }
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::server
//-----------------------------------------------------------------------------
