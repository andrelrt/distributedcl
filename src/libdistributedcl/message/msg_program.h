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
#ifndef _DCL_PROGRAM_MESSAGES_H_
#define _DCL_PROGRAM_MESSAGES_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
    //msgCreateProgramWithSource  = 47,
    //msgCreateProgramWithBinary  = 48,
    //msgRetainProgram            = 49,
    //msgReleaseProgram           = 50,
    //msgBuildProgram             = 51,
    //msgUnloadCompiler           = 52,
    //msgGetProgramInfo           = 53,
    //msgGetProgramBuildInfo      = 54,
//-----------------------------------------------------------------------------
// msgCreateProgramWithSource
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateProgramWithSource > : public base_message
{
public:
    dcl_message< msgCreateProgramWithSource >() : 
        base_message( msgCreateProgramWithSource, true, 0, sizeof( dcl::remote_id_t ) ),
    context_id_( INVALID_REMOTE_ID ), remote_id_( INVALID_REMOTE_ID ){}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, context_id_, context_id )
    MSG_PARAMETER_GET( std::string&, source_code_, source_code )

    inline void set_source_code( const std::string& source_code )
    {
        source_code_.assign( source_code );

        set_size( source_code.length() +  sizeof( msgCreateProgramWithSource_request ) - 1 );
    }

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )

private:
    dcl::remote_id_t context_id_;
    std::string source_code_;

    dcl::remote_id_t remote_id_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateProgramWithSource_request
    {
        dcl::remote_id_t context_id_;
        uint32_t source_code_len_;

        uint8_t source_code_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
// msgCreateProgramWithBinary
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgCreateProgramWithBinary > : public base_message
{
public:
    dcl_message< msgCreateProgramWithBinary >() :
        base_message( msgCreateProgramWithBinary, true, 0, sizeof( dcl::remote_id_t ) ),
    new_binaries_(false), context_id_( INVALID_REMOTE_ID ), binaries_( NULL ), remote_id_( INVALID_REMOTE_ID ){}

    ~dcl_message< msgCreateProgramWithBinary >()
    {
        if( new_binaries_ )
            delete[] binaries_;
    }

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, context_id_, context_id )
    MSG_PARAMETER_GET( dcl::remote_ids_t&, devices_, devices )
    MSG_PARAMETER_GET_SET( unsigned char**, binaries_, binaries )

    void add_device( dcl::remote_id_t device_id )
    {
        devices_.push_back( device_id );
        set_req_size();
    }

    void set_lengths( const size_t* lengths, size_t size )
    {
        lengths_.reserve( size );

        for( uint32_t i = 0; i < size; ++i )
        {
            lengths_.push_back( static_cast<uint32_t>( lengths[ i ] ) );
        }
        set_req_size();
    }

    const std::vector<size_t>& get_lengths() const
    {
        return lengths_;
    }

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )
    MSG_PARAMETER_GET( std::vector<cl_int>&, binary_status_, binary_status )

private:
    bool new_binaries_;
    dcl::remote_id_t context_id_;
    dcl::remote_ids_t devices_;
    std::vector<size_t> lengths_;
    const unsigned char** binaries_;

    dcl::remote_id_t remote_id_;
    std::vector<cl_int> binary_status_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    void set_req_size()
    {
        size_t size = sizeof( msgCreateProgramWithBinary_request ) - 1;

        size += devices_.size() * (sizeof( dcl::remote_id_t ) + sizeof(uint32_t));

        for( uint16_t i = 0; i < lengths_.size(); ++i )
        {
            size += lengths_[ i ];
        }

        set_size( size );
    }

    void set_res_size()
    {
        size_t size = sizeof( msgCreateProgramWithBinary_response ) - sizeof(int16_t);

        size += binary_status_.size() * sizeof(int16_t);

        set_response_size( size );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgCreateProgramWithBinary_request
    {
        dcl::remote_id_t context_id_;
        uint16_t devices_count_;

        uint8_t buffer_[1];
    };

    struct msgCreateProgramWithBinary_response
    {
        dcl::remote_id_t program_id_;
        uint16_t status_count_;

        uint16_t binary_status_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
// msgBuildProgram
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgBuildProgram > : public base_message
{
public:
    dcl_message< msgBuildProgram >() : 
        base_message( msgBuildProgram, false, 0, 0 ), program_id_( INVALID_REMOTE_ID ) {}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, program_id_, program_id )
    MSG_PARAMETER_GET( std::string&, build_options_, build_options )
    MSG_PARAMETER_GET( dcl::remote_ids_t&, devices_, devices )

    inline void set_build_options( const std::string& build_options )
    {
        build_options_.assign( build_options );
        update_request_size();
    }

    void add_device( dcl::remote_id_t device_id )
    {
        devices_.push_back( device_id );
    }

private:
    std::string build_options_;
    dcl::remote_id_t program_id_;
    dcl::remote_ids_t devices_;

    virtual void create_request( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );

    inline void update_request_size()
    {
        set_size( build_options_.length() +
                  devices_.size() * sizeof( dcl::remote_id_t ) +
                  sizeof( msgBuildProgram_request ) - 1 );
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgBuildProgram_request
    {
        dcl::remote_id_t program_id_;
        uint16_t devices_count_;
        uint32_t build_options_len_;

        uint8_t buffer_[1];
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
// msgGetProgramBuildInfo
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgGetProgramBuildInfo > : public base_message
{
public:
    dcl_message< msgGetProgramBuildInfo >() : 
        base_message( msgGetProgramBuildInfo, true, sizeof(msgGetProgramBuildInfo_request), 0 ) {}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, remote_id_, remote_id )
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, device_id_, device_id )
    MSG_PARAMETER_GET_SET( cl_program_build_info, build_info_, build_info )

    // Response
    MSG_PARAMETER_GET( std::string&, build_log_, build_log )
    MSG_PARAMETER_GET( cl_build_status, build_status_, build_status )

    inline void set_build_log( const std::string& build_log )
    {
        build_log_.assign( build_log );

        update_response_size();
    }

    inline void set_build_status( cl_build_status build_status )
    {
        build_status_ = build_status;

        update_response_size();
    }

private:
    dcl::remote_id_t remote_id_;
    dcl::remote_id_t device_id_;
    cl_program_build_info build_info_;

    std::string build_log_;
    cl_build_status build_status_;

    virtual void create_request( void* payload_ptr );
    virtual void create_response( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
    virtual void parse_response( const void* payload_ptr );

    inline void update_response_size()
    {
        if( build_info_ == CL_PROGRAM_BUILD_STATUS )
        {
            set_response_size( sizeof(uint32_t) );
        }
        else if( build_info_ == CL_PROGRAM_BUILD_LOG )
        {
            set_response_size( sizeof(uint32_t) + build_log_.length() );
        }
    }

    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgGetProgramBuildInfo_request
    {
        dcl::remote_id_t id_;
        dcl::remote_id_t device_id_;
        uint32_t build_info_;
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_PROGRAM_MESSAGES_H_
