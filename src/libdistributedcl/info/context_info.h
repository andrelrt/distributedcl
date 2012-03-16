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
class generic_memory;
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

        programs_.push_back( program_ptr );

        return program_ptr;
    }

    inline generic_command_queue* create_command_queue( const generic_device* device_ptr, cl_command_queue_properties properties )
    {
        generic_command_queue* queue_ptr = do_create_command_queue( device_ptr, properties );

        queues_.push_back( queue_ptr );

        return queue_ptr;
    }

    inline generic_memory* create_buffer( const void* host_ptr, size_t size, cl_mem_flags flags )
    {
        generic_memory* buffer_ptr = do_create_buffer( host_ptr, size, flags );

        memory_objects_.push_back( buffer_ptr );

        return buffer_ptr;
    }

    inline const generic_platform& get_platform() const
    {
        return platform_;
    }

protected:
    typedef std::vector< generic_program* > programs_t;
    typedef std::vector< generic_command_queue* > queues_t;
    typedef std::vector< generic_memory* > memory_objects_t;

    queues_t queues_;
    devices_t devices_;
    programs_t programs_;
    memory_objects_t memory_objects_;
    const generic_platform& platform_;

    virtual void load_devices() = 0;
    virtual generic_program* do_create_program( const std::string& source_code ) = 0;

    virtual generic_command_queue*
        do_create_command_queue( const generic_device* device_ptr, 
                                 cl_command_queue_properties properties ) = 0;

    virtual generic_memory*
        do_create_buffer( const void* host_ptr, size_t size, cl_mem_flags flags ) = 0;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_CONTEXT_H_
