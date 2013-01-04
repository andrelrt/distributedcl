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
#ifndef _DCL_SAMPLER_MESSAGES_H_
#define _DCL_SAMPLER_MESSAGES_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
    //msgCreateSampler            = 43,
    //msgRetainSampler            = 44,
    //msgReleaseSampler           = 45,
    //msgGetSamplerInfo           = 46,
//-----------------------------------------------------------------------------
// msgCreateSampler
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateSampler > : public base_message
{
public:
    dcl_message< msgCreateSampler >() : 
        base_message( msgCreateSampler, true, sizeof(msgCreateSampler_request), sizeof( dcl::remote_id_t ) ),
    context_id_( INVALID_REMOTE_ID ), normalized_coords_( CL_FALSE ), addressing_mode_( 0 ), filter_mode_( 0 ), remote_id_( INVALID_REMOTE_ID ){}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, context_id_, context_id )
    MSG_PARAMETER_GET_SET( cl_bool, normalized_coords_, normalized_coords )
    MSG_PARAMETER_GET_SET( cl_addressing_mode, addressing_mode_, addressing_mode )
    MSG_PARAMETER_GET_SET( cl_filter_mode, filter_mode_, filter_mode )

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )

private:
    dcl::remote_id_t context_id_;
    cl_bool normalized_coords_;
    cl_addressing_mode addressing_mode_;
    cl_filter_mode filter_mode_;

    dcl::remote_id_t remote_id_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateSampler_request
    {
        dcl::remote_id_t context_id_;
        uint16_t normalized_coords_:1;
        uint16_t addressing_mode_:3;
        uint16_t filter_mode_:1;
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_SAMPLER_MESSAGES_H_
