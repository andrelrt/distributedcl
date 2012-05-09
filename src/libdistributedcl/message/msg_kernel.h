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
// msgCreateKernel
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateKernel > : public base_message
{
public:
    dcl_message< msgCreateKernel >() :
        base_message( msgCreateKernel, true, 0, sizeof(dcl::remote_id_t) ),
        remote_id_( 0xffff ), program_id_( 0xffff ){}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, program_id_, program_id )
    MSG_PARAMETER_GET( std::string&, name_, name )

    inline void set_name( const std::string& name )
    {
        name_.assign( name );

        update_request_size();
    }

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )

private:
    std::string name_;
    dcl::remote_id_t remote_id_;
    dcl::remote_id_t program_id_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

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
// msgEnqueueNDRangeKernel
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgEnqueueNDRangeKernel > :
    public enqueue_message
{
public:
    dcl_message< msgEnqueueNDRangeKernel >() :
        enqueue_message( msgEnqueueNDRangeKernel, false,
                         sizeof( msgEnqueueNDRangeKernel_request ) ),
        kernel_id_( 0xffff ), command_queue_id_( 0xffff )
    {
    }

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, kernel_id_, kernel_id )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, command_queue_id_, command_queue_id )

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

    inline virtual void update_request_size()
    {
        set_size( get_enqueue_request_size() + sizeof(msgEnqueueNDRangeKernel_request) );
    }

    virtual void create_request( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgEnqueueNDRangeKernel_request
    {
        dcl::remote_id_t kernel_id_;
        dcl::remote_id_t command_queue_id_;
        uint32_t dimensions_;
        uint32_t offset_[ 3 ];
        uint32_t global_[ 3 ];
        uint32_t local_[ 3 ];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
// msgSetKernelArg
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgSetKernelArg > : public base_message
{
public:
    dcl_message< msgSetKernelArg >() :
        base_message( msgSetKernelArg, false, 0, 0 ), kernel_id_( 0xffff ),
        memory_id_( 0xffff ), index_( 0 ), is_memory_object_( false ),
        size_( 0 ), is_null_( false ){}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, kernel_id_, kernel_id )
    MSG_PARAMETER_GET_SET( uint16_t, index_, index )
    MSG_PARAMETER_GET( dcl::remote_id_t, memory_id_, memory_id )
    MSG_PARAMETER_GET( uint32_t, size_, buffer_size )

    inline const uint8_t* get_buffer_pointer() const
    {
        return is_null_ ? NULL : buffer_.data();
    }

    inline void set_memory_id( dcl::remote_id_t memory_id )
    {
        is_memory_object_ = true;
        memory_id_ = memory_id;

        set_size( sizeof(msgSetKernelArg_memory_request) );
    }

    inline void set_buffer( const uint8_t* arg_value, size_t arg_size )
    {
        is_memory_object_ = false;
        is_null_ = (arg_value == NULL);
        size_ = static_cast<uint32_t>( arg_size );

        if( !is_null_ )
        {
            buffer_.assign( arg_value, arg_value + arg_size );

            set_size( arg_size + sizeof(msgSetKernelArg_buffer_request) - 1 );
        }
        else
        {
            set_size( sizeof(msgSetKernelArg_buffer_request) - 1 );
        }
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
    uint32_t size_;
    bool is_null_;

    virtual void create_request( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );

    #pragma pack( push, 1 )

    // Better when aligned in 32 bits boundary
    struct msgSetKernelArg_memory_request
    {
        dcl::remote_id_t kernel_id_;
        uint16_t is_memory_object_:1;
        uint16_t index_:15;
        dcl::remote_id_t memory_id_;
    };

    struct msgSetKernelArg_buffer_request
    {
        dcl::remote_id_t kernel_id_;
        uint16_t is_memory_object_:1;
        uint16_t is_null_:1;
        uint16_t index_:14;
        uint32_t size_;

        uint8_t buffer_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
// msgGetKernelWorkGroupInfo
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgGetKernelWorkGroupInfo > : public base_message
{
public:
    dcl_message< msgGetKernelWorkGroupInfo >() :
        base_message( msgGetKernelWorkGroupInfo, true, 2*sizeof(dcl::remote_id_t), 
                      sizeof(msgGetKernelWorkGroupInfo_response) ),
        kernel_id_( 0xffff ), device_id_( 0xffff ){}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, kernel_id_, kernel_id )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, device_id_, device_id )

    // Response
    inline dcl::info::kernel_group_info& get_info()
    {
        return info_;
    }

private:
    dcl::remote_id_t kernel_id_;
    dcl::remote_id_t device_id_;
    dcl::info::kernel_group_info info_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgGetKernelWorkGroupInfo_response
    {
        uint64_t local_mem_size_;
        uint64_t private_mem_size_;
        uint32_t work_group_size_;
        uint32_t compile_work_group_size_[ 3 ];
        uint32_t preferred_work_group_size_multiple_;
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_PROGRAM_MESSAGES_H_
