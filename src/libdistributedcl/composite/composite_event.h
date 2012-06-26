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
#ifndef _DCL_COMPOSITE_EVENT_H_
#define _DCL_COMPOSITE_EVENT_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "composite_object.h"
#include "composite_command_queue.h"
#include "info/event_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
class composite_event :
    public dcl::info::generic_event,
    public composite_object< dcl::info::generic_event >
{
public:
    composite_event( const composite_context& context_ref, const dcl::info::generic_command_queue* queue_ptr ) :
        dcl::info::generic_event( queue_ptr ),
        composite_object< dcl::info::generic_event >( context_ref ), executed_( false ), semaphore_( 0 ){}

    ~composite_event(){}

    static void wait( events_t& events );
    static void get_context_events( const dcl::info::generic_context* ctx,
                                    const dcl::events_t& wait_events,
                                    dcl::events_t& context_events );

    virtual void wait();
    virtual void load_info();

    void wait_execute();
    void add_event( const dcl::info::generic_context* context_ptr, dcl::info::generic_event* event_ptr );

private:
    bool executed_;
    boost::interprocess::interprocess_semaphore semaphore_;
    boost::interprocess::interprocess_mutex mutex_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
#endif //_DCL_COMPOSITE_EVENT_H_
