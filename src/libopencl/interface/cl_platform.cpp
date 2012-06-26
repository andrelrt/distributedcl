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
#include "composite/opencl_composite.h"
#include "composite/composite_platform.h"
using dcl::icd::icd_object_manager;
using dcl::composite::opencl_composite;
using dcl::composite::composite_platform;
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetPlatformIDs( cl_uint num_entries, cl_platform_id* platforms,
                  cl_uint* num_platforms ) CL_API_SUFFIX__VERSION_1_0
{
    if( ( (num_entries == 0) && (platforms != NULL) ) ||
        ( (platforms == NULL) && (num_platforms == NULL) ) )
    {
        return CL_INVALID_VALUE;
    }

    if( num_platforms != NULL )
    {
        *num_platforms = 1;
    }

    if( platforms != NULL )
    {
        try
        {
            const composite_platform& plat = opencl_composite::get_instance().get_platform();
            *platforms = icd_object_manager::get_instance().get_cl_id< composite_platform >( const_cast< composite_platform* >( &plat ) );
        }
        catch( dcl::library_exception& ex )
        {
            return ex.get_error();
        }
        catch( ... )
        {
            return CL_INVALID_VALUE;
        }
    }

    return CL_SUCCESS;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetPlatformInfo( cl_platform_id platform, cl_platform_info param_name,
                   size_t param_value_size, void *param_value,
                   size_t *param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
{
    try
    {
        get_info< composite_platform >( platform, param_name, param_value_size, 
                                        param_value, param_value_size_ret );
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    return CL_SUCCESS;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clIcdGetPlatformIDsKHR( cl_uint num_entries, cl_platform_id* platforms,
                        cl_uint* num_platforms ) CL_API_SUFFIX__VERSION_1_0
{
    return clGetPlatformIDs( num_entries, platforms, num_platforms );
}
//-----------------------------------------------------------------------------
