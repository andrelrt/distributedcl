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
#include "distributedcl_internal.h"
#include "cl_utils.h"
#include "icd/icd_object_manager.h"
#include "info/program_info.h"
#include "single/program.h"
#include "composite/composite_device.h"
#include "composite/composite_context.h"
#include "composite/composite_program.h"
using dcl::icd::icd_object_manager;
using dcl::composite::composite_device;
using dcl::composite::composite_context;
using dcl::composite::composite_program;
using dcl::info::program_info;
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithSource( cl_context context, cl_uint count,
                           const char** strings, const size_t* lengths,
                           cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_0
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

        composite_context* context_ptr =
            icd.get_object_ptr< composite_context >( context );

        composite_program* program_ptr =
            reinterpret_cast< composite_program* >( context_ptr->create_program( source_code ) );

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }

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
                           cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_0
{
    if( (num_devices == 0) || (device_list == NULL) || 
        (lengths == NULL)  || (binaries == NULL) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    for( size_t i = 0; i < num_devices; ++i )
    {
        if( (lengths[ i ] == 0) || (binaries[ i ] == NULL) )
        {
            if( errcode_ret != NULL )
            {
                *errcode_ret = CL_INVALID_VALUE;
            }
            return NULL;
        }
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_context* context_ptr =
            icd.get_object_ptr< composite_context >( context );

        dcl::devices_t program_devices;

        for( size_t i = 0; i < num_devices; ++i )
        {
            composite_device* device_ptr =
                icd.get_object_ptr< composite_device >( device_list[ i ] );

            if( !context_ptr->has_device( device_ptr ) )
            {
                if( errcode_ret != NULL )
                {
                    *errcode_ret = CL_INVALID_DEVICE;
                }
                return NULL;
            }

            program_devices.push_back( device_ptr );
        }

        composite_program* program_ptr =
            reinterpret_cast< composite_program* >( context_ptr->create_program( program_devices, lengths, binaries, binary_status ) );

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }

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
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clRetainProgram( cl_program program ) CL_API_SUFFIX__VERSION_1_0
{
    return retain_object< composite_program >( program );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clReleaseProgram( cl_program program ) CL_API_SUFFIX__VERSION_1_0
{
    return release_object< composite_program >( program );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clBuildProgram( cl_program program, cl_uint num_devices,
                const cl_device_id* device_list, const char* options,
                void (CL_CALLBACK* pfn_notify)( cl_program program, void* user_data ),
                void* user_data ) CL_API_SUFFIX__VERSION_1_0
{
    if( ((device_list == NULL) && (num_devices != 0)) ||
        ((device_list != NULL) && (num_devices == 0)) ||
        ((pfn_notify == NULL) && (user_data != NULL)) )
    {
        return CL_INVALID_VALUE;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_program* program_ptr = icd.get_object_ptr< composite_program >( program );

        std::string build_options( (options != NULL)? options : "" );

        if( num_devices != 0 )
        {
            dcl::devices_t devices;

            devices.reserve( num_devices );

            for( uint32_t i = 0; i < num_devices; i++ )
            {
                composite_device* device_ptr = icd.get_object_ptr< composite_device >( device_list[ i ] );
                devices.push_back( device_ptr );
            }

            program_ptr->build( devices, build_options );
        }
        else
        {
            program_ptr->build( build_options );
        }

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clUnloadCompiler( void ) CL_API_SUFFIX__VERSION_1_0
{
    // Nothing to do
    return CL_SUCCESS;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetProgramInfo( cl_program program, cl_program_info param_name,
                  size_t param_value_size, void* param_value,
                  size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
{
    // TODO: Not Implemented
    //return CL_OUT_OF_RESOURCES;

//#define CL_PROGRAM_REFERENCE_COUNT 
//#define CL_PROGRAM_CONTEXT         
//#define CL_PROGRAM_NUM_DEVICES     
//#define CL_PROGRAM_DEVICES         
//#define CL_PROGRAM_SOURCE          
//#define CL_PROGRAM_BINARY_SIZES    
//#define CL_PROGRAM_BINARIES        

    try
    {
        composite_program* program_ptr = 
            get_info_check_parameters< composite_program >( program, param_value_size, 
                                                            param_value, param_value_size_ret );
        size_t info_size = 0;

        switch( param_name )
        {
            case CL_PROGRAM_REFERENCE_COUNT:
                get_reference_count< composite_program >( program, param_value_size, 
                                                          param_value, param_value_size_ret );
                break;

            case CL_PROGRAM_CONTEXT:
                if( param_value != NULL )
                {
                    *(reinterpret_cast< cl_context* >( param_value )) = program_ptr->get_context().get_icd_obj();
                }

                info_size = sizeof( cl_context );
                break;

            case CL_PROGRAM_NUM_DEVICES:
                if( param_value != NULL )
                {
                    *(reinterpret_cast< cl_uint* >( param_value )) =
                        static_cast<cl_uint>( program_ptr->get_context().get_device_count() );
                }

                info_size = sizeof( cl_uint );
                break;

            case CL_PROGRAM_DEVICES:
            {
                size_t count = program_ptr->get_context().get_device_count();
                if( param_value != NULL )
                {
                    for( size_t i = 0; i < count; ++i )
                    {
                        (reinterpret_cast< cl_device_id* >( param_value ))[i] =
                                             (program_ptr->get_context().get_devices()[i])->get_icd_obj();
                    }
                }
                info_size = count * sizeof( cl_device_id );
                break;
            }

            case CL_PROGRAM_BINARY_SIZES:
            {
                //FIXME: Return the correct binary sizes
                size_t count = program_ptr->get_context().get_device_count();
                if( param_value != NULL )
                {
                    for( size_t i = 0; i < count; ++i )
                    {
                        (reinterpret_cast< cl_uint* >( param_value ))[i] = 1;
                    }
                }
                info_size = count * sizeof( cl_uint );
                break;
            }

            case CL_PROGRAM_BINARIES:
                //FIXME: Return the correct binaries
                info_size = program_ptr->get_context().get_device_count();
                break;

            default:
                return CL_INVALID_VALUE;

        }

        if( param_value_size_ret != NULL )
        {
            *param_value_size_ret = info_size;
        }

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetProgramBuildInfo( cl_program program, cl_device_id device,
                       cl_program_build_info param_name, size_t param_value_size,
                       void* param_value, size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
{
    try
    {
        composite_program* program_ptr = 
            get_info_check_parameters< composite_program >( program, param_value_size, 
                                                            param_value, param_value_size_ret );

        icd_object_manager& icd = icd_object_manager::get_instance();
        composite_device* device_ptr = icd.get_object_ptr< composite_device >( device );

        switch( param_name )
        {
            case CL_PROGRAM_BUILD_STATUS:
                if( param_value_size_ret != NULL )
                {
                    *param_value_size_ret = sizeof(cl_build_status);
                }

                if( (param_value != NULL) && 
                    (param_value_size >= sizeof(cl_build_status)) )
                {
                    *(reinterpret_cast< cl_build_status* >( param_value )) = 
                        program_ptr->get_build_status( device_ptr );
                }
                break;

            case CL_PROGRAM_BUILD_LOG:
            {
                std::string build_log;
                program_ptr->get_build_log( device_ptr, build_log );

                if( param_value_size_ret != NULL )
                {
                    *param_value_size_ret = build_log.length() + 1;
                }

                if( (param_value != NULL) &&
                    (param_value_size > build_log.length()) )
                {
                    memcpy( param_value, build_log.c_str(), build_log.length() + 1 );
                }
                break;
            }

            case CL_PROGRAM_BUILD_OPTIONS:
                get_info< composite_program >( program, param_name, param_value_size, 
                                               param_value, param_value_size_ret );
                break;
        }

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
