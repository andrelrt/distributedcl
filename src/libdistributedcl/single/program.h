/*
 * Copyright (c) 2009-2010 André Tupinambá (andrelrt@gmail.com)
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
#ifndef _OCG_PROGRAM_H_
#define _OCG_PROGRAM_H_

#include <map>
#include <string>
#include "opencl_internal.h"
#include "single_object.h"
//-----------------------------------------------------------------------------
namespace ocg 
{
//-----------------------------------------------------------------------------
class program;
//-----------------------------------------------------------------------------
template<>
struct context_wrapper< program >
{
    static void context_attach( context* context_ptr, program* program_ptr );
};
//-----------------------------------------------------------------------------
template<>
struct reference_wrapper< cl_program >
{
    //-------------------------------------------------------------------------
    static inline void retain( const opencl_library& opencl, cl_program prog )
    {
        if( opencl.loaded() )
            opencl.clRetainProgram( prog );
    }

    //-------------------------------------------------------------------------
    static inline void release( const opencl_library& opencl, cl_program prog )
    {
        if( opencl.loaded() )
            opencl.clReleaseProgram( prog );
    }
};
//-----------------------------------------------------------------------------
typedef std::map< device*, bytes_t > binary_map_t;

struct program_info
{
public:
    bool from_source_;
    std::string source_code_;
    std::string build_options_;
    binary_map_t binaries_;

    inline program_info( const std::string& source_code ) :
        from_source_( true ), source_code_( source_code ) {}

    inline program_info( const binary_map_t& binaries ) :
        from_source_( false ), binaries_( binaries ) {}

    inline program_info( const program_info& info ) :
        from_source_( info.from_source_ ),
        source_code_( info.source_code_ ),
        build_options_( info.build_options_ ),
        binaries_( info.binaries_ )
    {}
};
//-----------------------------------------------------------------------------
class program :
    public opencl_object< cl_program, cl_program_info, CL_INVALID_PROGRAM >,
    public context_object< program >,
    public single_object< program_info >
{
public:

    program( const opencl_library& opencl, const program_info& info ) :
        opencl_object< cl_program, cl_program_info, CL_INVALID_PROGRAM >( opencl ),
        program_info_( info ) {}

    ~program(){}

    // TODO: Create a version of build method using the pfn_notify callback
    void build( const std::string& build_options, cl_bool blocking = CL_TRUE );
    void build( const devices_t& devices, const std::string& build_options, cl_bool blocking = CL_TRUE );

private:
    const program_info& program_info_;

public:
    inline bool get_from_source() const
    {
        return program_info_.from_source_;
    }

    inline const std::string& get_source_code() const
    {
        return program_info_.source_code_;
    }

    inline const binary_map_t& get_binaries() const
    {
        return program_info_.binaries_;
    }

    inline const std::string& get_build_options() const 
    {
        return program_info_.build_options_;
    }

    cl_build_status get_build_status( const device& dev );
    void get_build_log( const device& dev, std::string& str_log );
};
//-----------------------------------------------------------------------------
} // namespace ocg

#endif //_OCG_PROGRAM_H_
