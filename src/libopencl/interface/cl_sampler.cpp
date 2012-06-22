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
#include "distributedcl_internal.h"
#include "cl_utils.h"
#include "icd/icd_object_manager.h"
#include "composite/composite_context.h"
#include "composite/composite_sampler.h"
using dcl::icd::icd_object_manager;
using dcl::composite::composite_sampler;
using dcl::composite::composite_context;
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_sampler CL_API_CALL
clCreateSampler( cl_context context, cl_bool normalized_coords,
                 cl_addressing_mode addressing_mode, cl_filter_mode filter_mode,
                 cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_0
{
    if( (addressing_mode < CL_ADDRESS_NONE) ||
        (addressing_mode > CL_ADDRESS_MIRRORED_REPEAT) ||
        ((filter_mode != CL_FILTER_NEAREST) && (filter_mode != CL_FILTER_LINEAR)) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_context* context_ptr =
            icd.get_object_ptr< composite_context >( context );

        composite_sampler* sampler_ptr = reinterpret_cast< composite_sampler* >(
            context_ptr->create_sampler( normalized_coords, addressing_mode, filter_mode ) );

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }

        return icd.get_cl_id< composite_sampler >( sampler_ptr );
    }
    catch( dcl::library_exception& ex )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = ex.get_error();
        }
        return NULL;
    }
    catch( ... )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    // Dummy
    if( errcode_ret != NULL )
    {
        *errcode_ret = CL_INVALID_VALUE;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clRetainSampler( cl_sampler sampler ) CL_API_SUFFIX__VERSION_1_0
{
    return retain_object< composite_sampler >( sampler );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clReleaseSampler( cl_sampler sampler ) CL_API_SUFFIX__VERSION_1_0
{
    return release_object< composite_sampler >( sampler );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetSamplerInfo( cl_sampler sampler, cl_sampler_info param_name,
                  size_t param_value_size, void* param_value,
                  size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
{
	return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
