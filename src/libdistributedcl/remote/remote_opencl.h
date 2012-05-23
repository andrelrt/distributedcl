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
#ifndef _DCL_REMOTE_OPENCL_H_
#define _DCL_REMOTE_OPENCL_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "remote_platform.h"
#include "network/session_manager.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_opencl
{
public:
    remote_opencl( dcl::network::client::session_manager::session_t& session_ref ) :
        session_ref_( session_ref ) 
    {
        remote_platform* platform_ptr = new remote_platform( session_ref_ );
        platforms_.push_back( reinterpret_cast< dcl::info::generic_platform* >( platform_ptr ) );
    }

    ~remote_opencl(){}

    inline const platforms_t& get_platforms() const
    {
        return platforms_;
    }

    inline dcl::network::client::session_manager::session_t& get_session() const
    {
        return session_ref_;
    }

private:
    platforms_t platforms_;
    dcl::network::client::session_manager::session_t& session_ref_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_OPENCL_H_
