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
#include "distributedcl_internal.h"
#include "cl_utils.h"
#include "icd/icd_object_manager.h"
#include "composite/composite_event.h"
using dcl::events_t;
using dcl::icd::icd_object_manager;
using dcl::composite::composite_event;
//-----------------------------------------------------------------------------
void load_events( dcl::events_t& events, cl_uint num_events_in_wait_list, 
                  const cl_event* event_wait_list )
{
    icd_object_manager& icd = icd_object_manager::get_instance();

    if( event_wait_list != NULL )
    {
        events.reserve( num_events_in_wait_list );

        for( uint32_t i = 0; i < num_events_in_wait_list; i++ )
        {
            events.push_back( icd.get_object_ptr< composite_event >( event_wait_list[ i ] ) );
        }
    }
}
//-----------------------------------------------------------------------------
