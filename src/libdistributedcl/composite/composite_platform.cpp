/*
 * Copyright (c) 2009-2011 André Tupinambá (andrelrt@gmail.com)
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
#include "composite_platform.h"
#include "composite_context.h"
#include "info/device_info.h"
#include "info/context_info.h"
using dcl::info::generic_context;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
void composite_platform::get_devices( devices_t& devices, cl_device_type device_type )
{
    devices.clear();

    for( devices_t::const_iterator it = devices_.begin(); it != devices_.end(); it++ )
    {
        if( device_type & (*it)->get_type() )
        {
            devices.push_back( *it );
        }
    }
}
//-----------------------------------------------------------------------------
generic_context* composite_platform::create_context( const devices_t& devices )
{
    composite_context* composite_context_ptr = new composite_context();

    try
    {
        devices_t context_devices;
        for( platforms_t::const_iterator it = platforms_.begin(); it != platforms_.end(); it++ )
        {
            context_devices.clear();
    
            for( devices_t::const_iterator dev_it = devices.begin(); dev_it != devices.end(); dev_it++ )
            {
                if( (*dev_it)->get_platform() == *it )
                {
                    context_devices.push_back( *dev_it );
                }
            }

            if( context_devices.size() != 0 )
            {
                generic_context* context_ptr = (*it)->create_context( context_devices );

                composite_context_ptr->add( context_ptr, context_devices );
            }
        }
    }
    catch( library_exception& ex )
    {
        if( composite_context_ptr != NULL )
        {
            delete composite_context_ptr;
        }

        throw ex;
    }

    return composite_context_ptr;
}
//-----------------------------------------------------------------------------
//composite_context* composite_platform::create_context( cl_device_type device_type )
//{
//    composite_context* ctx_ptr = new composite_context();
//
//    try
//    {
//        const platforms_t& platforms_ref = comp_platform_.get_platforms();
//
//        for( platforms_t::const_iterator it = platforms_ref.begin(); it != platforms_ref.end(); it++ )
//        {
//            context* ctx = new context( it->second->get_opencl(), device_type, *(it->second) );
//
//            ctx_ptr->add( ctx, ctx->get_devices() );
//        }
//    }
//    catch( library_exception& ex )
//    {
//        if( ctx_ptr != NULL )
//        {
//            delete ctx_ptr;
//        }
//
//        throw ex;
//    }
//
//    return ctx_ptr;
//}
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
