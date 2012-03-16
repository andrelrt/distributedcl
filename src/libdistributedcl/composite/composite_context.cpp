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
#include "composite_context.h"
#include "composite_program.h"
#include "composite_device.h"
#include "composite_command_queue.h"
#include "composite_memory.h"
#include "info/program_info.h"
using dcl::info::generic_device;
using dcl::info::generic_context;
using dcl::info::generic_program;
using dcl::info::generic_command_queue;
using dcl::info::generic_memory;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
composite_context::composite_context() : 
    generic_context( opencl_composite::get_instance().get_platform() )
{
}
//-----------------------------------------------------------------------------
void composite_context::add( generic_context* context_ptr, const devices_t& devices )
{
    contexts_.push_back( context_ptr );
    devices_.insert( devices_.end(), devices.begin(), devices.end() );

    for( devices_t::const_iterator it = devices.begin(); it != devices.end(); it++ )
    {
        context_map_.insert( context_map_t::value_type( *it, context_ptr ) );
    }
}
//-----------------------------------------------------------------------------
void composite_context::load_devices()
{
    devices_.clear();

    for( contexts_t::iterator it = contexts_.begin(); it != contexts_.end(); it++ )
    {
        add( *it, (*it)->get_devices() );
    }
}
//-----------------------------------------------------------------------------
generic_program* composite_context::do_create_program( const std::string& source_code )
{
    contexts_t::iterator it;

    composite_program* programs = new composite_program( *this, source_code );

    for( it = contexts_.begin(); it != contexts_.end(); it++ )
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
generic_memory* composite_context::do_create_buffer( const void* host_ptr, size_t size, cl_mem_flags flags )
{
    contexts_t::iterator it;

    composite_memory* memories = new composite_memory( *this );

    for( it = contexts_.begin(); it != contexts_.end(); it++ )
    {
        generic_memory* memory_ptr = (*it)->create_buffer( host_ptr, size, flags );

        memories->insert_context_object( *it, memory_ptr );
    }

    return reinterpret_cast< generic_memory* >( memories );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
