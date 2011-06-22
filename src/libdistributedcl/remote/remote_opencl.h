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
#ifndef _DCL_REMOTE_OPENCL_H_
#define _DCL_REMOTE_OPENCL_H_

#include "distributedcl_internal.h"
#include "object_manager.h"
#include "client/session_manager.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_device;
//-----------------------------------------------------------------------------
class remote_opencl
{
public:
    remote_opencl( dcl::network::client::session_manager::session_t& session_ref ) :
        session_ref_( session_ref ) {}

    ~remote_opencl(){}

    inline const devices_t& get_devices() const
    {
        if( devices_.empty() )
        {
            const_cast< remote_opencl* >( this )->load_devices();
        }

        return( devices_ );
    }

    inline dcl::network::client::session_manager::session_t& get_session() const
    {
        return session_ref_;
    }

private:
    devices_t devices_;
    object_manager< remote_device > device_manager_;
    dcl::network::client::session_manager::session_t& session_ref_;

    void load_devices();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_OPENCL_H_
