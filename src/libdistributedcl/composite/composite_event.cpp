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
#include "composite_event.h"
using dcl::info::generic_context;
using dcl::info::generic_event;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
void composite_event::wait( events_t& events )
{
    for( events_t::iterator it = events.begin(); it != events.end(); ++it )
    {
        (*it)->wait();
    }
}
//-----------------------------------------------------------------------------
void composite_event::get_context_events( const generic_context* ctx,
                                          const events_t& wait_events,
                                          events_t& context_events )
{
    events_t::const_iterator it;
    for( it = wait_events.begin(); it != wait_events.end(); ++it )
    {
        const composite_event* comp_event_ptr =
            reinterpret_cast<const composite_event*>( *it );

        generic_event* event_ptr = comp_event_ptr->find( ctx );

        context_events.push_back( event_ptr );
    }
}
//-----------------------------------------------------------------------------
void composite_event::wait()
{
    wait_execute();

    for( iterator it = begin(); it != end(); ++it )
    {
        it->second->wait();
    }
}
//-----------------------------------------------------------------------------
void composite_event::load_info()
{
    if( (local_info_.end_ == 0)    ||
        (local_info_.start_ == 0)  ||
        (local_info_.submit_ == 0) ||
        (local_info_.queued_ == 0) )
    {
        iterator it = begin();
        it->second->load_info();
        local_info_ = it->second->get_info();
    }
}
//-----------------------------------------------------------------------------
void composite_event::wait_execute()
{
    dcl::scoped_lock_t lock( mutex_ );

    if( !executed_ )
    {
        semaphore_.wait();
        executed_ = true;
    }
}
//-----------------------------------------------------------------------------
void composite_event::add_event( const generic_context* context_ptr, generic_event* event_ptr )
{
   insert_context_object( const_cast<generic_context*>( context_ptr ), event_ptr );
   semaphore_.post();
}
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
