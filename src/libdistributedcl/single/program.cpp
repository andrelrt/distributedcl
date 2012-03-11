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
#include "program.h"
#include "device.h"
#include "library_exception.h"
#include <boost/scoped_array.hpp>
//-----------------------------------------------------------------------------
namespace ocg
{
//-----------------------------------------------------------------------------
void context_wrapper< program >::context_attach( context* context_ptr, program* program_ptr )
{
    if( context_ptr->get_opencl().loaded() )
    {
        cl_int error_code;
        cl_program prog;

        if( program_ptr->get_from_source() )
        {
            const char *strings = program_ptr->get_source_code().c_str();

            prog = context_ptr->get_opencl().clCreateProgramWithSource( *context_ptr, 1, &strings,
                                                                        NULL, &error_code );
        }
        else
        {
            error_code = CL_INVALID_BINARY;
            //size_t devices = program_ptr->get_binaries().size();
            //cl_device_id *device_list = new cl_device_id[ devices ];
            //unsigned char** binaries = new unsigned char*[ devices ];

            //int i = 0;
            //for( program::binary_map_t::const_iterator it = program_ptr->get_binaries().begin(); 
            //     it != program_ptr->get_binaries().end(); 
            //     it++ )
            //{
            //    device_list[ i ] = it->first->get_id();
            //    binaries[ i ] = it->second.begin();

            //    i++;
            //}

            //prog = context_ptr->get_opencl().clCreateProgramWithBinary( *context_ptr, binaries_.size()
        }

        if( error_code != CL_SUCCESS )
        {
            throw library_exception( error_code );
        }

        program_ptr->set_id( prog );
    }
}
//-----------------------------------------------------------------------------
void program::build( const std::string& build_options, cl_bool blocking )
{
    if( get_opencl().loaded() )
    {
        cl_int error_code;

        //TODO: Use the callback function
        error_code = get_opencl().clBuildProgram( *this, 0, NULL, build_options.c_str(), NULL, NULL );

        if( error_code != CL_SUCCESS )
        {
            throw library_exception( error_code );
        }
    }
}
//-----------------------------------------------------------------------------
void program::build( const devices_t& devices, const std::string& build_options, cl_bool blocking )
{
    if( get_opencl().loaded() )
    {
        cl_int error_code;
        cl_uint device_count = static_cast< cl_uint >( devices.size() );
        boost::scoped_array< cl_device_id > device_list( new cl_device_id[ device_count ] );

        //program_info_.build_options_.assign( build_options );

        int i = 0;
        for( devices_t::const_iterator it = devices.begin(); it != devices.end(); it++ )
        {
            device_list[ i++ ] = (*it)->get_id();
        }
        
        //TODO: Use the callback function
        error_code = get_opencl().clBuildProgram( *this, device_count, device_list.get(), 
                                                  build_options.c_str(), NULL, NULL );

        if( error_code != CL_SUCCESS )
        {
            throw library_exception( error_code );
        }
    }
}
//-----------------------------------------------------------------------------
cl_build_status program::get_build_status( const device& dev )
{
    cl_build_status build_status = CL_BUILD_NONE;

    if( get_opencl().loaded() )
    {
        cl_int error_code;
        size_t param_value_size_ret;

        error_code = get_opencl().clGetProgramBuildInfo( *this, dev, CL_PROGRAM_BUILD_STATUS, 
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
void program::get_build_log( const device& dev, std::string& str_log )
{
    str_log.clear();

    if( get_opencl().loaded() )
    {
        cl_int error_code;
        size_t param_value_size_ret;

        error_code = get_opencl().clGetProgramBuildInfo( *this, dev, CL_PROGRAM_BUILD_LOG, 
                                                         0, NULL, &param_value_size_ret );
        if( error_code != CL_SUCCESS )
        {
            throw library_exception( error_code );
        }

        boost::scoped_array< char > log( new char[ param_value_size_ret +1 ] );

        error_code = get_opencl().clGetProgramBuildInfo( *this, dev, CL_PROGRAM_BUILD_LOG, 
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
} // namespace ocg
