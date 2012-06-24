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
#include "server_command.h"
#include "server_platform.h"
#include "server_context.h"
#include "server_program.h"
#include "server_kernel.h"
#include "server_command_queue.h"
#include "server_memory.h"
#include "server_event.h"
#include "server_sampler.h"
#include "network/server_session.h"
#include "message/message.h"
#include "message/msg_kernel.h"
#include "message/msg_memory.h"
#include "message/msg_event.h"
#include "composite/composite_context.h"
#include "composite/composite_command_queue.h"
#include "composite/composite_memory.h"
#include "composite/composite_program.h"
#include "composite/composite_kernel.h"
#include "composite/composite_event.h"
#include "composite/composite_sampler.h"
using namespace dcl::network::message;
using namespace dcl::composite;
using dcl::network::server::server_session_context;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
#define MSG_DEBUG
#if defined MSG_DEBUG
#define MSG( x ) case x: {x##_command command(*it,session_context_ptr);std::cout<<"dispatch message " #x "...";command.execute();std::cout<<"... Ok"<<std::endl;}break
#define MSG_ASYNC( x )\
case x:\
{\
    boost::shared_ptr<x##_command> command_sp(new x##_command(*it,session_context_ptr));\
    std::cout<<"async dispatch message " #x "..."<<std::endl;\
    command_sp->async_execute(command_sp,boost::shared_static_cast<dcl::network::message::dcl_message<x> >(*it)->get_command_queue_id());\
}\
break
#define MSG_RELEASE( x, y, d ) case x: {boost::shared_ptr<release_command<x,d> >command_sp(new release_command<x,d>(*it,y,session_context_ptr));std::cout<<"sync dispatch message " #x "..."<<std::endl;command_sp->execute();}break
#define MSG_IGNORE( x ) case x: std::cout<<"ignoring message " #x "..."<<std::endl;break
#else
#define MSG( x ) case x: {x##_command command(*it,session_context_ptr);command.execute();}break
#define MSG_ASYNC( x )\
case x:\
{\
    boost::shared_ptr<x##_command> command_sp(new x##_command(*it,session_context_ptr));\
    command_sp->async_execute(command_sp,boost::shared_static_cast<dcl::network::message::dcl_message<x> >(*it)->get_command_queue_id());\
}\
break
#define MSG_RELEASE( x, y, d ) case x: {boost::shared_ptr<release_command<x,d> >command_sp(new release_command<x,d>(*it,y,session_context_ptr));command_sp->execute();}break
#define MSG_IGNORE( x ) case x: break
#endif
#define MSG_NOT_IMPLEMENTED( x ) case x: throw dcl::library_exception("dispatch_messages: " #x " not implemented");break
//-----------------------------------------------------------------------------
void message_dispatcher::dispatch_messages( message_vector_t& messages, server_session_context* session_context_ptr )
{
    message_vector_t::iterator it;
    server_platform& server = session_context_ptr->get_server_platform();

    for( it = messages.begin(); it != messages.end(); it++ )
    {
        switch( (*it)->get_type() )
        {
            // Internal base_messages [1-20)
            MSG_NOT_IMPLEMENTED( msg_invalid_message );
            MSG_IGNORE( msg_error_message );
            MSG( msg_flush_server );
            MSG_IGNORE( msg_dummy_message );
            MSG( msg_get_context );
            MSG( msg_attach_context );

            // OpenCL base_messages [20-128)
            MSG_NOT_IMPLEMENTED( msgGetPlatformIDs );
            MSG_NOT_IMPLEMENTED( msgGetPlatformInfo );

            // Device
            MSG( msgGetDeviceIDs );
            MSG( msgGetDeviceInfo );

            // Context
            MSG( msgCreateContext );
            MSG( msgCreateContextFromType );
            MSG_NOT_IMPLEMENTED( msgRetainContext );
            MSG_RELEASE( msgReleaseContext, server.get_context_manager(), composite_context );
            MSG( msgGetContextInfo );

            // Command queue
            MSG( msgCreateCommandQueue );
            MSG_NOT_IMPLEMENTED( msgRetainCommandQueue );
            MSG_RELEASE( msgReleaseCommandQueue, server.get_command_queue_manager(), composite_command_queue );
            MSG_NOT_IMPLEMENTED( msgGetCommandQueueInfo );
            MSG_NOT_IMPLEMENTED( msgSetCommandQueueProperty );

            // Memory object
            MSG( msgCreateBuffer );
            MSG( msgCreateImage2D );
            MSG_NOT_IMPLEMENTED( msgCreateImage3D );
            MSG_NOT_IMPLEMENTED( msgRetainMemObject );
            MSG_RELEASE( msgReleaseMemObject, server.get_memory_manager(), composite_memory );
            MSG_NOT_IMPLEMENTED( msgGetSupportedImageFormats );
            MSG_NOT_IMPLEMENTED( msgGetMemObjectInfo );
            MSG_NOT_IMPLEMENTED( msgGetImageInfo );

            // Sampler
            MSG( msgCreateSampler );
            MSG_NOT_IMPLEMENTED( msgRetainSampler );
            MSG_RELEASE( msgReleaseSampler, server.get_sampler_manager(), composite_sampler );
            MSG_NOT_IMPLEMENTED( msgGetSamplerInfo );

            // Program
            MSG( msgCreateProgramWithSource );
            MSG_NOT_IMPLEMENTED( msgCreateProgramWithBinary );
            MSG_NOT_IMPLEMENTED( msgRetainProgram );
            MSG_RELEASE( msgReleaseProgram, server.get_program_manager(), composite_program );
            MSG( msgBuildProgram );
            MSG_NOT_IMPLEMENTED( msgUnloadCompiler );
            MSG_NOT_IMPLEMENTED( msgGetProgramInfo );
            MSG( msgGetProgramBuildInfo );

            // Kernel
            MSG( msgCreateKernel );
            MSG_NOT_IMPLEMENTED( msgCreateKernelsInProgram );
            MSG_NOT_IMPLEMENTED( msgRetainKernel );
            MSG_RELEASE( msgReleaseKernel, server.get_kernel_manager(), composite_kernel );
            MSG( msgSetKernelArg );
            MSG_NOT_IMPLEMENTED( msgGetKernelInfo );
            MSG( msgGetKernelWorkGroupInfo );

            // Event
            MSG( msgWaitForEvents );
            MSG_NOT_IMPLEMENTED( msgGetEventInfo );
            MSG_NOT_IMPLEMENTED( msgRetainEvent );
            MSG_RELEASE( msgReleaseEvent, server.get_event_manager(), composite_event );
            MSG( msgGetEventProfilingInfo );

            // Enqueue
            MSG( msgFlush );
            MSG( msgFinish );
            MSG_ASYNC( msgEnqueueReadBuffer ); // async
            MSG_ASYNC( msgEnqueueWriteBuffer ); // async
            MSG_ASYNC( msgEnqueueCopyBuffer ); // async
            MSG_NOT_IMPLEMENTED( msgEnqueueReadImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueWriteImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueCopyImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueCopyImageToBuffer );
            MSG_NOT_IMPLEMENTED( msgEnqueueCopyBufferToImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueMapBuffer );
            MSG_NOT_IMPLEMENTED( msgEnqueueMapImage );
            MSG_NOT_IMPLEMENTED( msgEnqueueUnmapMemObject );
            MSG_ASYNC( msgEnqueueNDRangeKernel ); // async
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
#undef MSG_IGNORE
#undef MSG_RELEASE
#undef MSG_ASYNC
#undef MSG
//-----------------------------------------------------------------------------
void message_dispatcher::wait_messages( message_vector_t& messages, server_session_context* session_context_ptr )
{
    //session_context_ptr->get_server_platform().wait_unblock_all();
    //session_context_ptr->get_server_platform().flush_all();

    for( message_vector_t::iterator it = messages.begin(); it != messages.end(); it++ )
    {
        (*it)->server_wait( session_context_ptr );
    }
}
//-----------------------------------------------------------------------------
void message_dispatcher::flush_async_queue( server_session_context* session_context_ptr )
{
    session_context_ptr->get_server_platform().flush_all();
//	async_server::get_instance().flush_queue();
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
