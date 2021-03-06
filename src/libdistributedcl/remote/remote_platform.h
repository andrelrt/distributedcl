/*
 * Copyright (c) 2009-2012 Andr� Tupinamb� (andrelrt@gmail.com)
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
#ifndef _DCL_REMOTE_PLATFORM_H_
#define _DCL_REMOTE_PLATFORM_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <vector>
#include "distributedcl_internal.h"
#include "library_exception.h"
#include "remote_object.h"
#include "info/object_manager.h"
#include "info/dcl_objects.h"
#include "info/platform_info.h"
#include "info/context_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_device;
//-----------------------------------------------------------------------------
class remote_platform :
    public dcl::info::generic_platform,
    public remote_object< remote_platform, dcl::network::message::msg_dummy_message >
{
public:
    remote_platform( dcl::network::client::session_manager::session_t& session_ref ) : 
        remote_object< remote_platform, dcl::network::message::msg_dummy_message >( session_ref )
    {
        load_devices();
    }

    ~remote_platform(){}

    dcl::info::generic_context* create_context( const dcl::devices_t& devices ) const;
    dcl::info::generic_context* create_context( cl_device_type device_type = CL_DEVICE_TYPE_ALL ) const;

private:
    void load_devices();
    dcl::info::object_manager< remote_device > device_manager_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_PLATFORM_H_
