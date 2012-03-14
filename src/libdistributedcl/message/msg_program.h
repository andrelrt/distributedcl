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
template<>
class dcl_message< msgCreateProgramWithSource > : public base_message
{
public:
    dcl_message< msgCreateProgramWithSource >() : 
        base_message( msgCreateProgramWithSource, true, 0, sizeof( dcl::remote_id_t ) ) {}

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

    inline const std::string& get_source_code() const
    {
        return source_code_;
    }

    inline void set_source_code( const std::string& source_code )
    {
        source_code_.assign( source_code );

        set_size( source_code.length() +  sizeof( msgCreateProgramWithSource_request ) - 1 );
    }

private:
    dcl::remote_id_t id_;
    dcl::remote_id_t context_id_;
    std::string source_code_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void create_response( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );
    virtual void parse_response( const base_message* message_ptr );

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
template<>
class dcl_message< msgBuildProgram > : public base_message
{
public:
    dcl_message< msgBuildProgram >() : 
        base_message( msgBuildProgram, true, 0, 0 ) {}

    inline const dcl::remote_id_t get_program_id() const
    {
        return program_id_;
    }

    inline void set_program_id( dcl::remote_id_t id )
    {
        program_id_ = id;
    }

    inline const std::string& get_build_options() const
    {
        return build_options_;
    }

    inline void set_build_options( const std::string& build_options )
    {
        build_options_.assign( build_options );

        update_request_size();
    }

    inline const dcl::remote_ids_t& get_devices() const
    {
        return devices_;
    }

    void set_devices( const devices_t& devices );

private:
    std::string build_options_;
    dcl::remote_id_t program_id_;
    dcl::remote_ids_t devices_;

    virtual void create_request( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );

    void update_request_size()
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
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_PROGRAM_MESSAGES_H_
