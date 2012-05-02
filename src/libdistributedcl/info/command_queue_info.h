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
#ifndef _DCL_INFO_COMMAND_QUEUE_H_
#define _DCL_INFO_COMMAND_QUEUE_H_

#include <string>
#include "distributedcl_internal.h"
#include "library_exception.h"
#include "dcl_objects.h"
#include "icd_object.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
class generic_device;
class generic_context;
//-----------------------------------------------------------------------------
struct command_queue_info
{
public:
    const generic_device* device_ptr_;
    const generic_context* context_ptr_;
    cl_command_queue_properties properties_;

    inline command_queue_info(){}

    inline command_queue_info( const generic_context* context_ptr, const generic_device* device_ptr, 
                               cl_command_queue_properties properties ) : 
        device_ptr_( device_ptr ), context_ptr_( context_ptr ), properties_( properties ){}
};
//-----------------------------------------------------------------------------
class generic_command_queue :
    public cl_object< cl_command_queue, cl_command_queue_info, CL_INVALID_COMMAND_QUEUE >,
    public icd_object< cl_command_queue, dcl_command_queue_id >,
    public dcl_object< command_queue_info >
{
public:
    virtual ~generic_command_queue(){}
    generic_command_queue( const generic_context* context_ptr, const generic_device* device_ptr,
                           cl_command_queue_properties properties )
    {
        local_info_.device_ptr_ = device_ptr;
        local_info_.context_ptr_ = context_ptr;
        local_info_.properties_ = properties;
    }

    inline const generic_device* get_device() const
    {
        return local_info_.device_ptr_;
    }

    inline const generic_context* get_context() const
    {
        return local_info_.context_ptr_;
    }

    inline cl_command_queue_properties get_properties() const
    {
        return local_info_.properties_;
    }

    virtual void flush() const = 0;
    virtual void finish() const = 0;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_COMMAND_QUEUE_H_
