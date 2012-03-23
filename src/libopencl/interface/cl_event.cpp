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
#include "composite/composite_event.h"
using dcl::composite::composite_event;
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clWaitForEvents( cl_uint num_events, const cl_event* event_list ) CL_API_SUFFIX__VERSION_1_0
{
    if( (num_events == 0) || (event_list == NULL) )
    {
        return CL_INVALID_VALUE;
    }

    try
    {
        events_t events;
        icd_object_manager& icd = icd_object_manager::get_instance();

        for( uint32_t i = 0; i < num_events; i++ )
        {
            events.push_back( icd.get_object_ptr< composite_event >( event_list[ i ] ) );
        }

        composite_event::wait( events );

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
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clGetEventInfo( cl_event event, cl_event_info param_name, size_t param_value_size,
//                void* param_value, size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
//{
//}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_event CL_API_CALL
//clCreateUserEvent( cl_context context, cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clRetainEvent( cl_event event ) CL_API_SUFFIX__VERSION_1_0
{
    return retain_object< composite_event >( event );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clReleaseEvent( cl_event event ) CL_API_SUFFIX__VERSION_1_0
{
    return retain_object< composite_event >( event );
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clSetUserEventStatus( cl_event event , cl_int execution_status ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clSetEventCallback( cl_event event, cl_int command_exec_callback_type,
//                    void (CL_CALLBACK* pfn_notify)(cl_event, cl_int, void *),
//                    void* user_data ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
