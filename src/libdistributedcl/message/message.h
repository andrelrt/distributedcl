/*
 * Copyright (c) 2009-2011 André Tupinambá (andrelrt@gmail.com)
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

#include "distributedcl_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
enum message_type
{
    // Internal messages [1-20)
    msg_invalid_message = 0,
    msg_error_message   = 1,    

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

    static base_message* parse_message( uint8_t* msg_buffer_ptr, std::size_t length );

    inline message_type get_type() const
    {
        return type_;
    }

    virtual void set_response( const base_message* ){}

    inline void get_buffer( uint8_t* buffer_ptr )
    {
        buffer_ptr_ = buffer_ptr;

        message_header* header = reinterpret_cast< message_header* >( buffer_ptr_ );
        header->version = message_v1_0;
        header->type = type_;
        header->request = 1;
        header->id = 0;
        header->length = static_cast< uint32_t >( htonl( static_cast< u_long >( size_ ) ) );

        create_buffer();
    }

    inline std::size_t get_size() const
    {
        return size_;
    }

    inline bool waiting_response()
    {
        return wait_response_;
    }

protected:
    base_message( message_type type, bool wait_response = false, std::size_t size = 0 ) : 
        wait_response_( wait_response ), buffer_ptr_( NULL ),
        size_( size + sizeof( message_header ) ), type_( type )
    {}

    virtual void parse(){}
    virtual void create_buffer(){}

    inline void set_size( std::size_t size )
    {
        size_ = size + sizeof( message_header );
    }

    inline uint8_t* get_payload()
    {
        return buffer_ptr_ + sizeof( message_header );
    }

    inline bool is_request()
    {
        return (reinterpret_cast< message_header* >( buffer_ptr_ )->request == 1);
    }

    inline uint32_t get_payload_size()
    {
        return static_cast< uint32_t >( size_ - sizeof( message_header ) );
    }

    inline void set_wait_response()
    {
        wait_response_ = true;
    }

private:
    bool wait_response_;
    uint8_t* buffer_ptr_;
    std::size_t size_;
    message_type type_;

    enum{ message_v1_0 = 0x10 };

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct message_header
    {
        uint8_t version;
        uint8_t type;
        uint16_t request : 1;
        uint16_t id : 15;
        uint32_t length;
    };
    #pragma pack( pop )

    inline void set_buffer( uint8_t* msg_buffer_ptr, std::size_t size )
    {
        buffer_ptr_ = msg_buffer_ptr;
        size_ = size;
    }
};
//-----------------------------------------------------------------------------
template< message_type MESSAGE_NUMBER > 
class dcl_message : public base_message {};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_MESSAGE_H_
