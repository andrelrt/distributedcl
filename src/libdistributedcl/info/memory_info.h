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
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

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
    cl_image_format format_;
    size_t element_size_;
    size_t row_pitch_;
    size_t slice_pitch_;
    size_t width_;
    size_t height_;
    size_t depth_;

    inline size_t get_info_size( cl_device_info info ) const
    {
        switch( info )
        {
            case CL_MEM_TYPE:       return sizeof(cl_mem_object_type);
            case CL_MEM_FLAGS:      return sizeof(cl_mem_flags);
            case CL_MEM_HOST_PTR:   return sizeof(void*);
            case CL_MEM_MAP_COUNT:  return sizeof(cl_uint);
            case CL_IMAGE_FORMAT:   return sizeof(cl_image_format);

            case CL_MEM_SIZE:
            case CL_IMAGE_ELEMENT_SIZE:
            case CL_IMAGE_ROW_PITCH:
            case CL_IMAGE_SLICE_PITCH:
            case CL_IMAGE_WIDTH:
            case CL_IMAGE_HEIGHT:
            case CL_IMAGE_DEPTH:
                return sizeof(size_t);

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }

    inline const void* get_info_pointer( cl_device_info info ) const
    {
        switch( info )
        {
            case CL_MEM_TYPE:           return &type_;
            case CL_MEM_FLAGS:          return &flags_;
            case CL_MEM_SIZE:           return &size_;
            case CL_MEM_HOST_PTR:       return &host_ptr_;
            case CL_MEM_MAP_COUNT:      return &map_count_;
            case CL_IMAGE_FORMAT:       return &format_;
            case CL_IMAGE_ELEMENT_SIZE: return &element_size_;
            case CL_IMAGE_ROW_PITCH:    return &row_pitch_;
            case CL_IMAGE_SLICE_PITCH:  return &slice_pitch_;
            case CL_IMAGE_WIDTH:        return &width_;
            case CL_IMAGE_HEIGHT:       return &height_;
            case CL_IMAGE_DEPTH:        return &depth_;

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }
};
//-----------------------------------------------------------------------------
class generic_memory_object :
    public cl_object< cl_mem, cl_mem_info, CL_INVALID_MEM_OBJECT >,
    public dcl_object< memory_info >
{
public:
    virtual ~generic_memory_object(){}
    inline void load_info(){}

    virtual void unmap( generic_command_queue* queue_ptr, void* data_ptr,
                        events_t& wait_events, generic_event** ret_event_ptr ) = 0;

    inline cl_mem_object_type get_type() const
    {
        return local_info_.type_;
    }

protected:
    generic_memory_object() : map_count_( 0 ){}

    void set_info( const void* host_ptr, size_t size, cl_mem_flags flags )
    {
        local_info_.type_ = CL_MEM_OBJECT_BUFFER;
        local_info_.host_ptr_ = host_ptr;
        local_info_.size_ = size;
        local_info_.flags_ = flags;
        local_info_.map_count_ = 0;
    }

    void set_info( const void* host_ptr, cl_mem_flags flags,
                   const cl_image_format* format, size_t width,
                   size_t height, size_t row_pitch )
    {
        local_info_.type_ = CL_MEM_OBJECT_IMAGE2D;
        local_info_.host_ptr_ = host_ptr;
        local_info_.size_ = row_pitch * height;
        local_info_.flags_ = flags;
        local_info_.map_count_ = 0;
        local_info_.format_ = *format;
        local_info_.width_ = width;
        local_info_.height_ = height;
        local_info_.row_pitch_ = row_pitch;
    }
    
private:
    uint32_t map_count_;
};
//-----------------------------------------------------------------------------
class generic_memory :
    public icd_object< cl_mem, dcl_memory_id >,
    public generic_memory_object
{
public:
    virtual ~generic_memory(){}

    virtual void write( generic_command_queue* queue_ptr, const void* data_ptr,
                        size_t size, size_t offset, cl_bool blocking,
                        events_t& wait_events, generic_event** ret_event_ptr ) = 0;

    virtual void read( generic_command_queue* queue_ptr, void* data_ptr,
                       size_t size, size_t offset, cl_bool blocking,
                       events_t& wait_events, generic_event** ret_event_ptr ) = 0;

    virtual void* map( generic_command_queue* queue_ptr, cl_map_flags flags,
                       size_t size, size_t offset, cl_bool blocking,
                       events_t& wait_events, generic_event** ret_event_ptr ) = 0;

    virtual void copy( generic_command_queue* queue_ptr, generic_memory* src_ptr,
                       size_t size, size_t src_offset, size_t dst_offset,
                       events_t& wait_events, generic_event** ret_event_ptr ) = 0;

protected:
    generic_memory(){}
};
//-----------------------------------------------------------------------------
class generic_image :
    public icd_object< cl_mem, dcl_image_id >,
    public generic_memory_object
{
public:
    virtual ~generic_image(){}

    virtual void write( generic_command_queue* queue_ptr, const void* data_ptr,
                        const size_t origin[3], const size_t region[3],
                        size_t input_row_pitch, size_t input_slice_pitch, bool blocking,
                        events_t& wait_events, generic_event** ret_event_ptr ) = 0;

protected:
    generic_image(){}
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_MEMORY_H_
