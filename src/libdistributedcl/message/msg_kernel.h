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
#ifndef _DCL_KERNEL_MESSAGES_H_
#define _DCL_KERNEL_MESSAGES_H_

#include "message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
    //msgCreateKernel             = 55,
    //msgCreateKernelsInProgram   = 56,
    //msgRetainKernel             = 57,
    //msgReleaseKernel            = 58,
    //msgSetKernelArg             = 59,
    //msgGetKernelInfo            = 60,
    //msgGetKernelWorkGroupInfo   = 61,
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateKernel > : public base_message
{
public:
    dcl_message< msgCreateKernel >() : 
        base_message( msgCreateKernel, true, 0, sizeof(dcl::remote_id_t) ) {}

    inline const dcl::remote_id_t get_remote_id() const
    {
        return id_;
    }

    inline void set_remote_id( dcl::remote_id_t id )
    {
        id_ = id;
    }

    inline const dcl::remote_id_t get_program_id() const
    {
        return program_id_;
    }

    inline void set_program_id( dcl::remote_id_t id )
    {
        program_id_ = id;
    }

    inline const std::string& get_name() const
    {
        return name_;
    }

    inline void set_name( const std::string& name )
    {
        name_.assign( name );

        update_request_size();
    }

private:
    std::string name_;
    dcl::remote_id_t id_;
    dcl::remote_id_t program_id_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void create_response( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );
    virtual void parse_response( const base_message* message_ptr );

    inline void update_request_size()
    {
        set_size( name_.length() +
                  sizeof( msgCreateKernel_request ) - 1 );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateKernel_request
    {
        dcl::remote_id_t program_id_;
        uint32_t name_len_;

        uint8_t name_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_PROGRAM_MESSAGES_H_
