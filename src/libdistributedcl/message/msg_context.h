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
#ifndef _DCL_CONTEXT_MESSAGES_H_
#define _DCL_CONTEXT_MESSAGES_H_

#include "message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
    //msgCreateContext            = 24,
    //msgCreateContextFromType    = 25,
    //msgRetainContext            = 26,
    //msgReleaseContext           = 27,
    //msgGetContextInfo           = 28,
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateContextFromType > : public base_message
{
public:
    dcl_message< msgCreateContextFromType >() : 
        base_message( msgCreateContextFromType, true, sizeof( cl_device_type ), sizeof( remote_id_t ) ) {}

    inline const dcl::remote_id_t get_remote_id() const
    {
        return id_;
    }

    inline void set_remote_id( dcl::remote_id_t id )
    {
        id_ = id;
    }

    inline cl_device_type get_device_type() const
    {
        return device_type_;
    }

    inline void set_device_type( cl_device_type device_type )
    {
        device_type_ = device_type;
    }

protected:
    dcl::remote_id_t id_;
    cl_device_type device_type_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void create_response( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );
    virtual void parse_response( const base_message* message_ptr );
};
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgGetContextInfo > : public base_message
{
public:
    dcl_message< msgGetContextInfo >() : 
        device_count_( 0 ), id_( 0xffff ), devices_( NULL ), 
        base_message( msgGetContextInfo, true, sizeof( remote_id_t ), 0 ) {}

    inline const dcl::remote_id_t get_remote_id() const
    {
        return id_;
    }

    inline void set_remote_id( dcl::remote_id_t id )
    {
        id_ = id;
    }

    inline dcl::remote_id_t* get_devices()
    {
        return devices_;
    }

    inline uint32_t get_device_count()
    {
        return device_count_;
    }

    inline void set_device_count( uint32_t count )
    {
        device_count_ = count;

        if( devices_ != NULL )
        {
            delete[] devices_;
        }

        devices_ = new dcl::remote_id_t[ count ];

        set_size( sizeof( uint32_t ) + sizeof( dcl::remote_id_t ) * count );
    }

protected:
    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgGetContextInfo_response
    {
        uint32_t device_count_;
        dcl::remote_id_t devices_[1];
    };
    #pragma pack( pop )

    uint32_t device_count_;
    dcl::remote_id_t id_;
    dcl::remote_id_t* devices_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void create_response( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );
    virtual void parse_response( const base_message* message_ptr );
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_CONTEXT_MESSAGES_H_
