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
        base_message( msgCreateBuffer, true, 0, sizeof( dcl::remote_id_t ) ) {}

    typedef std::vector<uint8_t> buffer_t;

    // Request
    inline const dcl::remote_id_t get_context_id() const
    {
        return context_id_;
    }

    inline void set_context_id( dcl::remote_id_t id )
    {
        context_id_ = id;
    }

    inline const buffer_t& get_buffer() const
    {
        return buffer_;
    }

    inline void set_buffer( const uint8_t* ptr, size_t size )
    {
        buffer_.assign( ptr, ptr + size );
        update_request_size();
    }

    inline cl_mem_flags get_flags() const
    {
        return flags_;
    }

    inline void set_flags( cl_mem_flags flags )
    {
        flags_ = flags;
    }

    // Response
    inline const dcl::remote_id_t get_remote_id() const
    {
        return id_;
    }

    inline void set_remote_id( dcl::remote_id_t id )
    {
        id_ = id;
    }

private:
    dcl::remote_id_t context_id_;
    buffer_t buffer_;
    cl_mem_flags flags_;

    dcl::remote_id_t id_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void create_response( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );
    virtual void parse_response( const base_message* message_ptr );

    inline void update_request_size()
    {
        set_size( buffer_.size() + sizeof(msgCreateBuffer_request) - 1 );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateBuffer_request
    {
        dcl::remote_id_t context_id_;
        uint16_t flags_;
        uint32_t buffer_len_;

        uint8_t buffer_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_MEMORY_MESSAGES_H_
