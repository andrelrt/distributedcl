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
#include "composite_kernel.h"
#include "composite_command_queue.h"
#include "composite_memory.h"
#include "composite_device.h"
#include "composite_event.h"
#include "composite_sampler.h"
using dcl::info::ndrange;
using dcl::info::kernel_group_info;
using dcl::info::generic_event;
using dcl::info::generic_kernel;
using dcl::info::generic_memory;
using dcl::info::generic_image;
using dcl::info::generic_sampler;
using dcl::info::generic_device;
using dcl::info::generic_context;
using dcl::info::generic_command_queue;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
void composite_kernel::execute( const generic_command_queue* queue_ptr,
                                const ndrange& offset, const ndrange& global, 
                                const ndrange& local, events_t& wait_events,
                                generic_event** event_ptr )
{
    const generic_context* ctx = queue_ptr->get_context();
    generic_kernel* kernel_ptr = find( ctx );

    if( (event_ptr == NULL) || (*event_ptr == NULL) )
    {
        kernel_ptr->execute( queue_ptr, offset, global, local, wait_events, event_ptr );
    }
    else
    {
        generic_event* evnt_ptr = NULL;

        kernel_ptr->execute( queue_ptr, offset, global, local, wait_events, &evnt_ptr );

        reinterpret_cast<composite_event*>(*event_ptr)->add_event( ctx, evnt_ptr );
    }
}
//-----------------------------------------------------------------------------
void composite_kernel::set_argument( uint32_t arg_index, const generic_memory* memory_ptr )
{
    set_argument< composite_memory, generic_memory >( arg_index, memory_ptr );
}
//-----------------------------------------------------------------------------
void composite_kernel::set_argument( uint32_t arg_index, const generic_image* image_ptr )
{
    set_argument< composite_image, generic_image >( arg_index, image_ptr );
}
//-----------------------------------------------------------------------------
void composite_kernel::set_argument( uint32_t arg_index, const generic_sampler* sampler_ptr )
{
    set_argument< composite_sampler, generic_sampler >( arg_index, sampler_ptr );
}
//-----------------------------------------------------------------------------
void composite_kernel::set_argument( uint32_t arg_index, size_t arg_size, const void* arg_value )
{
    for( iterator it = begin(); it != end(); it++ )
    {
        it->second->set_argument( arg_index, arg_size, arg_value );
    }
}
//-----------------------------------------------------------------------------
const kernel_group_info& composite_kernel::get_group_info( const generic_device* device_ptr )
{
    const composite_device* dev_ptr =
        reinterpret_cast<const composite_device*>( device_ptr );

    for( iterator it = begin(); it != end(); it++ )
    {
        if( it->first->get_platform() == dev_ptr->get_platform() )
        {
            return it->second->get_group_info( device_ptr );
        }
    }

    throw dcl::library_exception( CL_INVALID_DEVICE );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
