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
};
//-----------------------------------------------------------------------------
class generic_memory :
    public cl_object< cl_mem, cl_mem_info, CL_INVALID_MEM_OBJECT >,
    public icd_object< cl_mem, dcl_memory_id >,
    public dcl_object< memory_info >
{
public:
    generic_memory(){}
    virtual ~generic_memory(){}

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
