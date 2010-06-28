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
#include "distributedcl_internal.h"
#include "cl_utils.h"
#include "local_session.h"
#include "remote/remote_platform.h"
using dcl::remote::remote_platform;
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

    try
    {
        dcl::opencl::local_session&  
        dcl::remote::remote_platforms_t remote_platforms;

        remote_platform::get_platforms( client_session, remote_platforms );

        cl_uint count = static_cast< cl_uint >( remote_platforms.size() );
        
        if( num_platforms != NULL )
        {
            *num_platforms = count;
        }

        if( platforms != NULL )
        {
            if( count > num_entries )
            {
                count = num_entries;
            }

            for( cl_uint i = 0; i < count; i++ )
            {
                platforms[ i ] = reinterpret_cast< cl_platform_id >( remote_platforms[ i ] );
            }
        }
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
clGetPlatformInfo( cl_platform_id platform, cl_platform_info param_name,
                   size_t param_value_size, void *param_value,
                   size_t *param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
{
    try
    {
        get_info< remote_platform >( platform, param_name, param_value_size, 
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
