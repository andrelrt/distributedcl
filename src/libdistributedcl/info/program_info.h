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
//-----------------------------------------------------------------------------
struct program_info
{
public:
    std::string source_code_;
    std::string build_options_;

    inline program_info(){}

    inline program_info( const std::string& source_code ) :
        source_code_( source_code ) {}

    inline program_info( const program_info& info ) :
        source_code_( info.source_code_ ),
        build_options_( info.build_options_ )
    {}
};
//-----------------------------------------------------------------------------
class generic_program :
    public cl_object< cl_program, cl_program_info, CL_INVALID_PROGRAM >,
    public icd_object< cl_program, dcl_program_id >,
    public dcl_object< program_info >
{
public:
    generic_program(){}
    generic_program( const std::string& source_code )
    {
        local_info_.source_code_.assign( source_code );
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
    //virtual void build( const devices_t& devices, const std::string& build_options, cl_bool blocking = CL_TRUE ) = 0;

    //cl_build_status get_build_status( const device& dev );
    //void get_build_log( const device& dev, std::string& str_log );
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_PROGRAM_H_

