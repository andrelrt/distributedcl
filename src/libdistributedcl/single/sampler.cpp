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
#include "sampler.h"
#include "context.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
sampler::~sampler()
{
    opencl_.clReleaseSampler( id_ );
}
//-----------------------------------------------------------------------------
sampler::sampler( const context& context_ref, cl_bool normalized_coords,
                  cl_addressing_mode addressing_mode, cl_filter_mode filter_mode ) :
    generic_sampler( normalized_coords, addressing_mode, filter_mode ),
    opencl_object< cl_sampler >( context_ref.get_opencl() )
{
    if( opencl_.loaded() )
    {
        cl_int error_code;
        cl_sampler sampler_id;

        sampler_id =
            opencl_.clCreateSampler( context_ref.get_id(), normalized_coords,
                                     addressing_mode, filter_mode, &error_code );

        if( error_code != CL_SUCCESS )
        {
            throw dcl::library_exception( error_code );
        }

        set_id( sampler_id );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
