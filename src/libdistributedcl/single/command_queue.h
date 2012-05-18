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
#ifndef _DCL_COMMAND_QUEUE_H_
#define _DCL_COMMAND_QUEUE_H_

#include "distributedcl_internal.h"
#include "single_object.h"
#include "opencl_library.h"
#include "info/command_queue_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
class command_queue;
class context;
class device;
//-----------------------------------------------------------------------------
class command_queue :
    public dcl::info::generic_command_queue,
    public opencl_object< cl_command_queue >,
    public context_object< command_queue >
{
public:
    command_queue( const context& context_ref, const device& device_ref, 
                   cl_command_queue_properties properties );

    ~command_queue();

    virtual void flush() const;
    virtual void finish() const;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
#endif //_DCL_COMMAND_QUEUE_H_
