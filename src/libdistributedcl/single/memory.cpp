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
#include "memory.h"
#include "context.h"
#include "command_queue.h"
#include "event.h"
using dcl::info::generic_event;
using dcl::info::generic_command_queue;
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
memory_object::memory_object( const context& context_ref ) :
    opencl_object< cl_mem >( context_ref.get_opencl() )
{
}
//-----------------------------------------------------------------------------
memory_object::~memory_object()
{
    opencl_.clReleaseMemObject( id_ );
}
//-----------------------------------------------------------------------------
memory::memory( const context& context_ref, const void* host_ptr, size_t size, cl_mem_flags flags ) :
    memory_object( context_ref )
{
    if( opencl_.loaded() )
    {
        cl_int error_code;
        cl_mem mem;

        mem = opencl_.clCreateBuffer( context_ref.get_id(), flags, size, 
                                      const_cast<void*>( host_ptr ), &error_code );

        if( error_code != CL_SUCCESS )
        {
            throw dcl::library_exception( error_code );
        }

        set_id( mem );
    }
}
//-----------------------------------------------------------------------------
void memory::write( generic_command_queue* queue_ptr, const void* data_ptr, 
                    size_t size, size_t offset, cl_bool blocking,
                    events_t& wait_events, generic_event** ret_event_ptr )
{
    if( opencl_.loaded() )
    {
        cl_int error_code;

        command_queue* queue = reinterpret_cast<command_queue*>( queue_ptr );
        cl_event evnt;

        if( wait_events.empty() )
        {
            error_code =
                opencl_.clEnqueueWriteBuffer( queue->get_id(), get_id(),
                                              blocking, offset, size,
                                              data_ptr, 0, NULL,
                                              (ret_event_ptr == NULL) ? NULL : &evnt );
        }
        else
        {
            boost::scoped_array<cl_event> events( new cl_event[wait_events.size()] );

            for( uint32_t i = 0; i < wait_events.size(); i++ )
            {
                events[ i ] = (reinterpret_cast<const event*>( wait_events[ i ] ))->get_id();
            }

            error_code =
                opencl_.clEnqueueWriteBuffer( queue->get_id(), get_id(), blocking, offset, size,
                                              data_ptr, static_cast<cl_uint>( wait_events.size() ),
                                              events.get(), (ret_event_ptr == NULL) ? NULL : &evnt );
        }

        if( error_code != CL_SUCCESS )
        {
            throw dcl::library_exception( error_code );
        }

        if( (ret_event_ptr != NULL) && (evnt != NULL) )
        {
            *ret_event_ptr = new event( opencl_, evnt );
        }
    }
}
//-----------------------------------------------------------------------------
void memory::read( generic_command_queue* queue_ptr, void* data_ptr, 
                   size_t size, size_t offset, cl_bool blocking,
                   events_t& wait_events, generic_event** ret_event_ptr )
{
    if( opencl_.loaded() )
    {
        cl_int error_code;

        command_queue* queue = reinterpret_cast<command_queue*>( queue_ptr );
        cl_event evnt;

        if( wait_events.empty() )
        {
            error_code =
                opencl_.clEnqueueReadBuffer( queue->get_id(), get_id(),
                                             blocking, offset, size,
                                             data_ptr, 0, NULL,
                                             (ret_event_ptr == NULL) ? NULL : &evnt );
        }
        else
        {
            boost::scoped_array<cl_event> events( new cl_event[wait_events.size()] );

            for( uint32_t i = 0; i < wait_events.size(); i++ )
            {
                events[ i ] = (reinterpret_cast<const event*>( wait_events[ i ] ))->get_id();
            }

            error_code =
                opencl_.clEnqueueReadBuffer( queue->get_id(), get_id(), blocking, offset, size,
                                             data_ptr, static_cast<cl_uint>( wait_events.size() ),
                                             events.get(), (ret_event_ptr == NULL) ? NULL : &evnt );
        }

        if( error_code != CL_SUCCESS )
        {
            throw dcl::library_exception( error_code );
        }

        if( (ret_event_ptr != NULL) && (evnt != NULL) )
        {
            *ret_event_ptr = new event( opencl_, evnt );
        }
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
