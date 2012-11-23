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
#ifndef _DCL_COMPOSITE_CONTEXT_H_
#define _DCL_COMPOSITE_CONTEXT_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <map>
#include "distributedcl_internal.h"
#include "opencl_composite.h"
#include "info/dcl_objects.h"
#include "info/context_info.h"
#include "info/memory_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
class generic_device;
class generic_program;
class generic_sampler;
}}
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
class opencl_composite;
//-----------------------------------------------------------------------------
class composite_context :
    public dcl::info::generic_context
{
private:
    typedef std::vector< dcl::info::generic_context* > contexts_t;
    typedef std::map< const dcl::info::generic_device*, dcl::info::generic_context* > context_map_t;

    contexts_t contexts_;
    context_map_t context_map_;

public:
    typedef contexts_t::const_iterator iterator;

    static const char* get_name(){ return "composite_context"; }

    composite_context();
    ~composite_context();

    void add( generic_context* context_ptr, const devices_t& devices );

    size_t size() const
    {
        return contexts_.size();
    }

    void retain();
    void release();

    inline contexts_t::const_iterator begin() const
    {
        return contexts_.begin();
    }

    inline contexts_t::const_iterator end() const
    {
        return contexts_.end();
    }

    inline const dcl::info::generic_context* find( const dcl::info::generic_device* device_ptr ) const
    {
        context_map_t::const_iterator it = context_map_.find( device_ptr );

        if( it == context_map_.end() )
            throw dcl::library_exception( CL_INVALID_DEVICE );

        return it->second;
    }

private:
    virtual void load_devices();
    virtual dcl::info::generic_program* do_create_program( const std::string& source_code );
    virtual dcl::info::generic_command_queue*
        do_create_command_queue( const dcl::info::generic_device* device_ptr,
                                 cl_command_queue_properties properties );
    virtual dcl::info::generic_memory*
        do_create_buffer( const void* host_ptr, size_t size, cl_mem_flags flags );

    virtual dcl::info::generic_image*
        do_create_image( const void* host_ptr, cl_mem_flags flags, const cl_image_format* format,
                         size_t width, size_t height, size_t row_pitch );

    virtual dcl::info::generic_sampler*
        do_create_sampler( cl_bool normalized_coords, cl_addressing_mode addressing_mode,
                           cl_filter_mode filter_mode );
};
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
#endif //_DCL_COMPOSITE_PLATFORM_H_
