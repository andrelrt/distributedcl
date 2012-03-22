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
#include "info/kernel_info.h"
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
    //msgEnqueueNDRangeKernel     = 80,
    //msgEnqueueTask              = 81,
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
template<>
class dcl_message< msgEnqueueNDRangeKernel > : public base_message
{
public:
    dcl_message< msgEnqueueNDRangeKernel >() :
        base_message( msgEnqueueNDRangeKernel, false, sizeof( msgEnqueueNDRangeKernel_request ), 0 ) {}

    inline const dcl::remote_id_t get_kernel_id() const
    {
        return kernel_id_;
    }

    inline void set_kernel_id( dcl::remote_id_t id )
    {
        kernel_id_ = id;
    }

    inline const dcl::remote_id_t get_command_queue_id() const
    {
        return command_queue_id_;
    }

    inline void set_command_queue_id( dcl::remote_id_t id )
    {
        command_queue_id_ = id;
    }

    inline dcl::info::ndrange& get_offset()
    {
        return offset_;
    }

    inline dcl::info::ndrange& get_global()
    {
        return global_;
    }

    inline dcl::info::ndrange& get_local()
    {
        return local_;
    }

private:
    dcl::remote_id_t kernel_id_;
    dcl::remote_id_t command_queue_id_;
    dcl::info::ndrange offset_;
    dcl::info::ndrange global_;
    dcl::info::ndrange local_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgEnqueueNDRangeKernel_request
    {
        dcl::remote_id_t kernel_id_;
        dcl::remote_id_t command_queue_id_;
        uint16_t dimensions_;
        uint16_t offset_[ 3 ];
        uint16_t global_[ 3 ];
        uint16_t local_[ 3 ];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgSetKernelArg > : public base_message
{
public:
    dcl_message< msgSetKernelArg >() :
        base_message( msgSetKernelArg, false, 0, 0 ),
        is_memory_object( false ), memory_id_( 0xffff ){}

    inline const dcl::remote_id_t get_kernel_id() const
    {
        return kernel_id_;
    }

    inline void set_kernel_id( dcl::remote_id_t id )
    {
        kernel_id_ = id;
    }

    inline uint16_t get_index() const
    {
        return index_;
    }

    inline void set_index( uint16_t arg_index )
    {
        index_ = index;
    }

    inline dcl::remote_id_t get_memory_id() const
    {
        return memory_id_;
    }

    inline void set_memory_id( dcl::remote_id_t memory_id )
    {
        is_memory_object_ = true;
        memory_id_ = memory_id;

        set_size( sizeof(msgSetKernelArg_memory_request) );
    }

    inline const std::vector<uint8_t>& get_buffer() const
    {
        return buffer_;
    }

    inline void set_buffer( const uint8_t* arg_value, size_t arg_size )
    {
        is_memory_object = false;
        buffer_.assign( arg_value, arg_value + arg_size );

        set_size( arg_size + sizeof(msgSetKernelArg_buffer_request) - 1 );
    }

    inline bool is_memory_object() const
    {
        return is_memory_object_;
    }

private:
    dcl::remote_id_t kernel_id_;
    dcl::remote_id_t memory_id_;
    uint16_t index_;
    std::vector<uint8_t> buffer_;
    bool is_memory_object_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );

    #pragma pack( push, 1 )

    // Better when aligned in 32 bits boundary
    struct msgSetKernelArg_memory_request
    {
        dcl::remote_id_t kernel_id_;
        uint16_t is_memory_object:1;
        uint16_t index:15;
        dcl::remote_id_t memory_id_;
    };

    struct msgSetKernelArg_buffer_request
    {
        dcl::remote_id_t kernel_id_;
        uint16_t is_memory_object_:1;
        uint16_t index_:15;
        uint32_t size_;

        uint8_t buffer_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_PROGRAM_MESSAGES_H_
