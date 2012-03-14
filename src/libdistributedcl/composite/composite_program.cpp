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
#include <set>
#include "composite_program.h"
#include "composite_context.h"
#include "info/device_info.h"
using dcl::info::generic_device;
using dcl::info::generic_context;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
void composite_program::build( const std::string& build_options, cl_bool blocking )
{
    for( iterator it = begin(); it != end(); it++ )
    {
        it->second->build( build_options, blocking );
    }
}
//-----------------------------------------------------------------------------
void composite_program::build( const devices_t& devices, const std::string& build_options, cl_bool blocking )
{
    typedef std::set< generic_device* > device_set_t;

    for( iterator it = begin(); it != end(); it++ )
    {
        devices_t build_devices;

        const devices_t& context_devices = it->first->get_devices();
        device_set_t device_set( context_devices.begin(), context_devices.end() );

        for( devices_t::const_iterator dev_it = devices.begin(); dev_it != devices.end(); dev_it++ )
        {
            if( device_set.find( *dev_it ) != device_set.end() )
            {
                build_devices.push_back( *dev_it );
            }
        }

        it->second->build( build_devices, build_options, blocking );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
