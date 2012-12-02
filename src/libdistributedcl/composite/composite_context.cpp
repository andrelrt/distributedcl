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
#include "composite_context.h"
#include "composite_program.h"
#include "composite_device.h"
#include "composite_command_queue.h"
#include "composite_memory.h"
#include "composite_sampler.h"
#include "info/program_info.h"
using dcl::info::generic_device;
using dcl::info::generic_context;
using dcl::info::generic_program;
using dcl::info::generic_sampler;
using dcl::info::generic_command_queue;
using dcl::info::generic_memory;
using dcl::info::generic_image;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
composite_context::composite_context() : 
    generic_context( opencl_composite::get_instance().get_platform() )
{
}
//-----------------------------------------------------------------------------
composite_context::~composite_context()
{
    for( contexts_t::iterator it = contexts_.begin(); it != contexts_.end(); ++it )
    {
        delete *it;
    }
    contexts_.clear();
    context_map_.clear();
}
//-----------------------------------------------------------------------------
void composite_context::add( generic_context* context_ptr, const devices_t& devices )
{
    contexts_.push_back( context_ptr );
    devices_.insert( devices_.end(), devices.begin(), devices.end() );

    for( devices_t::const_iterator it = devices.begin(); it != devices.end(); ++it )
    {
        context_map_.insert( context_map_t::value_type( *it, context_ptr ) );
    }
}
//-----------------------------------------------------------------------------
void composite_context::load_devices()
{
    devices_.clear();

    for( contexts_t::iterator it = contexts_.begin(); it != contexts_.end(); ++it )
    {
        add( *it, (*it)->get_devices() );
    }
}
//-----------------------------------------------------------------------------
generic_program* composite_context::do_create_program( const std::string& source_code )
{
    contexts_t::iterator it;

    composite_program* programs = new composite_program( *this, source_code );

    for( it = contexts_.begin(); it != contexts_.end(); ++it )
    {
        generic_program* program_ptr = (*it)->create_program( source_code );

        programs->insert_context_object( *it, program_ptr );
    }

    return reinterpret_cast< generic_program* >( programs );
}
//-----------------------------------------------------------------------------
generic_command_queue*
    composite_context::do_create_command_queue( const generic_device* device_ptr,
                                                cl_command_queue_properties properties )
{
    context_map_t::iterator it = context_map_.find( device_ptr );

    if( it == context_map_.end() )
    {
        throw dcl::library_exception( CL_INVALID_DEVICE );
    }

    return it->second->create_command_queue( device_ptr, properties );
}
//-----------------------------------------------------------------------------
generic_memory*
composite_context::do_create_buffer( const void* host_ptr, size_t size, cl_mem_flags flags )
{
    contexts_t::iterator it;

    composite_memory* memories = new composite_memory( *this, host_ptr, size, flags );

    for( it = contexts_.begin(); it != contexts_.end(); ++it )
    {
        generic_memory* memory_ptr = (*it)->create_buffer( host_ptr, size, flags );

        memories->insert_context_object( *it, memory_ptr );
    }

    return reinterpret_cast< generic_memory* >( memories );
}
//-----------------------------------------------------------------------------
generic_image*
composite_context::do_create_image( const void* host_ptr, cl_mem_flags flags,
                                    const cl_image_format* format, size_t width,
                                    size_t height, size_t row_pitch )
{
    contexts_t::iterator it;

    composite_image* images = new composite_image( *this, host_ptr, flags, format,
                                                   width, height, row_pitch );

    for( it = contexts_.begin(); it != contexts_.end(); ++it )
    {
        generic_image* image_ptr = (*it)->create_image( host_ptr, flags, format,
                                                        width, height, row_pitch );

        images->insert_context_object( *it, image_ptr );
    }

    return reinterpret_cast< generic_image* >( images );
}
//-----------------------------------------------------------------------------
generic_sampler*
composite_context::do_create_sampler( cl_bool normalized_coords, cl_addressing_mode addressing_mode,
                                      cl_filter_mode filter_mode )
{
    contexts_t::iterator it;

    composite_sampler* samplers = new composite_sampler( *this, normalized_coords, addressing_mode, filter_mode );

    for( it = contexts_.begin(); it != contexts_.end(); ++it )
    {
        generic_sampler* sampler_ptr = (*it)->create_sampler( normalized_coords, addressing_mode, filter_mode );

        samplers->insert_context_object( *it, sampler_ptr );
    }

    return reinterpret_cast< generic_sampler* >( samplers );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
