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
#include "event.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
event::~event()
{
    opencl_.clReleaseEvent( id_ );
}
//-----------------------------------------------------------------------------
void event::wait()
{
    cl_int error_code = opencl_.clWaitForEvents( 1, &id_ );

    if( error_code != CL_SUCCESS )
        throw dcl::library_exception( error_code );
}
//-----------------------------------------------------------------------------
void event::load_info()
{
    cl_int error_code;

    if( local_info_.queued_ == 0 )
    {
        error_code =
            opencl_.clGetEventProfilingInfo( id_, CL_PROFILING_COMMAND_QUEUED,
                                             sizeof(uint64_t), &local_info_.queued_, NULL );

        if( error_code != CL_SUCCESS )
            throw dcl::library_exception( error_code );
    }

    if( local_info_.submit_ == 0 )
    {
        error_code =
            opencl_.clGetEventProfilingInfo( id_, CL_PROFILING_COMMAND_SUBMIT,
                                             sizeof(uint64_t), &local_info_.submit_, NULL );

        if( error_code != CL_SUCCESS )
            throw dcl::library_exception( error_code );
    }

    if( local_info_.start_ == 0 )
    {
        error_code =
            opencl_.clGetEventProfilingInfo( id_, CL_PROFILING_COMMAND_START,
                                             sizeof(uint64_t), &local_info_.start_, NULL );

        if( error_code != CL_SUCCESS )
            throw dcl::library_exception( error_code );
    }

    if( local_info_.end_ == 0 )
    {
        error_code =
            opencl_.clGetEventProfilingInfo( id_, CL_PROFILING_COMMAND_END,
                                             sizeof(uint64_t), &local_info_.end_, NULL );

        if( error_code != CL_SUCCESS )
            throw dcl::library_exception( error_code );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
