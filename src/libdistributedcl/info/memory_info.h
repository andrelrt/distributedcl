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
#ifndef _DCL_INFO_MEMORY_H_
#define _DCL_INFO_MEMORY_H_

#include "distributedcl_internal.h"
#include "library_exception.h"
#include "dcl_objects.h"
#include "icd_object.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
class generic_command_queue;
class generic_event;
//-----------------------------------------------------------------------------
struct memory_info
{
    cl_mem_object_type type_;
    cl_mem_flags flags_;
    size_t size_;
    const void* host_ptr_;
    cl_uint map_count_;

    inline size_t get_info_size( cl_device_info info ) const
    {
        switch( info )
        {
            case CL_MEM_TYPE:       return sizeof(cl_mem_object_type);
            case CL_MEM_FLAGS:      return sizeof(cl_mem_flags);
            case CL_MEM_SIZE:       return sizeof(size_t);
            case CL_MEM_HOST_PTR:   return sizeof(void*);
            case CL_MEM_MAP_COUNT:  return sizeof(cl_uint);

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }

    inline const void* get_info_pointer( cl_device_info info ) const
    {
        switch( info )
        {
            case CL_MEM_TYPE:       return &type_;
            case CL_MEM_FLAGS:      return &flags_;
            case CL_MEM_SIZE:       return &size_;
            case CL_MEM_HOST_PTR:   return &host_ptr_;
            case CL_MEM_MAP_COUNT:  return &map_count_;

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }
};
//-----------------------------------------------------------------------------
class generic_memory :
    public cl_object< cl_mem, cl_mem_info, CL_INVALID_MEM_OBJECT >,
    public icd_object< cl_mem, dcl_memory_id >,
    public dcl_object< memory_info >
{
public:
    generic_memory( cl_mem_object_type type, const void* host_ptr, 
                    size_t size, cl_mem_flags flags )
    {
        local_info_.type_ = type;
        local_info_.host_ptr_ = host_ptr;
        local_info_.size_ = size;
        local_info_.flags_ = flags;
        local_info_.map_count_ = 0;
    }

    virtual ~generic_memory(){}
    inline void load_info(){}

    virtual void write( generic_command_queue* queue_ptr, const void* data_ptr,
                        size_t size, size_t offset, cl_bool blocking,
                        events_t& wait_events, generic_event** ret_event_ptr ) = 0;

    virtual void read( generic_command_queue* queue_ptr, void* data_ptr,
                       size_t size, size_t offset, cl_bool blocking,
                       events_t& wait_events, generic_event** ret_event_ptr ) = 0;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_MEMORY_H_
