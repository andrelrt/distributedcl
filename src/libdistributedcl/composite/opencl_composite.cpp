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
#include "opencl_composite.h"
#include "single/opencl_single.h"
#include "single/opencl_library.h"
#include "remote/remote_opencl.h"
#include "client/session_manager.h"
using dcl::single::platforms_t;
using dcl::single::opencl_single;
using dcl::single::opencl_library;
using dcl::remote::remote_opencl;
using dcl::network::client::session_manager;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
opencl_composite opencl_composite::instance_;
//-----------------------------------------------------------------------------
void opencl_composite::free_all()
{
    for( opencl_set_t::iterator it = opencl_set_.begin(); it != opencl_set_.end(); it++ )
    {
        delete( *it );
    }
    opencl_set_.clear();

    for( library_set_t::iterator it = library_set_.begin(); it != library_set_.end(); it++ )
    {
        delete( *it );
    }
    library_set_.clear();
}
//-----------------------------------------------------------------------------
void opencl_composite::add_library( const std::string& libpath )
{
    opencl_library* library_ptr = new opencl_library( libpath );
    opencl_single* opencl_ptr = new opencl_single( *library_ptr );

    library_set_.insert( library_ptr );
    opencl_set_.insert( opencl_ptr );

    const devices_t& single_devices = opencl_ptr->get_devices();
    devices_.insert( devices_.end(), single_devices.begin(), single_devices.end() );
}
//-----------------------------------------------------------------------------
void opencl_composite::add_remote( const std::string& connection_string )
{
    session_manager::session_t& session_ref = session_manager::create_session( connection_string );

    remote_opencl* remote_ptr = new remote_opencl( session_ref );
    
    const devices_t& remote_devices = remote_ptr->get_devices();
    devices_.insert( devices_.end(), remote_devices.begin(), remote_devices.end() );

    remote_set_.insert( remote_ptr );
}
//-----------------------------------------------------------------------------
void opencl_composite::get_devices( devices_t& devices, cl_device_type device_type )
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
//composite_context* opencl_composite::create_context( const devices_t& devices )
//{
//    composite_context* ctx_ptr = new composite_context();
//
//    try
//    {
//        const platforms_t& platforms_ref = comp_platform_.get_platforms();
//
//        devices_t context_devices;
//        for( platforms_t::const_iterator it = platforms_ref.begin(); it != platforms_ref.end(); it++ )
//        {
//            context_devices.clear();
//    
//            for( devices_t::const_iterator dev_it = devices.begin(); dev_it != devices.end(); dev_it++ )
//            {
//                if( (*dev_it)->get_platform() == *(it->second) )
//                {
//                    context_devices.push_back( *dev_it );
//                }
//            }
//
//            if( context_devices.size() != 0 )
//            {
//                context* ctx = new context( it->second->get_opencl(), context_devices, *(it->second) );
//
//                ctx_ptr->add( ctx, context_devices );
//            }
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
//composite_context* opencl_composite::create_context( cl_device_type device_type )
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
void opencl_composite::unload_compiler()
{
    for( opencl_set_t::iterator it = opencl_set_.begin(); it != opencl_set_.end(); it++ )
    {
        (*it)->unload_compiler();
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
