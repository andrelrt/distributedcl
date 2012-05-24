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
#ifndef _DCL_MESSAGE_H_
#define _DCL_MESSAGE_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "info/object_manager.h"
#include "info/event_info.h"
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
//-----------------------------------------------------------------------------
#define MSG_PARAMETER_GET( type_t, var_att, name_m ) \
    inline const type_t get_##name_m() const{return var_att;}

#define MSG_PARAMETER_SET( type_t, var_att, name_m ) \
    inline void set_##name_m( const type_t copy_obj ){ var_att = copy_obj; }

#define MSG_PARAMETER_GET_SET( type_t, var_att, name_m ) \
    MSG_PARAMETER_GET( type_t, var_att, name_m )\
    MSG_PARAMETER_SET( type_t, var_att, name_m )
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace server {
class server_session_context;
}}}
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
enum message_type
{
    msg_invalid_message = 0,

    // Internal messages [1-20)
    msg_error_message = 1,
    msg_flush_server = 2,
    msg_dummy_message = 3,
    msg_get_context = 4,
    msg_attach_context = 5,

    // OpenCL messages [20-128)
    msgGetPlatformIDs           = 20,
    msgGetPlatformInfo          = 21,

    msgGetDeviceIDs             = 22,
    msgGetDeviceInfo            = 23,

    msgCreateContext            = 24,
    msgCreateContextFromType    = 25,
    msgRetainContext            = 26,
    msgReleaseContext           = 27,
    msgGetContextInfo           = 28,

    msgCreateCommandQueue       = 29,
    msgRetainCommandQueue       = 30,
    msgReleaseCommandQueue      = 31,
    msgGetCommandQueueInfo      = 32,
    msgSetCommandQueueProperty  = 33,

    msgCreateBuffer             = 34,
    msgCreateImage2D            = 35,
    msgCreateImage3D            = 37,
    msgRetainMemObject          = 38,
    msgReleaseMemObject         = 39,
    msgGetSupportedImageFormats = 40,
    msgGetMemObjectInfo         = 41,
    msgGetImageInfo             = 42,

    msgCreateSampler            = 43,
    msgRetainSampler            = 44,
    msgReleaseSampler           = 45,
    msgGetSamplerInfo           = 46,

    msgCreateProgramWithSource  = 47,
    msgCreateProgramWithBinary  = 48,
    msgRetainProgram            = 49,
    msgReleaseProgram           = 50,
    msgBuildProgram             = 51,
    msgUnloadCompiler           = 52,
    msgGetProgramInfo           = 53,
    msgGetProgramBuildInfo      = 54,

    msgCreateKernel             = 55,
    msgCreateKernelsInProgram   = 56,
    msgRetainKernel             = 57,
    msgReleaseKernel            = 58,
    msgSetKernelArg             = 59,
    msgGetKernelInfo            = 60,
    msgGetKernelWorkGroupInfo   = 61,

    msgWaitForEvents            = 62,
    msgGetEventInfo             = 63,
    msgRetainEvent              = 64,
    msgReleaseEvent             = 65,
    msgGetEventProfilingInfo    = 66,

    msgFlush                    = 67,
    msgFinish                   = 68,

    msgEnqueueReadBuffer        = 69,
    msgEnqueueWriteBuffer       = 70,
    msgEnqueueCopyBuffer        = 71,
    msgEnqueueReadImage         = 72,
    msgEnqueueWriteImage        = 73,
    msgEnqueueCopyImage         = 74,
    msgEnqueueCopyImageToBuffer = 75,
    msgEnqueueCopyBufferToImage = 76,
    msgEnqueueMapBuffer         = 77,
    msgEnqueueMapImage          = 78,
    msgEnqueueUnmapMemObject    = 79,
    msgEnqueueNDRangeKernel     = 80,
    msgEnqueueTask              = 81,
    msgEnqueueNativeKernel      = 82,
    msgEnqueueMarker            = 83,
    msgEnqueueWaitForEvents     = 84,
    msgEnqueueBarrier           = 85,

    // OpenCL Extension messages [128-256)
    msgExtension0 = 128,
    msgExtension1 = 129,
};
//-----------------------------------------------------------------------------
class base_message
{
public:
    virtual ~base_message(){}

    static base_message* parse_message( uint8_t* msg_buffer_ptr, std::size_t length, bool is_request );

    inline virtual bool is_async() const
    {
        return false;
    }

    inline message_type get_type() const
    {
        return type_;
    }

    void set_response_mode()
    {
        response_ = true;
        wait_response_ = false;
        set_size( response_size_ );
    }

    inline void get_buffer( uint8_t* buffer_ptr )
    {
        buffer_ptr_ = buffer_ptr;

        message_header* header = reinterpret_cast< message_header* >( buffer_ptr_ );
        header->version = message_v1_0;
        header->type = type_;
        header->request = host_to_network( static_cast<uint16_t>( response_ ? 0 : 1 ) );
        header->id = host_to_network( message_id_ );
        header->length = host_to_network( static_cast< uint32_t >( size_ ) );

        if( !response_ )
        {
            create_request( buffer_ptr_ + sizeof( message_header ) );
        }
        else
        {
            create_response( buffer_ptr_ + sizeof( message_header ) );
        }
    }

    inline std::size_t get_size() const
    {
        return size_;
    }

    inline bool waiting_response()
    {
        return wait_response_;
    }

    inline const void* get_payload() const
    {
        return buffer_ptr_ + sizeof( message_header );
    }

    inline uint16_t get_id() const
    {
        return message_id_;
    }

    virtual void parse_response( const void* payload_ptr ){} //, uint32_t size

    virtual void server_wait( dcl::network::server::server_session_context* session_context_ptr ){}

protected:
    base_message( message_type type, bool wait_response = false,
                  std::size_t request_size = 0, std::size_t response_size = 0 );

    base_message( const base_message& copy );

    virtual void create_request( void* payload_ptr ){}
    virtual void create_response( void* payload_ptr ){}
    virtual void parse_request( const void* payload_ptr ){}

    inline virtual void set_size( std::size_t size )
    {
        size_ = size + sizeof( message_header );
    }

    inline bool is_request() const
    {
        return (reinterpret_cast< message_header* >( buffer_ptr_ )->request == 1);
    }

    inline uint32_t get_payload_size() const
    {
        return static_cast< uint32_t >( size_ - sizeof( message_header ) );
    }

    inline void set_wait_response()
    {
        wait_response_ = true;
    }

    inline virtual void set_response_size( std::size_t size )
    {
        response_size_ = size;
    }

private:
    uint16_t message_id_;
    bool wait_response_;
    uint8_t* buffer_ptr_;
    std::size_t size_;
    message_type type_;
    std::size_t request_size_;
    std::size_t response_size_;
    bool response_;

    static uint16_t next_id_;
    static boost::interprocess::interprocess_mutex mutex_;

    enum{ message_v1_0 = 0x10 };

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct message_header
    {
        uint8_t version;
        uint8_t type : 7;
        uint8_t request : 1;
        uint16_t id;
        uint32_t length;
    };
    #pragma pack( pop )

    inline void set_received_buffer( uint8_t* msg_buffer_ptr, std::size_t size, bool is_request )
    {
        buffer_ptr_ = msg_buffer_ptr;
        size_ = size;

        const message_header* header_ptr = reinterpret_cast< const message_header* >( msg_buffer_ptr );
        message_id_ = network_to_host( header_ptr->id );

        if( is_request )
            parse_request( get_payload() );
        //else
        //    parse_response( get_payload() );
    }
};
//-----------------------------------------------------------------------------
class enqueue_message
    : public base_message
{
public:
    inline virtual bool is_async() const
    {
        return true;
    }

    // Request
    MSG_PARAMETER_GET_SET( bool, blocking_, blocking )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, command_queue_id_, command_queue_id )

    MSG_PARAMETER_GET( dcl::remote_ids_t, events_, events )
    MSG_PARAMETER_GET( bool, return_event_, return_event )
    MSG_PARAMETER_GET( dcl::remote_id_t, event_id_, event_id )
    
    inline void add_event( dcl::remote_id_t event_id )
    {
        events_.push_back( event_id );

        update_request_size();
    }

    inline void set_event_id( dcl::remote_id_t event_id )
    {
        event_id_ = event_id;
        return_event_ = true;
    }

    inline dcl::remote_id_t get_event_id( dcl::info::generic_event* event_ptr )
    {
        event_id_ = remote_event_ids_.get( event_ptr, true );
        return_event_ = true;

        return event_id_;
    }

    inline void set_server_event( dcl::info::generic_event* event_ptr )
    {
        event_ptr_ = event_ptr;
    }

    virtual void server_wait( dcl::network::server::server_session_context* session_context_ptr );

    virtual ~enqueue_message()
    {
        if( !return_event_ )
        {
            delete event_ptr_;
        }
    }

protected:
    bool blocking_;
    bool return_event_;
    dcl::remote_ids_t events_;
    dcl::remote_id_t command_queue_id_;

    dcl::remote_id_t event_id_;
    dcl::info::generic_event* event_ptr_;

    static dcl::info::object_manager< dcl::info::generic_event > remote_event_ids_;

    enqueue_message( message_type type, bool wait_response = false,
                     std::size_t request_size = 0, std::size_t response_size = 0 ) :
        base_message( type, wait_response,
                      request_size + sizeof(enqueue_message_request),
                      response_size ),
        blocking_( false ), return_event_( false ), command_queue_id_( 0xffff ),
        event_id_( 0xffff ), event_ptr_( NULL ){}

    inline std::size_t get_enqueue_request_size()
    {
        return( sizeof(enqueue_message_request) + 
                events_.size() * sizeof(dcl::remote_id_t) );
    }

    virtual void update_request_size() = 0;

    virtual void* create_enqueue_request( void* payload_ptr );
    virtual const void* parse_enqueue_request( const void* payload_ptr );

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct enqueue_message_request
    {
        uint16_t event_count_:14;
        uint16_t return_event_:1;
        uint16_t blocking_:1;
        dcl::remote_id_t command_queue_id_;
        dcl::remote_id_t events_[ 1 ];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
template< message_type MESSAGE_NUMBER >
class dcl_message : public base_message{};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_MESSAGE_H_
