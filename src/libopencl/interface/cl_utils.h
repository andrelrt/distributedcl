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
#ifndef _DCL_CL_UTILS_H_
#define _DCL_CL_UTILS_H_

#include <string.h>
#include "distributedcl_internal.h"
#include "library_exception.h"
#include "icd/icd_object_manager.h"
//-----------------------------------------------------------------------------
void load_events( dcl::events_t& events, cl_uint num_events_in_wait_list, 
                  const cl_event* event_wait_list );
//-----------------------------------------------------------------------------
template< class DCL_TYPE_T >
cl_int retain_object( typename DCL_TYPE_T::cl_type_t id )
{
    try
    {
        dcl::icd::icd_object_manager::get_instance().retain< DCL_TYPE_T >( id );
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
template< class DCL_TYPE_T >
cl_int release_object( typename DCL_TYPE_T::cl_type_t id )
{
    try
    {
        dcl::icd::icd_object_manager::get_instance().release< DCL_TYPE_T >( id );
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
template< typename DCL_TYPE_T > inline
DCL_TYPE_T* get_info_check_parameters( const typename DCL_TYPE_T::cl_type_t cl_obj,
                                       size_t param_value_size, void *param_value, 
                                       size_t *param_value_size_ret )
{
    if( cl_obj == NULL )
    {
        throw dcl::library_exception( "cl_obj == NULL", DCL_TYPE_T::invalid_error_value );
    }

    if( ( (param_value_size == 0) && (param_value != NULL) ) ||
        ( (param_value == NULL) && (param_value_size_ret == NULL) ) )
    {
        throw dcl::library_exception( "Invalid param_value and param_value_size values.", CL_INVALID_VALUE );
    }

    return dcl::icd::icd_object_manager::get_instance().get_object_ptr< DCL_TYPE_T >( cl_obj );
}
//-----------------------------------------------------------------------------
template< typename DCL_TYPE_T >
void get_info( typename DCL_TYPE_T::cl_type_t cl_obj, typename DCL_TYPE_T::cl_type_info_t info, 
               size_t param_value_size, void *param_value, size_t *param_value_size_ret )
{
    DCL_TYPE_T* obj_ptr = 
        get_info_check_parameters< DCL_TYPE_T >( cl_obj, param_value_size, 
                                                 param_value, param_value_size_ret );

    obj_ptr->load_info();

    const typename DCL_TYPE_T::dcl_info_t& info_ptr = obj_ptr->get_info();

    size_t info_size = info_ptr.get_info_size( info );

    if( param_value_size_ret != NULL )
    {
        *param_value_size_ret = info_size;
    }

    if( param_value != NULL )
    {
        if( param_value_size < info_size )
        {
            throw dcl::library_exception( CL_INVALID_VALUE );
        }

        memset( param_value, 0, param_value_size );
        memcpy( param_value, info_ptr.get_info_pointer( info ), info_size );
    }
}
//-----------------------------------------------------------------------------
#endif // _DCL_CL_UTILS_H_
