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
#include "msg_memory.h"
#include "msg_event.h"
#include "network/server_session.h"
#include "server/server_platform.h"
#include <boost/interprocess/sync/scoped_lock.hpp>
using dcl::network::server::server_session_context;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
uint16_t base_message::next_id_ = 0;
boost::interprocess::interprocess_mutex base_message::mutex_;
//-----------------------------------------------------------------------------
base_message::base_message( message_type type, bool wait_response,
                            std::size_t request_size, std::size_t response_size ) :
        wait_response_( wait_response ), buffer_ptr_( NULL ),
        size_( request_size + sizeof( message_header ) ), type_( type ),
        request_size_( request_size ), response_size_( response_size ), response_( false )
{
    scoped_lock_t lock( mutex_ );

    message_id_ = next_id_++;
}
//-----------------------------------------------------------------------------
base_message::base_message( const base_message& copy ) :
        wait_response_( copy.wait_response_ ), buffer_ptr_( copy.buffer_ptr_ ),
        size_( copy.size_ ), type_( copy.type_ ), request_size_( copy.request_size_ ),
        response_size_( copy.response_size_ ), response_( copy.response_ )
{
    message_id_ = copy.message_id_;
}
//-----------------------------------------------------------------------------
#define THROW_IF(b,ex) if(b) throw dcl::library_exception(ex)
#define MSG( x ) case x: ret_ptr=reinterpret_cast<base_message*>(new dcl_message<x>());break
#define MSG_RELEASE( x ) case x: ret_ptr=reinterpret_cast<base_message*>(new release_message<x>());break
#define MSG_NOT_IMPLEMENTED( x ) case x: throw dcl::library_exception("parse_message: " #x " not implemented");break

base_message* base_message::parse_message( uint8_t* msg_buffer_ptr, std::size_t length, bool is_request )
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
        MSG_NOT_IMPLEMENTED( msg_invalid_message );
        MSG( msg_error_message );
        MSG( msg_flush_server );
        MSG( msg_dummy_message );

        // OpenCL base_messages [20-128)
        
        // Platform
        MSG_NOT_IMPLEMENTED( msgGetPlatformIDs );
        MSG_NOT_IMPLEMENTED( msgGetPlatformInfo );

        // Device
        MSG( msgGetDeviceIDs );
        MSG( msgGetDeviceInfo );

        // Context
        MSG( msgCreateContext );
        MSG( msgCreateContextFromType );
        MSG_NOT_IMPLEMENTED( msgRetainContext );
        MSG_RELEASE( msgReleaseContext );
        MSG( msgGetContextInfo );

        // Command queue
        MSG( msgCreateCommandQueue );
        MSG_NOT_IMPLEMENTED( msgRetainCommandQueue );
        MSG_RELEASE( msgReleaseCommandQueue );
        MSG_NOT_IMPLEMENTED( msgGetCommandQueueInfo );
        MSG_NOT_IMPLEMENTED( msgSetCommandQueueProperty );

        // Memory object
        MSG( msgCreateBuffer );
        MSG( msgCreateImage2D );
        MSG_NOT_IMPLEMENTED( msgCreateImage3D );
        MSG_NOT_IMPLEMENTED( msgRetainMemObject );
        MSG_RELEASE( msgReleaseMemObject );
        MSG_NOT_IMPLEMENTED( msgGetSupportedImageFormats );
        MSG_NOT_IMPLEMENTED( msgGetMemObjectInfo );
        MSG_NOT_IMPLEMENTED( msgGetImageInfo );

        // Sampler
        MSG_NOT_IMPLEMENTED( msgCreateSampler );
        MSG_NOT_IMPLEMENTED( msgRetainSampler );
        MSG_NOT_IMPLEMENTED( msgReleaseSampler );
        MSG_NOT_IMPLEMENTED( msgGetSamplerInfo );

        // Program
        MSG( msgCreateProgramWithSource );
        MSG_NOT_IMPLEMENTED( msgCreateProgramWithBinary );
        MSG_NOT_IMPLEMENTED( msgRetainProgram );
        MSG_RELEASE( msgReleaseProgram );
        MSG( msgBuildProgram );
        MSG_NOT_IMPLEMENTED( msgUnloadCompiler );
        MSG_NOT_IMPLEMENTED( msgGetProgramInfo );
        MSG( msgGetProgramBuildInfo );

        // Kernel
        MSG( msgCreateKernel );
        MSG_NOT_IMPLEMENTED( msgCreateKernelsInProgram );
        MSG_NOT_IMPLEMENTED( msgRetainKernel );
        MSG_RELEASE( msgReleaseKernel );
        MSG( msgSetKernelArg );
        MSG_NOT_IMPLEMENTED( msgGetKernelInfo );
        MSG( msgGetKernelWorkGroupInfo );

        // Event
        MSG( msgWaitForEvents );
        MSG_NOT_IMPLEMENTED( msgGetEventInfo );
        MSG_NOT_IMPLEMENTED( msgRetainEvent );
        MSG_RELEASE( msgReleaseEvent );
        MSG( msgGetEventProfilingInfo );

        // Enqueue
        MSG( msgFlush );
        MSG( msgFinish );

        MSG( msgEnqueueReadBuffer );
        MSG( msgEnqueueWriteBuffer );
        MSG_NOT_IMPLEMENTED( msgEnqueueCopyBuffer );
        MSG_NOT_IMPLEMENTED( msgEnqueueReadImage );
        MSG_NOT_IMPLEMENTED( msgEnqueueWriteImage );
        MSG_NOT_IMPLEMENTED( msgEnqueueCopyImage );
        MSG_NOT_IMPLEMENTED( msgEnqueueCopyImageToBuffer );
        MSG_NOT_IMPLEMENTED( msgEnqueueCopyBufferToImage );
        MSG_NOT_IMPLEMENTED( msgEnqueueMapBuffer );
        MSG_NOT_IMPLEMENTED( msgEnqueueMapImage );
        MSG_NOT_IMPLEMENTED( msgEnqueueUnmapMemObject );
        MSG( msgEnqueueNDRangeKernel );
        MSG_NOT_IMPLEMENTED( msgEnqueueTask );
        MSG_NOT_IMPLEMENTED( msgEnqueueNativeKernel );
        MSG_NOT_IMPLEMENTED( msgEnqueueMarker );
        MSG_NOT_IMPLEMENTED( msgEnqueueWaitForEvents );
        MSG_NOT_IMPLEMENTED( msgEnqueueBarrier );

        // OpenCL Extension base_messages [128-255]
        //MSG_NOT_IMPLEMENTED( msgExtension0 );
        //MSG_NOT_IMPLEMENTED( msgExtension1 );

        default:
            throw dcl::library_exception( "Unkown message" );
    }

    ret_ptr->set_received_buffer( msg_buffer_ptr, message_len, is_request );

    return ret_ptr;
}

#undef MSG_NOT_IMPLEMENTED
#undef MSG
#undef THROW_IF
//-----------------------------------------------------------------------------
// enqueue_message
//-----------------------------------------------------------------------------
using dcl::info::object_manager;
using dcl::info::generic_event;
//-----------------------------------------------------------------------------
object_manager< generic_event > enqueue_message::remote_event_ids_;
//-----------------------------------------------------------------------------
void* enqueue_message::create_enqueue_request( void* payload_ptr )
{
    enqueue_message_request* request_ptr =
        reinterpret_cast< enqueue_message_request* >( payload_ptr );

    request_ptr->blocking_ = static_cast<uint16_t>( blocking_? 1 : 0 );
    request_ptr->return_event_ = static_cast<uint16_t>( return_event_? 1 : 0 );
    request_ptr->event_count_ = static_cast<uint16_t>( events_.size() );
    request_ptr->command_queue_id_ = host_to_network( command_queue_id_ );

    request_ptr->events_[ 0 ] = host_to_network( event_id_ );

    for( uint32_t i = 1; i <= events_.size(); i++ )
    {
        request_ptr->events_[ i ] = host_to_network( events_[ i - 1 ] );
    }

    return( reinterpret_cast<uint8_t*>( payload_ptr ) + get_enqueue_request_size() );
}
//-----------------------------------------------------------------------------
const void* enqueue_message::parse_enqueue_request( const void* payload_ptr )
{
    const enqueue_message_request* request_ptr =
        reinterpret_cast< const enqueue_message_request* >( payload_ptr );

    blocking_ = (request_ptr->blocking_ == 1)? true : false;
    return_event_ = (request_ptr->return_event_ == 1)? true : false;
    command_queue_id_ = network_to_host( request_ptr->command_queue_id_ );

    event_id_ = network_to_host( request_ptr->events_[ 0 ] );

    //if( return_event_ )
    //    set_wait_response();

    events_.clear();
    uint32_t event_count = request_ptr->event_count_;

    if( event_count != 0 )
    {
        events_.reserve( event_count );

        for( uint32_t i = 1; i <= event_count; i++ )
        {
            events_.push_back( network_to_host( request_ptr->events_[ i ] ) );
        }
    }

    return( reinterpret_cast<const uint8_t*>( payload_ptr ) + get_enqueue_request_size() );
}
//-----------------------------------------------------------------------------
void enqueue_message::server_wait( server_session_context* session_context_ptr )
{
    //if( blocking_ )
    //{
    //    session_context_ptr->get_server_platform().wait( command_queue_id_ );
    //}

    if( event_ptr_ != NULL )
    {
        event_ptr_->wait();
    }
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
