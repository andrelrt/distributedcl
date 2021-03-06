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
#ifndef _DCL_REMOTE_CONTEXT_H_
#define _DCL_REMOTE_CONTEXT_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "remote_object.h"
#include "remote_platform.h"
#include "info/context_info.h"
#include "info/dcl_objects.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_context : 
    public dcl::info::generic_context,
    public remote_object< remote_context, dcl::network::message::msgReleaseContext >
{
public:
    remote_context( const remote_platform* platform_ptr ) :
        dcl::info::generic_context( *platform_ptr ),
        remote_object< remote_context, dcl::network::message::msgReleaseContext >( platform_ptr->get_session() ),
        platform_ptr_( platform_ptr ){}

    ~remote_context(){}

private:
    const remote_platform* platform_ptr_;

    virtual void load_devices();
    virtual dcl::info::generic_program* do_create_program( const std::string& source_code );
    virtual dcl::info::generic_program* do_create_program( const dcl::devices_t& devs, const size_t* lengths,
                                                           const unsigned char** binaries, cl_int* binary_status );
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
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_CONTEXT_H_
