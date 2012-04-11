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
    context_id_( 0xffff ), id_( 0xffff ){}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, context_id_, context_id )
    MSG_PARAMETER_GET( std::string&, source_code_, source_code )

    inline void set_source_code( const std::string& source_code )
    {
        source_code_.assign( source_code );

        set_size( source_code.length() +  sizeof( msgCreateProgramWithSource_request ) - 1 );
    }

    // Response
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, id_, remote_id )

private:
    dcl::remote_id_t context_id_;
    std::string source_code_;

    dcl::remote_id_t id_;

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
// msgBuildProgram
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgBuildProgram > : public base_message
{
public:
    dcl_message< msgBuildProgram >() : 
        base_message( msgBuildProgram, false, 0, 0 ), program_id_( 0xffff ) {}

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
    inline const dcl::remote_id_t get_remote_id() const
    {
        return id_;
    }

    inline void set_remote_id( dcl::remote_id_t id )
    {
        id_ = id;
    }

    inline const dcl::remote_id_t get_device_id() const
    {
        return device_id_;
    }

    inline void set_device_id( dcl::remote_id_t id )
    {
        device_id_ = id;
    }

    inline cl_program_build_info get_build_info() const
    {
        return build_info_;
    }

    inline void set_build_info( cl_program_build_info build_info )
    {
        build_info_ = build_info;
    }


    // Response
    inline const std::string& get_build_log() const
    {
        return build_log_;
    }

    inline void set_build_log( const std::string& build_log )
    {
        build_log_.assign( build_log );

        update_response_size();
    }

    inline cl_build_status get_build_status() const
    {
        return build_status_;
    }

    inline void set_build_status( cl_build_status build_status )
    {
        build_status_ = build_status;

        update_response_size();
    }

private:
    dcl::remote_id_t id_;
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
        dcl::remote_id_t device_id_;
        uint16_t build_info_;
    };
    #pragma pack( pop )
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_PROGRAM_MESSAGES_H_
