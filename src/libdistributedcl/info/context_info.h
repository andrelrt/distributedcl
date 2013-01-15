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
#ifndef _DCL_INFO_CONTEXT_H_
#define _DCL_INFO_CONTEXT_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "dcl_objects.h"
#include "icd_object.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
class generic_program;
class generic_platform;
class generic_command_queue;
class generic_memory_object;
class generic_memory;
class generic_image;
class generic_sampler;
//-----------------------------------------------------------------------------
struct context_info
{
};
//-----------------------------------------------------------------------------
class generic_context :
    public cl_object< cl_context, cl_context_info, CL_INVALID_CONTEXT >,
    public icd_object< cl_context, dcl_context_id >,
    public dcl_object< context_info >
{
public:
    generic_context( const generic_platform& platform ) : 
        platform_( platform ){}
    generic_context( const generic_platform& platform, const devices_t& devices_ref ) : 
        devices_( devices_ref ), platform_( platform ){}

    virtual ~generic_context(){}

    inline size_t get_device_count() const
    {
        return devices_.size();
    }

    inline const devices_t& get_devices()
    {
        if( devices_.empty() )
        {
            load_devices();
        }

        return( devices_ );
    }

    inline generic_program* create_program( const std::string& source_code )
    {
        generic_program* program_ptr = do_create_program( source_code );

        scoped_lock_t lock( programs_mutex_ );
        programs_.push_back( program_ptr );

        return program_ptr;
    }

    inline generic_program* create_program( const dcl::devices_t& devs, const size_t* lengths,
                                            const unsigned char** binaries, cl_int* binary_status )
    {
        generic_program* program_ptr = do_create_program( devs, lengths, binaries, binary_status );

        scoped_lock_t lock( programs_mutex_ );
        programs_.push_back( program_ptr );

        return program_ptr;
    }

    inline generic_command_queue* create_command_queue( const generic_device* device_ptr,
                                                        cl_command_queue_properties properties )
    {
        generic_command_queue* queue_ptr = do_create_command_queue( device_ptr, properties );

        scoped_lock_t lock( queues_mutex_ );
        queues_.push_back( queue_ptr );

        return queue_ptr;
    }

    inline generic_memory* create_buffer( const void* host_ptr, size_t size, cl_mem_flags flags )
    {
        generic_memory* buffer_ptr = do_create_buffer( host_ptr, size, flags );

        scoped_lock_t lock( memory_objects_mutex_ );
        memory_objects_.push_back( reinterpret_cast<generic_memory_object*>( buffer_ptr ) );

        return buffer_ptr;
    }

    // Creates Image2D objects
    inline generic_image* create_image( const void* host_ptr, cl_mem_flags flags,
                                        const cl_image_format* format, size_t width,
                                        size_t height, size_t row_pitch )
    {
        generic_image* image_ptr =
            do_create_image( host_ptr, flags, format, width, height, row_pitch );

        scoped_lock_t lock( memory_objects_mutex_ );
        memory_objects_.push_back(  reinterpret_cast<generic_memory_object*>( image_ptr ) );

        return image_ptr;
    }

    // TODO: Create Image3D objects


    inline generic_sampler* create_sampler( cl_bool normalized_coords,
                                            cl_addressing_mode addressing_mode,
                                            cl_filter_mode filter_mode )
    {
        generic_sampler* sampler_ptr =
            do_create_sampler( normalized_coords, addressing_mode, filter_mode );

        scoped_lock_t lock( samplers_mutex_ );
        samplers_.push_back( sampler_ptr );

        return sampler_ptr;
    }

    inline const generic_platform* get_platform() const
    {
        return &platform_;
    }

protected:
    typedef std::vector< generic_program* > programs_t;
    typedef std::vector< generic_command_queue* > queues_t;
    typedef std::vector< generic_memory_object* > memory_objects_t;
    typedef std::vector< generic_sampler* > samplers_t;
    
    queues_t queues_;
    devices_t devices_;
    samplers_t samplers_;
    programs_t programs_;
    memory_objects_t memory_objects_;
    const generic_platform& platform_;
    
    dcl::mutex_t programs_mutex_;
    dcl::mutex_t queues_mutex_;
    dcl::mutex_t memory_objects_mutex_;
    dcl::mutex_t samplers_mutex_;

    virtual void load_devices() = 0;
    virtual generic_program* do_create_program( const std::string& source_code ) = 0;
    virtual generic_program* do_create_program( const dcl::devices_t& devs, const size_t* lengths,
                                                const unsigned char** binaries, cl_int* binary_status ) = 0;

    virtual generic_command_queue*
        do_create_command_queue( const generic_device* device_ptr, 
                                 cl_command_queue_properties properties ) = 0;

    virtual generic_memory*
        do_create_buffer( const void* host_ptr, size_t size, cl_mem_flags flags ) = 0;

    // Creates Image2D objects
    virtual generic_image*
        do_create_image( const void* host_ptr, cl_mem_flags flags, const cl_image_format* format,
                         size_t width, size_t height, size_t row_pitch ) = 0;

    virtual generic_sampler*
        do_create_sampler( cl_bool normalized_coords, cl_addressing_mode addressing_mode,
                           cl_filter_mode filter_mode ) = 0;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_CONTEXT_H_
