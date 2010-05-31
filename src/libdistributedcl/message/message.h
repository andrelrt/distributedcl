/*
 * Copyright (c) 2009-2010 André Tupinambá (andrelrt@gmail.com)
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

#include <vector>
#include "distributedcl_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
enum message_type
{
    // Internal Messages [1-20)
    invalid_message = 0,
    error_message   = 1,    

    // OpenCL Messages [20-128)
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

    // OpenCL Extension Messages [128-256)
    msgExtension0 = 128,
    msgExtension1 = 129,
};
//-----------------------------------------------------------------------------
class message
{
public:
    static const message* parse_message( const boost::uint8_t* msg_buffer_ptr,
                                         std::size_t length );

    inline boost::uint16_t length() const
    {
        return length_;
    }

    inline message_type type() const
    {
        return type_;
    }

protected:
    message( message_type type ) : type_( type ), length_( 0 ) {}
    ~message();

    enum{ message_v1_0 = 0x10 };

    #pragma pack( push, 1 )

    // Better when aligned in 32 bits boundary
    struct message_header
    {
        boost::uint8_t version;
        boost::uint8_t type;
        boost::uint16_t length;
    };
    #pragma pack( pop )

    inline void set_length( std::size_t length )
    {
        length_ = length;
    }

    inline void set_message_buffer( const boost::uint8_t* msg_buffer_ptr )
    {
        header_ptr_ = reinterpret_cast< const message_header* >( msg_buffer_ptr );
        payload_ptr_ = msg_buffer_ptr + sizeof( message_header );
    }

private:
    message_type type_;
    std::size_t length_;
    const message_header* header_ptr_;
    const boost::uint8_t* payload_ptr_;
};
//-----------------------------------------------------------------------------
class msg_platform : 
    public message
{
public:
    msg_platform( remote_id_t remote_id ) : 
        remote_id_( remote_id ), message( msgGetPlatformInfo ) {}

    const char* get_name() const { return ""; }
    const char* get_vendor() const { return ""; }
    const char* get_profile() const { return ""; }
    const char* get_version() const { return ""; }
    const char* get_extensions() const { return ""; }

private:
    remote_id_t remote_id_;
};
//-----------------------------------------------------------------------------
class msg_get_platform_ids : public message
{
public:
    msg_get_platform_ids() : message( msgGetPlatformIDs ) {}

    inline const std::vector< remote_id_t >& get_ids()
    {
        return remote_ids_;
    }

private:
    std::vector< remote_id_t > remote_ids_;
};
//-----------------------------------------------------------------------------
class msg_get_devices : public message
{
public:
    msg_get_devices( remote_id_t remote_id ) : 
        remote_id_( remote_id ), message( msgGetDeviceIDs ) {}

    inline const std::vector< remote_id_t >& get_ids()
    {
        return remote_ids_;
    }

private:
    remote_id_t remote_id_;
    std::vector< remote_id_t > remote_ids_;
};
//-----------------------------------------------------------------------------
class msg_device : public message
{
public:
    msg_device( remote_id_t remote_id ) : 
        remote_id_( remote_id ), message( msgGetDeviceInfo ) {}

private:
    remote_id_t remote_id_;
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_MESSAGE_H_
