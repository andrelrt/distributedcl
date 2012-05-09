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
        flags_( 0 ), remote_id_( 0xffff ){}

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
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )

private:
    dcl::remote_id_t context_id_;
    const uint8_t* buffer_ptr_;
    size_t buffer_len_;
    cl_mem_flags flags_;
    //buffer_t buffer_;

    dcl::remote_id_t remote_id_;

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
class dcl_message< msgEnqueueWriteBuffer > : public enqueue_message
{
public:
    dcl_message< msgEnqueueWriteBuffer >() :
        enqueue_message( msgEnqueueWriteBuffer, false ), remote_id_( 0xffff ),
        command_queue_id_( 0xffff ), buffer_ptr_( NULL ), buffer_len_( 0 ){}

    typedef std::vector<uint8_t> buffer_t;

    // Request
    MSG_PARAMETER_GET_SET( bool, return_event_, return_event )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, command_queue_id_, command_queue_id )

    MSG_PARAMETER_GET( uint8_t*, buffer_ptr_, buffer_pointer )
    MSG_PARAMETER_GET( size_t, buffer_len_, buffer_size )

    inline void set_buffer( const uint8_t* ptr, size_t size, size_t offset )
    {
        buffer_ptr_ = ptr + offset;
        buffer_len_ = size;

        update_request_size();
    }


private:
    dcl::remote_id_t remote_id_;
    dcl::remote_id_t command_queue_id_;
    const uint8_t* buffer_ptr_;
    size_t buffer_len_;
    //buffer_t buffer_;

    virtual void create_request( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );

    inline virtual void update_request_size()
    {
        set_size( buffer_len_ + 
                  get_enqueue_request_size() + 
                  sizeof(msgEnqueueWriteBuffer_request) - 1 );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgEnqueueWriteBuffer_request
    {
        dcl::remote_id_t id_;
        dcl::remote_id_t command_queue_id_;
        uint32_t buffer_len_;

        uint8_t buffer_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
// msgEnqueueReadBuffer
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgEnqueueReadBuffer > : public enqueue_message
{
public:
    dcl_message< msgEnqueueReadBuffer >() :
        enqueue_message( msgEnqueueReadBuffer, true, sizeof( msgEnqueueReadBuffer_request ) ),
        remote_id_( 0xffff ), command_queue_id_( 0xffff ), size_( 0 ),
        offset_( 0 ), data_ptr_( NULL ){}

    typedef std::vector<uint8_t> buffer_t;

    // Request
    MSG_PARAMETER_GET_SET( bool, return_event_, return_event )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, command_queue_id_, command_queue_id )
    MSG_PARAMETER_GET_SET( size_t, offset_, offset )

    MSG_PARAMETER_GET( size_t, size_, buffer_size )

    inline void set_data_pointer( void* data_ptr )
    {
        data_ptr_ = data_ptr;
    }

    inline void set_buffer_size( size_t size )
    {
        size_ = size;

        set_response_size( size + sizeof(msgEnqueueReadBuffer_response) - 1 );
    }

    // Response
    inline uint8_t* get_buffer_pointer()
    {
        return buffer_.data();
    }

    inline void allocate_buffer()
    {
        buffer_.resize( size_ );
    }

private:
    dcl::remote_id_t remote_id_;
    dcl::remote_id_t command_queue_id_;
    size_t size_;
    size_t offset_;
    void* data_ptr_;

    buffer_t buffer_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    inline virtual void update_request_size()
    {
        set_size( get_enqueue_request_size() + sizeof(msgEnqueueReadBuffer_request) );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgEnqueueReadBuffer_request
    {
        dcl::remote_id_t id_;
        dcl::remote_id_t command_queue_id_;
        uint32_t size_;
        uint32_t offset_:31;
        uint32_t blocking_:1;
    };

    struct msgEnqueueReadBuffer_response
    {
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
        width_( 0 ), height_( 0 ), row_pitch_( 0 ), remote_id_( 0xffff ){}

    typedef std::vector<uint8_t> buffer_t;

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, context_id_, context_id )
    MSG_PARAMETER_GET_SET( cl_mem_flags, flags_, flags )

    MSG_PARAMETER_GET( uint8_t*, buffer_ptr_, buffer )
    MSG_PARAMETER_GET( size_t, width_, width )
    MSG_PARAMETER_GET( size_t, height_, height )
    MSG_PARAMETER_GET( size_t, row_pitch_, row_pitch )
    MSG_PARAMETER_GET( size_t, buffer_len_, buffer_size )
    MSG_PARAMETER_GET( cl_channel_type, channel_type_, channel_type )
    MSG_PARAMETER_GET( cl_channel_order, channel_order_, channel_order )

    void set_width( size_t width )
    {
        width_ = width;

        update_request_size();
    }

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

    void set_channel_type( cl_channel_type channel_type )
    {
        channel_type_ = channel_type;

        update_request_size();
    }

    void set_channel_order( cl_channel_order channel_order )
    {
        channel_order_ = channel_order;

        update_request_size();
    }

    void set_buffer( const uint8_t* buffer_ptr )
    {
        buffer_ptr_ = buffer_ptr;

        update_request_size();
    }

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )

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
    //buffer_t buffer_;

    dcl::remote_id_t remote_id_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    inline void update_request_size()
    {
        size_t row_pitch = row_pitch_;
        if( row_pitch == 0 )
        {
            row_pitch = width_ * channel_element_size();
        }

        buffer_len_ = height_ * row_pitch;

        set_size( (( buffer_ptr_ == NULL ) ? 0 : buffer_len_) + sizeof(msgCreateImage2D_request) - 1 );
    }

    inline uint8_t channel_order_to_network()
    {
        return channel_order_ & 0xf;
    }

    inline uint8_t channel_type_to_network()
    {
        return channel_type_ & 0xf;
    }

    inline void network_to_channel_order( uint8_t channel_order )
    {
        channel_order_ = channel_order + CL_R; // 0x10B0
    }

    inline void network_to_channel_type( uint8_t channel_type )
    {
        channel_type_ = channel_type + CL_SNORM_INT8; // 0x10D0
    }

    inline size_t channel_element_size()
    {
        size_t type_size = 0;

        switch( channel_type_ )
        {
            case CL_UNORM_SHORT_565:
            case CL_UNORM_SHORT_555:
                return 2;

            case CL_UNORM_INT_101010:
                return 4;

            case CL_SNORM_INT8:
            case CL_UNORM_INT8:
            case CL_SIGNED_INT8:
            case CL_UNSIGNED_INT8:
                type_size = 1;
                break;

            case CL_SNORM_INT16:
            case CL_UNORM_INT16:
            case CL_SIGNED_INT16:
            case CL_UNSIGNED_INT16:
            case CL_HALF_FLOAT:
                type_size = 2;
                break;

            case CL_SIGNED_INT32:
            case CL_UNSIGNED_INT32:
            case CL_FLOAT:
                type_size = 4;
                break;

            default:
                return 0;
        }

        switch( channel_order_ )
        {
            case CL_R:
            case CL_A:
                return type_size;

            case CL_RG:
            case CL_RA:
            case CL_Rx:
                return 2 * type_size;

            case CL_RGB:
            case CL_RGx:
                return 3 * type_size;

            case CL_RGBA:
            case CL_BGRA:
            case CL_ARGB:
            case CL_RGBx:
                return 4 * type_size;

            case CL_INTENSITY:
            case CL_LUMINANCE:
                return 4 * type_size;

            default:
                return 0;
        }
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateImage2D_request
    {
        dcl::remote_id_t context_id_;
        uint8_t message_buffer_:1;
        uint8_t flags_:7;
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
