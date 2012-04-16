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
#include "composite_memory.h"
#include "composite_event.h"
#include "composite_command_queue.h"
using dcl::info::generic_event;
using dcl::info::generic_memory;
using dcl::info::generic_context;
using dcl::info::generic_command_queue;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
// composite_memory
//-----------------------------------------------------------------------------
composite_memory::composite_memory( const composite_context& context_ref,
                                    const void* host_ptr, size_t size,
                                    cl_mem_flags flags ) :
    composite_object< generic_memory >( context_ref )
{
    set_info( host_ptr, size, flags );
}
//-----------------------------------------------------------------------------
void composite_memory::write( generic_command_queue* queue_ptr, const void* data_ptr, 
                              size_t size, size_t offset, cl_bool blocking,
                              events_t& wait_events, generic_event** ret_event_ptr )
{
    const generic_context* ctx = queue_ptr->get_context();
    generic_memory* memory_ptr = reinterpret_cast<generic_memory*>( find( ctx ) );

    memory_ptr->write( queue_ptr, data_ptr, size, offset,
                       blocking, wait_events, ret_event_ptr );
}
//-----------------------------------------------------------------------------
void composite_memory::read( generic_command_queue* queue_ptr, void* data_ptr, 
                             size_t size, size_t offset, cl_bool blocking,
                             events_t& wait_events, generic_event** ret_event_ptr )
{
    const generic_context* ctx = queue_ptr->get_context();
    generic_memory* memory_ptr = reinterpret_cast<generic_memory*>( find( ctx ) );

    memory_ptr->read( queue_ptr, data_ptr, size, offset,
                      blocking, wait_events, ret_event_ptr );
}
//-----------------------------------------------------------------------------
// composite_image
//-----------------------------------------------------------------------------
composite_image::composite_image( const composite_context& context_ref,
                                  const void* host_ptr, cl_mem_flags flags,
                                  const cl_image_format* format, size_t width,
                                  size_t height, size_t row_pitch ) :
    composite_object< generic_image >( context_ref )
{
    set_info( host_ptr, flags, format, width, height, row_pitch );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
