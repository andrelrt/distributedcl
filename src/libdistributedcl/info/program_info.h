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
#ifndef _DCL_INFO_PROGRAM_H_
#define _DCL_INFO_PROGRAM_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include "distributedcl_internal.h"
#include "library_exception.h"
#include "dcl_objects.h"
#include "icd_object.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
class generic_context;
class generic_kernel;
class generic_device;
//-----------------------------------------------------------------------------
struct program_info
{
public:
    dcl::devices_t devices_;
    const size_t* lengths_;
    const unsigned char** binaries_;
    std::string source_code_;
    std::string build_options_;

    inline program_info() : lengths_( NULL ), binaries_( NULL ) {}

    inline program_info( const std::string& source_code ) :
        lengths_( NULL ), binaries_( NULL ), source_code_( source_code ) {}

    inline program_info( const dcl::devices_t& devices,
                         const size_t* lengths, const unsigned char** binaries ) :
        devices_( devices ), lengths_( lengths ), binaries_( binaries ) {}

    inline program_info( const program_info& info ) :
        source_code_( info.source_code_ ),
        build_options_( info.build_options_ )
    {}

    inline size_t get_info_size( cl_program_info info ) const
    {
        switch( info )
        {
            case CL_PROGRAM_BUILD_OPTIONS: return build_options_.length();
        }

        throw library_exception( CL_INVALID_VALUE );
    }

    inline const void* get_info_pointer( cl_program_info info ) const
    {
        if( info == CL_PROGRAM_BUILD_OPTIONS )
            return build_options_.data();

        throw library_exception( CL_INVALID_VALUE );
    }

};
//-----------------------------------------------------------------------------
class generic_program :
    public cl_object< cl_program, cl_program_info, CL_INVALID_PROGRAM >,
    public icd_object< cl_program, dcl_program_id >,
    public dcl_object< program_info >
{
public:
    virtual ~generic_program(){}

    generic_program( const std::string& source_code )
    {
        local_info_.source_code_.assign( source_code );
    }

    generic_program( const dcl::devices_t& devices, const size_t* lengths, 
                     const unsigned char** binaries )
    {
        local_info_.devices_.assign( devices.begin(), devices.end() );
        local_info_.lengths_ = lengths;
        local_info_.binaries_ = binaries;
    }

    inline const std::string& get_source() const
    {
        return local_info_.source_code_;
    }

    inline const std::string& get_build_options() const
    {
        return local_info_.build_options_;
    }

    inline void set_build_options( const std::string& build_options )
    {
        local_info_.build_options_.assign( build_options );
    }

    virtual void build( const std::string& build_options, cl_bool blocking = CL_TRUE ) = 0;
    virtual void build( const devices_t& devices, const std::string& build_options, cl_bool blocking = CL_TRUE ) = 0;
    virtual generic_kernel* create_kernel( const std::string& kernel_name ) = 0;
    virtual cl_build_status get_build_status( const generic_device* device_ptr ) const = 0;
    virtual void get_build_log( const generic_device* device_ptr, std::string& build_log ) const = 0;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_PROGRAM_H_

