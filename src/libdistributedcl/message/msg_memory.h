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
#ifndef _DCL_MEMORY_MESSAGES_H_
#define _DCL_MEMORY_MESSAGES_H_

#include "message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
    //msgCreateBuffer             = 34,
    //msgCreateImage2D            = 35,
    //msgCreateImage3D            = 37,
    //msgRetainMemObject          = 38,
    //msgReleaseMemObject         = 39,
    //msgGetSupportedImageFormats = 40,
    //msgGetMemObjectInfo         = 41,
    //msgGetImageInfo             = 42,
    //msgEnqueueReadBuffer        = 69,
    //msgEnqueueWriteBuffer       = 70,
    //msgEnqueueCopyBuffer        = 71,
    //msgEnqueueReadImage         = 72,
    //msgEnqueueWriteImage        = 73,
    //msgEnqueueCopyImage         = 74,
    //msgEnqueueCopyImageToBuffer = 75,
    //msgEnqueueCopyBufferToImage = 76,
    //msgEnqueueMapBuffer         = 77,
    //msgEnqueueMapImage          = 78,
    //msgEnqueueUnmapMemObject    = 79,
//-----------------------------------------------------------------------------
// msgCreateBuffer
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateBuffer > : public base_message
{
public:
    dcl_message< msgCreateBuffer >() :
        base_message( msgCreateBuffer, true, 0, sizeof( dcl::remote_id_t ) ),
        context_id_( 0xffff ), buffer_ptr_( NULL ), buffer_len_( 0 ), 
        flags_( 0 ), id_( 0xffff ){}

    typedef std::vector<uint8_t> buffer_t;

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, context_id_, context_id )
    MSG_PARAMETER_GET_SET( cl_mem_flags, flags_, flags )

    MSG_PARAMETER_GET( uint8_t*, buffer_ptr_, buffer_pointer )
    MSG_PARAMETER_GET( size_t, buffer_len_, buffer_size )

    inline void set_buffer( const uint8_t* ptr, size_t size )
    {
        buffer_ptr_ = ptr;
        buffer_len_ = size;

        update_request_size();
    }

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, id_, remote_id )

private:
    dcl::remote_id_t context_id_;
    const uint8_t* buffer_ptr_;
    size_t buffer_len_;
    cl_mem_flags flags_;
    buffer_t buffer_;

    dcl::remote_id_t id_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    inline void update_request_size()
    {
        set_size( (( buffer_ptr_ == NULL ) ? 0 : buffer_len_) + sizeof(msgCreateBuffer_request) - 1 );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateBuffer_request
    {
        dcl::remote_id_t context_id_;
        uint16_t message_buffer_:1;
        uint16_t flags_:15;
        uint32_t buffer_len_;

        uint8_t buffer_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
// msgEnqueueWriteBuffer
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgEnqueueWriteBuffer > : public base_message
{
public:
    dcl_message< msgEnqueueWriteBuffer >() :
        base_message( msgEnqueueWriteBuffer, false, 0, 0 ), id_( 0xffff ),
        command_queue_id_( 0xffff ), buffer_ptr_( NULL ), buffer_len_( 0 ),
        blocking_( false ), event_id_( 0xffff ){}

    typedef std::vector<uint8_t> buffer_t;

    // Request
    MSG_PARAMETER_GET_SET( bool, return_event_, return_event )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, id_, remote_id )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, command_queue_id_, command_queue_id )
    MSG_PARAMETER_GET_SET( bool, blocking_, blocking )

    MSG_PARAMETER_GET( uint8_t*, buffer_ptr_, buffer_pointer )
    MSG_PARAMETER_GET( size_t, buffer_len_, buffer_size )
    MSG_PARAMETER_GET( dcl::remote_ids_t, events_, events )

    inline void set_buffer( const uint8_t* ptr, size_t size, size_t offset )
    {
        buffer_ptr_ = ptr + offset;
        buffer_len_ = size;

        update_request_size();
    }

    inline void add_event( dcl::remote_id_t event_id )
    {
        events_.push_back( event_id );

        update_request_size();
    }

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, event_id_, event_id )

private:
    dcl::remote_id_t id_;
    dcl::remote_id_t command_queue_id_;
    const uint8_t* buffer_ptr_;
    size_t buffer_len_;
    buffer_t buffer_;
    bool blocking_;
    bool return_event_;
    dcl::remote_ids_t events_;

    dcl::remote_id_t event_id_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    inline void update_request_size()
    {
        set_size( buffer_len_ +
                  events_.size() * sizeof(dcl::remote_id_t) +
                  sizeof(msgEnqueueWriteBuffer_request) - 1 );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgEnqueueWriteBuffer_request
    {
        dcl::remote_id_t id_;
        dcl::remote_id_t command_queue_id_;
        uint16_t blocking_:1;
        uint16_t return_event_:1;
        uint16_t event_count_:14;
        uint32_t buffer_len_;

        uint8_t buffer_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
// msgEnqueueReadBuffer
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgEnqueueReadBuffer > : public base_message
{
public:
    dcl_message< msgEnqueueReadBuffer >() :
        base_message( msgEnqueueReadBuffer, true,
                      sizeof( msgEnqueueReadBuffer_request ) - sizeof( dcl::remote_id_t ),
                      sizeof( dcl::remote_id_t ) ),
        id_( 0xffff ), command_queue_id_( 0xffff ), size_( 0 ), offset_( 0 ),
        blocking_( false ), return_event_( false ), event_id_( 0xffff ){}

    typedef std::vector<uint8_t> buffer_t;

    // Request
    MSG_PARAMETER_GET_SET( bool, return_event_, return_event )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, id_, remote_id )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, command_queue_id_, command_queue_id )
    MSG_PARAMETER_GET_SET( size_t, offset_, offset )
    MSG_PARAMETER_GET_SET( bool, blocking_, blocking )

    MSG_PARAMETER_GET( size_t, size_, buffer_size )
    MSG_PARAMETER_GET( dcl::remote_ids_t, events_, events )

    inline void set_buffer_size( size_t size )
    {
        size_ = size;

        set_response_size( size + sizeof(msgEnqueueReadBuffer_response) - 1 );
    }

    inline void add_event( dcl::remote_id_t event_id )
    {
        events_.push_back( event_id );

        set_size( (events_.size() - 1) * sizeof(dcl::remote_id_t) +
                  sizeof( msgEnqueueReadBuffer_request ) );
    }

    // Response
    MSG_PARAMETER_GET( buffer_t, buffer_, buffer )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, event_id_, event_id )

    inline uint8_t* get_buffer_pointer()
    {
        return buffer_.data();
    }

    inline void allocate_buffer()
    {
        buffer_.resize( size_ );
    }

private:
    dcl::remote_id_t id_;
    dcl::remote_id_t command_queue_id_;
    size_t size_;
    size_t offset_;
    bool blocking_;
    bool return_event_;
    dcl::remote_ids_t events_;

    buffer_t buffer_;
    dcl::remote_id_t event_id_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgEnqueueReadBuffer_request
    {
        dcl::remote_id_t id_;
        dcl::remote_id_t command_queue_id_;
        uint32_t size_;
        uint32_t blocking_:1;
        uint32_t offset_:31;
        uint16_t return_event_:1;
        uint16_t event_count_:15;
        dcl::remote_id_t events_[ 1 ];
    };

    struct msgEnqueueReadBuffer_response
    {
        dcl::remote_id_t event_id_;
        uint32_t size_;
        uint8_t buffer_[ 1 ];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
// msgCreateImage2D
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateImage2D > : public base_message
{
public:
    dcl_message< msgCreateImage2D >() :
        base_message( msgCreateImage2D, true, 0, sizeof( dcl::remote_id_t ) ),
        context_id_( 0xffff ), buffer_ptr_( NULL ), buffer_len_( 0 ), 
        flags_( 0 ), channel_order_( 0 ), channel_type_( 0 ), 
        width_( 0 ), height_( 0 ), row_pitch_( 0 ), id_( 0xffff ){}

    typedef std::vector<uint8_t> buffer_t;

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, context_id_, context_id )
    MSG_PARAMETER_GET_SET( cl_mem_flags, flags_, flags )
    MSG_PARAMETER_GET_SET( cl_channel_order, channel_order_, channel_order )
    MSG_PARAMETER_GET_SET( cl_channel_type, channel_type_, channel_type )
    MSG_PARAMETER_GET_SET( size_t, width_, width )
    MSG_PARAMETER_GET_SET( uint8_t*, buffer_ptr_, buffer )

    MSG_PARAMETER_GET( size_t, height_, height )
    MSG_PARAMETER_GET( size_t, row_pitch_, row_pitch )
    MSG_PARAMETER_GET( size_t, buffer_len_, buffer_size )

    void set_height( size_t height )
    {
        height_ = height;

        update_request_size();
    }

    void set_row_pitch( size_t row_pitch )
    {
        row_pitch_ = row_pitch;

        update_request_size();
    }

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, id_, remote_id )

private:
    dcl::remote_id_t context_id_;
    const uint8_t* buffer_ptr_;
    size_t buffer_len_;
    cl_mem_flags flags_;
    cl_channel_order channel_order_;
    cl_channel_type channel_type_;
    size_t width_;
    size_t height_;
    size_t row_pitch_;
    buffer_t buffer_;

    dcl::remote_id_t id_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    inline void update_request_size()
    {
        buffer_len_ = height_ * row_pitch_;

        set_size( (( buffer_ptr_ == NULL ) ? 0 : buffer_len_) + sizeof(msgCreateImage2D_request) - 1 );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateImage2D_request
    {
        dcl::remote_id_t context_id_;
        uint8_t flags_;
        uint8_t channel_order:4;
        uint8_t channel_type:4;
        uint32_t width_;
        uint32_t height_;
        uint32_t row_pitch_;

        uint32_t buffer_len_;
        uint8_t buffer_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_MEMORY_MESSAGES_H_
