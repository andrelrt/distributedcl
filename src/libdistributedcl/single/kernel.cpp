/*
 * Copyright (c) 2009-2012 Andr� Tupinamb� (andrelrt@gmail.com)
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
#include "kernel.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
kernel::kernel( const program& program_ref, const std::string& name ) :
    generic_kernel( name ), opencl_object( program_ref.get_opencl() )
{
    if( !opencl_.loaded() )
    {
        throw library_exception( CL_INVALID_PROGRAM );
    }

    cl_int error_code;
    cl_kernel krnl = opencl_.clCreateKernel( program_ref.get_id(), name.c_str(), &error_code );

    if( error_code != CL_SUCCESS )
    {
        throw library_exception( error_code );
    }

    set_id( krnl );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------