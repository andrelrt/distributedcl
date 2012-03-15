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
#ifndef _DCL_COMMAND_QUEUE_MESSAGES_H_
#define _DCL_COMMAND_QUEUE_MESSAGES_H_

#include "message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
    //msgCreateCommandQueue       = 29,
    //msgRetainCommandQueue       = 30,
    //msgReleaseCommandQueue      = 31,
    //msgGetCommandQueueInfo      = 32,
    //msgSetCommandQueueProperty  = 33,
    //msgFlush                    = 67,
    //msgFinish                   = 68,
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateCommandQueue > : public base_message
{
public:
    dcl_message< msgCreateCommandQueue >() : 
        base_message( msgCreateCommandQueue, true, sizeof(msgCreateCommandQueue_request), sizeof(dcl::remote_id_t) ) {}

    inline const dcl::remote_id_t get_remote_id() const
    {
        return id_;
    }

    inline void set_remote_id( dcl::remote_id_t id )
    {
        id_ = id;
    }

    inline const dcl::remote_id_t get_context_id() const
    {
        return context_id_;
    }

    inline void set_context_id( dcl::remote_id_t id )
    {
        context_id_ = id;
    }

    inline const dcl::remote_id_t get_device_id() const
    {
        return device_id_;
    }

    inline void set_device_id( dcl::remote_id_t id )
    {
        device_id_ = id;
    }

    inline cl_command_queue_properties get_properties() const
    {
        return properties_;
    }

    inline void set_properties( cl_command_queue_properties properties )
    {
        properties_ = properties;
    }

private:
    dcl::remote_id_t id_;
    dcl::remote_id_t device_id_;
    dcl::remote_id_t context_id_;
    cl_command_queue_properties properties_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void create_response( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );
    virtual void parse_response( const base_message* message_ptr );

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateCommandQueue_request
    {
        dcl::remote_id_t device_id_;
        dcl::remote_id_t context_id_;
        cl_command_queue_properties properties_;
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgFinish > : public base_message
{
public:
    dcl_message< msgFinish >() : 
        base_message( msgFinish, true, sizeof(dcl::remote_id_t), 0 ) {}

    inline const dcl::remote_id_t get_remote_id() const
    {
        return id_;
    }

    inline void set_remote_id( dcl::remote_id_t id )
    {
        id_ = id;
    }

private:
    dcl::remote_id_t id_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_COMMAND_QUEUE_MESSAGES_H_
