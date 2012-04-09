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
#include <boost/scoped_array.hpp>
#include "program.h"
#include "device.h"
#include "library_exception.h"
#include "context.h"
#include "kernel.h"
using dcl::info::generic_program;
using dcl::info::generic_kernel;
using dcl::info::generic_device;
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
program::~program()
{
    opencl_.clReleaseProgram( id_ );
}
//-----------------------------------------------------------------------------
program::program( const context& context_ref, const std::string& source_code ) :
    generic_program( source_code ),
    opencl_object< cl_program >( context_ref.get_opencl() )
{
    if( opencl_.loaded() )
    {
        cl_int error_code;
        cl_program prog;

        size_t string_len = source_code.length();
        const char* strings = source_code.data();

        prog = opencl_.clCreateProgramWithSource( context_ref.get_id(), 1,
                                                  &strings, &string_len,
                                                  &error_code );
        if( error_code != CL_SUCCESS )
        {
            throw dcl::library_exception( error_code );
        }

        set_id( prog );
    }
}
//-----------------------------------------------------------------------------
void program::build( const std::string& build_options, cl_bool blocking )
{
    if( opencl_.loaded() )
    {
        cl_int error_code;

        local_info_.build_options_.assign( build_options );

        error_code = opencl_.clBuildProgram( id_, 0, NULL,
                                             build_options.c_str(),
                                             NULL, NULL );

        if( error_code != CL_SUCCESS )
        {
            throw library_exception( error_code );
        }
    }
}
//-----------------------------------------------------------------------------
void program::build( const devices_t& devices, const std::string& build_options, cl_bool blocking )
{
    if( opencl_.loaded() )
    {
        cl_int error_code;
        cl_uint device_count = static_cast< cl_uint >( devices.size() );

        local_info_.build_options_.assign( build_options );

        if( device_count != 0 )
        {
            boost::scoped_array< cl_device_id > device_list( new cl_device_id[ device_count ] );

            int i = 0;
            for( devices_t::const_iterator it = devices.begin(); it != devices.end(); it++ )
            {
                device_list[ i++ ] = reinterpret_cast<const device*>( *it )->get_id();
            }
        
            //TODO: Use the callback function
            error_code = opencl_.clBuildProgram( id_, device_count, device_list.get(), 
                                                 build_options.c_str(), NULL, NULL );
        }
        else
        {
            //TODO: Use the callback function
            error_code = opencl_.clBuildProgram( id_, 0, NULL, build_options.c_str(), NULL, NULL );
        }

        if( error_code != CL_SUCCESS )
        {
            throw library_exception( error_code );
        }
    }
}
//-----------------------------------------------------------------------------
generic_kernel* program::create_kernel( const std::string& kernel_name )
{
    return new kernel( *this, kernel_name );
}
//-----------------------------------------------------------------------------
cl_build_status program::get_build_status( const generic_device* device_ptr ) const
{
    cl_build_status build_status = CL_BUILD_NONE;

    if( opencl_.loaded() )
    {
        cl_int error_code;
        size_t param_value_size_ret;

        const device* dev_ptr = reinterpret_cast<const device*>( device_ptr );

        error_code = opencl_.clGetProgramBuildInfo( id_, dev_ptr->get_id(), CL_PROGRAM_BUILD_STATUS, 
                                                    sizeof( cl_build_status ), &build_status,
                                                    &param_value_size_ret );
        if( error_code != CL_SUCCESS )
        {
            throw library_exception( error_code );
        }
    }

    return build_status;
}
//-----------------------------------------------------------------------------
void program::get_build_log( const generic_device* device_ptr, std::string& str_log ) const
{
    str_log.clear();

    if( opencl_.loaded() )
    {
        cl_int error_code;
        size_t param_value_size_ret;

        const device* dev_ptr = reinterpret_cast<const device*>( device_ptr );

        error_code = opencl_.clGetProgramBuildInfo( id_, dev_ptr->get_id(), CL_PROGRAM_BUILD_LOG, 
                                                    0, NULL, &param_value_size_ret );

        if( error_code != CL_SUCCESS )
        {
            throw library_exception( error_code );
        }

        boost::scoped_array< char > log( new char[ param_value_size_ret +1 ] );

        error_code = opencl_.clGetProgramBuildInfo( id_, dev_ptr->get_id(), CL_PROGRAM_BUILD_LOG, 
                                                    param_value_size_ret, log.get(),
                                                    &param_value_size_ret );
        if( error_code != CL_SUCCESS )
        {
            throw library_exception( error_code );
        }

        str_log.assign( log.get() );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
