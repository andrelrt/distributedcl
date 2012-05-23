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
#ifndef _DCL_INFO_EVENT_H_
#define _DCL_INFO_EVENT_H_
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
//-----------------------------------------------------------------------------
struct event_info
{
    uint64_t queued_;
    uint64_t submit_;
    uint64_t start_;
    uint64_t end_;

    event_info()
    {
        memset( this, 0, sizeof(event_info) );
    }

    inline size_t get_info_size( cl_profiling_info info ) const
    {
        return sizeof( uint64_t );
    }

    inline const void* get_info_pointer( cl_profiling_info info ) const
    {
        switch( info )
        {
            case CL_PROFILING_COMMAND_QUEUED: return &queued_;
            case CL_PROFILING_COMMAND_SUBMIT: return &submit_;
            case CL_PROFILING_COMMAND_START:  return &start_;
            case CL_PROFILING_COMMAND_END:    return &end_;

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }

};
//-----------------------------------------------------------------------------
class generic_event :
    public cl_object< cl_event, cl_event_info, CL_INVALID_EVENT >,
    public icd_object< cl_event, dcl_event_id >,
    public dcl_object< event_info >
{
public:
    generic_event( const generic_command_queue* queue_ptr ) :
        queue_ptr_( queue_ptr ){}
    virtual ~generic_event(){}

    static const char* get_name(){ return "generic_event"; }

    virtual void wait() = 0;
    virtual void load_info() = 0;

    inline const generic_command_queue* get_queue() const
    {
        return queue_ptr_;
    }

protected:
    const generic_command_queue* queue_ptr_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_EVENT_H_
