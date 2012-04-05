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
#include "command_queue.h"
#include "context.h"
#include "device.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
command_queue::command_queue( const context& context_ref, const device& device_ref, 
                              cl_command_queue_properties properties ) :
    dcl::info::generic_command_queue( &context_ref, &device_ref, properties ), 
    opencl_object< cl_command_queue >( context_ref.get_opencl() )
{
    if( !opencl_.loaded() )
    {
        throw library_exception( CL_INVALID_CONTEXT );
    }

    cl_int error_code;
    cl_command_queue queue = 
        opencl_.clCreateCommandQueue( context_ref.get_id(), device_ref.get_id(), 
                                      properties, &error_code );

    if( error_code != CL_SUCCESS )
    {
        throw library_exception( error_code );
    }

    set_id( queue );
}
//-----------------------------------------------------------------------------
void command_queue::flush()
{
    cl_int error_code = opencl_.clFlush( get_id() );

    if( error_code != CL_SUCCESS )
    {
        throw library_exception( error_code );
    }
}
//-----------------------------------------------------------------------------
void command_queue::finish()
{
    cl_int error_code = opencl_.clFinish( get_id() );

    if( error_code != CL_SUCCESS )
    {
        throw library_exception( error_code );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
