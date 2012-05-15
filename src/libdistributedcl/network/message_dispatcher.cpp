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
#include <iostream>
#include "message_dispatcher.h"
#include "server_session.h"
#include "message/message.h"
#include "server/server_command.h"
#include "server/server_platform.h"
#include "server/server_context.h"
#include "server/server_program.h"
#include "server/server_kernel.h"
#include "server/server_command_queue.h"
#include "server/server_memory.h"
#include "server/server_event.h"
using namespace dcl::network::message;
using namespace dcl::server;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace server {
//-----------------------------------------------------------------------------
//#define MSG_DEBUG
#if defined MSG_DEBUG
#define MSG( x ) case x: {x##_command command(*it);std::cout<<"dispatch message " #x "...";command.execute();std::cout<<"... Ok"<<std::endl;}break
#define MSG_ASYNC( x ) case x: {boost::shared_ptr<x##_command>command_sp(new x##_command(*it,waiting_messages_ptr));std::cout<<"async dispatch message " #x "..."<<std::endl;command_sp->async_execute(command_sp);}break
#define MSG_IGNORE( x ) case x: std::cout<<"ignoring message " #x "..."<<std::endl;break
#else
#define MSG( x ) case x: {x##_command command(*it);command.execute();}break
#define MSG_ASYNC( x ) case x: {boost::shared_ptr<x##_command>command_sp(new x##_command(*it,waiting_messages_ptr));command_sp->async_execute(command_sp);}break
#define MSG_IGNORE( x ) case x: break
#endif
#define MSG_NOT_IMPLEMENTED( x ) case x: throw dcl::library_exception("dispatch_messages: " #x " not implemented");break
//-----------------------------------------------------------------------------
void message_dispatcher::dispatch_messages( message_vector_t& messages, server_messages* waiting_messages_ptr )
{
    message_vector_t::iterator it;

    for( it = messages.begin(); it != messages.end(); it++ )
    {
        switch( (*it)->get_type() )
        {
            // Internal base_messages [1-20)
            MSG_NOT_IMPLEMENTED( msg_invalid_message );
            MSG_IGNORE( msg_error_message );
            MSG( msg_flush_server );

            // OpenCL base_messages [20-128)
            MSG_NOT_IMPLEMENTED( msgGetPlatformIDs );
            MSG_NOT_IMPLEMENTED( msgGetPlatformInfo );

            MSG( msgGetDeviceIDs );

            MSG( msgGetDeviceInfo );
            MSG( msgCreateContext );
            MSG( msgCreateContextFromType );

            MSG_NOT_IMPLEMENTED( msgRetainContext );
            MSG_NOT_IMPLEMENTED( msgReleaseContext );

            MSG( msgGetContextInfo );
            MSG( msgCreateCommandQueue );

            MSG_NOT_IMPLEMENTED( msgRetainCommandQueue );
            MSG_NOT_IMPLEMENTED( msgReleaseCommandQueue );
            MSG_NOT_IMPLEMENTED( msgGetCommandQueueInfo );
            MSG_NOT_IMPLEMENTED( msgSetCommandQueueProperty );

            MSG( msgCreateBuffer );
            MSG( msgCreateImage2D );

            MSG_NOT_IMPLEMENTED( msgCreateImage3D );
            MSG_NOT_IMPLEMENTED( msgRetainMemObject );
            MSG_NOT_IMPLEMENTED( msgReleaseMemObject );
            MSG_NOT_IMPLEMENTED( msgGetSupportedImageFormats );
            MSG_NOT_IMPLEMENTED( msgGetMemObjectInfo );
            MSG_NOT_IMPLEMENTED( msgGetImageInfo );
            MSG_NOT_IMPLEMENTED( msgCreateSampler );
            MSG_NOT_IMPLEMENTED( msgRetainSampler );
            MSG_NOT_IMPLEMENTED( msgReleaseSampler );
            MSG_NOT_IMPLEMENTED( msgGetSamplerInfo );

            MSG( msgCreateProgramWithSource );

            MSG_NOT_IMPLEMENTED( msgCreateProgramWithBinary );
            MSG_NOT_IMPLEMENTED( msgRetainProgram );
            MSG_NOT_IMPLEMENTED( msgReleaseProgram );

            MSG( msgBuildProgram );

            MSG_NOT_IMPLEMENTED( msgUnloadCompiler );
            MSG_NOT_IMPLEMENTED( msgGetProgramInfo );

            MSG( msgGetProgramBuildInfo );

            MSG( msgCreateKernel );

            MSG_NOT_IMPLEMENTED( msgCreateKernelsInProgram );
            MSG_NOT_IMPLEMENTED( msgRetainKernel );
            MSG_NOT_IMPLEMENTED( msgReleaseKernel );

            MSG( msgSetKernelArg );

            MSG_NOT_IMPLEMENTED( msgGetKernelInfo );

            MSG( msgGetKernelWorkGroupInfo );

            MSG( msgWaitForEvents );

            MSG_NOT_IMPLEMENTED( msgGetEventInfo );
            MSG_NOT_IMPLEMENTED( msgRetainEvent );
            MSG_NOT_IMPLEMENTED( msgReleaseEvent );
            MSG_NOT_IMPLEMENTED( msgGetEventProfilingInfo );

            MSG( msgFlush );
            MSG( msgFinish );
            MSG_ASYNC( msgEnqueueReadBuffer );
            MSG_ASYNC( msgEnqueueWriteBuffer );

            MSG_NOT_IMPLEMENTED( msgEnqueueCopyBuffer );
            MSG_NOT_IMPLEMENTED( msgEnqueueReadImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueWriteImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueCopyImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueCopyImageToBuffer );
            MSG_NOT_IMPLEMENTED( msgEnqueueCopyBufferToImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueMapBuffer );
            MSG_NOT_IMPLEMENTED( msgEnqueueMapImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueUnmapMemObject );

            MSG_ASYNC( msgEnqueueNDRangeKernel );

            MSG_NOT_IMPLEMENTED( msgEnqueueTask );
            MSG_NOT_IMPLEMENTED( msgEnqueueNativeKernel );
            MSG_NOT_IMPLEMENTED( msgEnqueueMarker );
            MSG_NOT_IMPLEMENTED( msgEnqueueWaitForEvents );
            MSG_NOT_IMPLEMENTED( msgEnqueueBarrier );

            // OpenCL Extension base_messages [128-255]
            MSG_NOT_IMPLEMENTED( msgExtension0 );
            MSG_NOT_IMPLEMENTED( msgExtension1 );

            default: 
                throw dcl::library_exception( "dispatch_messages: Unknow Message" );
        }
    }
}
//-----------------------------------------------------------------------------
#undef MSG_NOT_IMPLEMENTED
#undef MSG_ASYNC
#undef MSG
//-----------------------------------------------------------------------------
void message_dispatcher::flush_async_queue()
{
	async_server::get_instance().flush_queue();
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::server
//-----------------------------------------------------------------------------
