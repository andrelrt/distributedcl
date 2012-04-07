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
class dcl_message< msgCreateContext > : public base_message
{
public:
    dcl_message< msgCreateContext >() :
        base_message( msgCreateContext, true, sizeof(msgCreateContext_request), sizeof( remote_id_t ) ) {}

    inline const remote_ids_t& get_devices() const
    {
        return devices_;
    }

    inline void set_device_count( uint32_t count )
    {
        devices_.reserve( count );
    }

    inline void add_device( dcl::remote_id_t device_id )
    {
        devices_.push_back( device_id );
        update_request_size();
    }

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, id_, remote_id )

protected:
    dcl::remote_id_t id_;
    dcl::remote_ids_t devices_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    inline void update_request_size()
    {
        set_size( sizeof(msgCreateContext_request) - sizeof(dcl::remote_id_t) +
                  devices_.size() * sizeof(dcl::remote_id_t) );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateContext_request
    {
        uint32_t device_count_;
        dcl::remote_id_t devices_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateContextFromType > : public base_message
{
public:
    dcl_message< msgCreateContextFromType >() : 
        base_message( msgCreateContextFromType, true, sizeof( cl_device_type ), sizeof( remote_id_t ) ) {}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, id_, remote_id )
    MSG_PARAMETER_GET_SET( cl_device_type, device_type_, device_type )

protected:
    dcl::remote_id_t id_;
    cl_device_type device_type_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );
};
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgGetContextInfo > : public base_message
{
public:
    dcl_message< msgGetContextInfo >() : 
        base_message( msgGetContextInfo, true, sizeof( remote_id_t ), 0 ),
        id_( 0xffff ){}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, id_, remote_id )

    inline const dcl::remote_ids_t& get_devices() const
    {
        return devices_;
    }

    inline uint32_t get_device_count()
    {
        return static_cast<uint32_t>( devices_.size() );
    }

    inline void set_device_count( uint32_t count )
    {
        devices_.reserve( count );
    }

    inline void add_device( dcl::remote_id_t device_id )
    {
        devices_.push_back( device_id );
        update_response_size();
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

    dcl::remote_id_t id_;
    dcl::remote_ids_t devices_;

    inline void update_response_size()
    {
        set_response_size( sizeof(msgGetContextInfo_response) - sizeof(dcl::remote_id_t) +
                           devices_.size() * sizeof(dcl::remote_id_t) );
    }

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_CONTEXT_MESSAGES_H_
