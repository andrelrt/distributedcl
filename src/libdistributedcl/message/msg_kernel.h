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
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

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
        remote_id_( INVALID_REMOTE_ID ), program_id_( INVALID_REMOTE_ID ){}

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
        kernel_id_( INVALID_REMOTE_ID )
    {
    }

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, kernel_id_, kernel_id )

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
        uint32_t offset_dimensions_:2;
        uint32_t global_dimensions_:2;
        uint32_t local_dimensions_:2;
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
class dcl_message< msgSetKernelArg > : // public base_message
    public enqueue_message
{
public:
    dcl_message< msgSetKernelArg >() :
        //base_message( msgSetKernelArg, false ), 
        enqueue_message( msgSetKernelArg, false ), 
        kernel_id_( INVALID_REMOTE_ID ), argument_id_( INVALID_REMOTE_ID ), index_( 0 ),
        argument_type_( unknow_type ), size_( 0 ), is_null_( false ){}

    enum argument_type_t
    {
        unknow_type = 0,
        memory_type = 1,
        image_type = 2,
        sampler_type = 3,
    };

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, kernel_id_, kernel_id )
    MSG_PARAMETER_GET_SET( uint16_t, index_, index )
    MSG_PARAMETER_GET( dcl::remote_id_t, argument_id_, memory_id )
    MSG_PARAMETER_GET( dcl::remote_id_t, argument_id_, image_id )
    MSG_PARAMETER_GET( dcl::remote_id_t, argument_id_, sampler_id )
    MSG_PARAMETER_GET( uint32_t, size_, buffer_size )
    MSG_PARAMETER_GET( argument_type_t, argument_type_, argument_type )

    inline const uint8_t* get_buffer_pointer() const
    {
        return is_null_ ? NULL : buffer_.data();
    }

    inline void set_memory_id( dcl::remote_id_t memory_id )
    {
        argument_type_ = memory_type;
        argument_id_ = memory_id;

        update_request_size();
    }

    inline void set_image_id( dcl::remote_id_t image_id )
    {
        argument_type_ = image_type;
        argument_id_ = image_id;

        update_request_size();
    }

    inline void set_sampler_id( dcl::remote_id_t sampler_id )
    {
        argument_type_ = sampler_type;
        argument_id_ = sampler_id;

        update_request_size();
    }

    inline void set_buffer( const uint8_t* arg_value, size_t arg_size )
    {
        argument_type_ = unknow_type;
        is_null_ = (arg_value == NULL);
        size_ = static_cast<uint32_t>( arg_size );

        if( !is_null_ )
        {
            buffer_.assign( arg_value, arg_value + arg_size );
        }

        update_request_size();
    }

    inline bool is_object() const
    {
        return argument_type_ != unknow_type;
    }

private:
    dcl::remote_id_t kernel_id_;
    dcl::remote_id_t argument_id_;
    uint16_t index_;
    std::vector<uint8_t> buffer_;
    argument_type_t argument_type_;
    uint32_t size_;
    bool is_null_;

    virtual void create_request( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );

    inline virtual void update_request_size()
    {
        if( argument_type_ != unknow_type )
        {
            set_size( get_enqueue_request_size() +
                      sizeof(msgSetKernelArg_memory_request) );
        }
        else
        {
            if( !is_null_ )
            {
                set_size( get_enqueue_request_size() +
                          buffer_.size() +
                          sizeof(msgSetKernelArg_buffer_request) - 1 );
            }
            else
            {
                set_size( get_enqueue_request_size() +
                          sizeof(msgSetKernelArg_buffer_request) - 1 );
            }
        }
    }

    #pragma pack( push, 1 )

    // Better when aligned in 32 bits boundary
    struct msgSetKernelArg_memory_request
    {
        dcl::remote_id_t kernel_id_;
        uint16_t argument_type_:2;
        uint16_t index_:14;
        dcl::remote_id_t argmuent_id_;
    };

    struct msgSetKernelArg_buffer_request
    {
        dcl::remote_id_t kernel_id_;
        uint16_t argument_type_:2;
        uint16_t is_null_:1;
        uint16_t index_:13;
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
        kernel_id_( INVALID_REMOTE_ID ), device_id_( INVALID_REMOTE_ID ){}

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
