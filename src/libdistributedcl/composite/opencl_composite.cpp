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
#include <stdio.h>
#include "opencl_composite.h"
#include "single/opencl_single.h"
#include "single/opencl_library.h"
#include "remote/remote_opencl.h"
#include "network/session_manager.h"
using dcl::platforms_t;
using dcl::single::opencl_single;
using dcl::single::opencl_library;
using dcl::remote::remote_opencl;
using dcl::network::client::session_manager;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
opencl_composite* opencl_composite::instance_ptr_ = NULL;
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

    const platforms_t& platforms = opencl_ptr->get_platforms();

    for( platforms_t::const_iterator it = platforms.begin(); it != platforms.end(); it++ )
    {
        comp_platform_.add_platform( *it );
    }
}
//-----------------------------------------------------------------------------
void opencl_composite::add_remote( const std::string& connection_string )
{
    session_manager::session_t& session_ref = session_manager::create_session( connection_string );

    remote_opencl* remote_ptr = new remote_opencl( session_ref );

    const platforms_t& platforms = remote_ptr->get_platforms();

    for( platforms_t::const_iterator it = platforms.begin(); it != platforms.end(); it++ )
    {
        comp_platform_.add_platform( *it );
    }

    remote_set_.insert( remote_ptr );

}
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
