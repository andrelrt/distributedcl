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
#ifndef _DCL_REMOTE_DEVICE_H_
#define _DCL_REMOTE_DEVICE_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "remote_object.h"
#include "remote_platform.h"
#include "info/device_info.h"
#include "info/dcl_objects.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_device : 
    public dcl::info::generic_device,
    public remote_object< remote_device, dcl::network::message::msg_dummy_message >
{
public:
    ~remote_device()
    {
        loaded_devices_.erase( get_remote_id() );
    }

    static remote_device* get_remote_device( const remote_platform* platform_ptr, dcl::remote_id_t id );
    static remote_device* get_remote_device( const remote_platform* platform_ptr, dcl::remote_id_t id, cl_device_type type );

protected:
    virtual bool load_device_info();

    remote_device( const remote_platform* platform_ptr, cl_device_type type ) : 
        dcl::info::generic_device( platform_ptr, type ),
        remote_object< remote_device, dcl::network::message::msg_dummy_message >( platform_ptr->get_session() ) {}

    remote_device( const remote_platform* platform_ptr, dcl::remote_id_t id ) : 
        dcl::info::generic_device( platform_ptr ),
        remote_object< remote_device, dcl::network::message::msg_dummy_message >( platform_ptr->get_session() ) 
    {
        set_remote_id( id );
        load_device_info();
    }

    static std::map<dcl::remote_id_t, remote_device*> loaded_devices_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_DEVICE_H_
