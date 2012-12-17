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
#include "kernel.h"
#include "command_queue.h"
#include "memory.h"
#include "device.h"
#include "event.h"
#include "sampler.h"
using dcl::info::ndrange;
using dcl::info::kernel_group_info;
using dcl::info::generic_command_queue;
using dcl::info::generic_memory;
using dcl::info::generic_image;
using dcl::info::generic_sampler;
using dcl::info::generic_device;
using dcl::info::generic_event;
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
kernel::kernel( const program& program_ref, const std::string& name ) :
    generic_kernel( name ),
    opencl_object< cl_kernel >( program_ref.get_opencl() )
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
kernel::~kernel()
{
    opencl_.clReleaseKernel( id_ );
}
//-----------------------------------------------------------------------------
void kernel::execute( const generic_command_queue* queue_ptr,
                      const ndrange& offset, const ndrange& global,
                      const ndrange& local, events_t& wait_events,
                      generic_event** event_ptr )
{
    const command_queue* queue = reinterpret_cast<const command_queue*>( queue_ptr );

    cl_int error_code;
    cl_event evnt;

    if( wait_events.empty() )
    {
        error_code =
            opencl_.clEnqueueNDRangeKernel( queue->get_id(), get_id(),
                                            static_cast<cl_uint>( global.get_dimensions() ),
                                            offset.get_pointer(), global.get_pointer(),
                                            local.get_pointer(), 0, NULL, &evnt );
    }
    else
    {
        boost::scoped_array<cl_event> events( new cl_event[wait_events.size()] );

        for( uint32_t i = 0; i < wait_events.size(); i++ )
        {
            events[ i ] = (reinterpret_cast<const event*>( wait_events[ i ] ))->get_id();
        }

        error_code =
            opencl_.clEnqueueNDRangeKernel( queue->get_id(), get_id(),
                                            static_cast<cl_uint>( global.get_dimensions() ),
                                            offset.get_pointer(), global.get_pointer(), local.get_pointer(),
                                            static_cast<cl_uint>( wait_events.size() ), events.get(), &evnt );
    }

    if( error_code != CL_SUCCESS )
    {
        throw dcl::library_exception( error_code );
    }

    if( (event_ptr != NULL) && (evnt != NULL) )
    {
        *event_ptr = new event( opencl_, queue_ptr, evnt );
    }
    else if( evnt != NULL )
    {
        opencl_.clReleaseEvent( evnt );
    }
}
//-----------------------------------------------------------------------------
void kernel::set_argument( uint32_t arg_index, const generic_memory* memory_ptr )
{
    cl_mem mem = (reinterpret_cast<const memory*>( memory_ptr ))->get_id();
    //std::cerr << "arg memory: " << (void*) mem << std::endl;
    set_argument( arg_index, sizeof(cl_mem), &mem );
}
//-----------------------------------------------------------------------------
void kernel::set_argument( uint32_t arg_index, const generic_image* image_ptr )
{
    cl_mem mem = (reinterpret_cast<const image*>( image_ptr ))->get_id();
    //std::cerr << "arg image: " << (void*) mem << std::endl;
    set_argument( arg_index, sizeof(cl_mem), &mem );
}
//-----------------------------------------------------------------------------
void kernel::set_argument( uint32_t arg_index, const generic_sampler* sampler_ptr )
{
    cl_sampler samp = (reinterpret_cast<const sampler*>( sampler_ptr ))->get_id();
    //std::cerr << "arg sampler: " << (void*) samp << std::endl;
    set_argument( arg_index, sizeof(cl_sampler), &samp );
}
//-----------------------------------------------------------------------------
void kernel::set_argument( uint32_t arg_index, size_t arg_size, const void* arg_value )
{
    //std::cerr << "arg unknow: (int) " << *((uint32_t*) arg_value) << std::endl;
    cl_int error_code = opencl_.clSetKernelArg( get_id(), arg_index, arg_size, arg_value );

    if( error_code != CL_SUCCESS )
    {
        throw dcl::library_exception( error_code );
    }
}
//-----------------------------------------------------------------------------
const kernel_group_info& kernel::get_group_info( const generic_device* device_ptr )
{
    kernel_group_info_map_t::iterator it = kernel_group_info_map_.find( device_ptr );

    if( it != kernel_group_info_map_.end() )
    {
        return it->second;
    }

    const device* dev_ptr = reinterpret_cast<const device*>( device_ptr );
    cl_int error_code;

    kernel_group_info& info = kernel_group_info_map_[ device_ptr ];

    // CL_KERNEL_WORK_GROUP_SIZE
    error_code = opencl_.clGetKernelWorkGroupInfo( get_id(), dev_ptr->get_id(),
                                                   CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t),
                                                   &(info.work_group_size_), NULL );

    if( error_code == CL_SUCCESS )
    {
        // CL_KERNEL_COMPILE_WORK_GROUP_SIZE
        error_code = opencl_.clGetKernelWorkGroupInfo( get_id(), dev_ptr->get_id(),
                                                       CL_KERNEL_COMPILE_WORK_GROUP_SIZE, 3*sizeof(size_t),
                                                       info.compile_work_group_size_, NULL );
    }

    if( error_code == CL_SUCCESS )
    {
        // CL_KERNEL_LOCAL_MEM_SIZE
        error_code = opencl_.clGetKernelWorkGroupInfo( get_id(), dev_ptr->get_id(),
                                                       CL_KERNEL_LOCAL_MEM_SIZE, sizeof(cl_ulong),
                                                       &(info.local_mem_size_), NULL );
    }

    if( error_code == CL_SUCCESS )
    {
        // CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE
        error_code = opencl_.clGetKernelWorkGroupInfo( get_id(), dev_ptr->get_id(),
                                                       CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t),
                                                       &(info.preferred_work_group_size_multiple_), NULL );
    }

    if( error_code == CL_SUCCESS )
    {
        // CL_KERNEL_PRIVATE_MEM_SIZE
        error_code = opencl_.clGetKernelWorkGroupInfo( get_id(), dev_ptr->get_id(),
                                                       CL_KERNEL_PRIVATE_MEM_SIZE, sizeof(cl_ulong),
                                                       &(info.private_mem_size_), NULL );
    }

    if( error_code != CL_SUCCESS )
    {
        kernel_group_info_map_.erase( device_ptr );

        throw dcl::library_exception( error_code );
    }

    return info;
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
