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
#include "info/program_info.h"
#include "single/program.h"
#include "composite/composite_context.h"
#include "composite/composite_program.h"
using dcl::icd::icd_object_manager;
using dcl::composite::composite_context;
using dcl::composite::composite_program;
using dcl::info::program_info;
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithSource( cl_context context, cl_uint count,
                           const char** strings, const size_t* lengths,
                           cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_1
{
    if( (count == 0) || (strings == NULL) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    try
    {
        std::string source_code;

        for( cl_uint i = 0; i < count; i++ )
        {
            if( strings[ i ] == NULL )
            {
                throw dcl::library_exception( CL_INVALID_VALUE );
            }

            if( (lengths != NULL) && 
                (lengths[ i ] != 0 ) )
            {
                source_code.append( strings[ i ], lengths[ i ] );
            }
            else
            {
                source_code.append( strings[ i ] );
            }
        }

        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_context* context_ptr = icd.get_object_ptr< composite_context >( context );
        composite_program* program_ptr = reinterpret_cast< composite_program* >( context_ptr->create_program( source_code ) );

        return icd.get_cl_id< composite_program >( program_ptr );
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
extern "C" CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBinary( cl_context context, cl_uint num_devices,
                           const cl_device_id* device_list, const size_t* lengths,
                           const unsigned char** binaries, cl_int* binary_status,
                           cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_1
{
    // Not supported
    if( errcode_ret != NULL )
    {
        *errcode_ret = CL_INVALID_VALUE;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clRetainProgram( cl_program program ) CL_API_SUFFIX__VERSION_1_1
{
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clReleaseProgram( cl_program program ) CL_API_SUFFIX__VERSION_1_1
{
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clBuildProgram( cl_program program, cl_uint num_devices,
                const cl_device_id* device_list, const char* options, 
                void (CL_CALLBACK* pfn_notify)( cl_program program, void* user_data ),
                void* user_data ) CL_API_SUFFIX__VERSION_1_1
{
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clUnloadCompiler( void ) CL_API_SUFFIX__VERSION_1_1
{
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetProgramInfo( cl_program program, cl_program_info param_name,
                  size_t param_value_size, void* param_value,
                  size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_1
{
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetProgramBuildInfo( cl_program program, cl_device_id device,
                       cl_program_build_info param_name, size_t param_value_size,
                       void* param_value, size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_1
{
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------