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
#include "server_context.h"
#include "server_platform.h"
#include "message/message.h"
#include "message/msg_context.h"
#include "composite/opencl_composite.h"
#include "composite/composite_device.h"
#include "composite/composite_context.h"
using dcl::network::message::dcl_message;
using dcl::network::message::msgCreateContextFromType;
using dcl::composite::opencl_composite;
using dcl::composite::composite_device;
using dcl::composite::composite_context;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
void CreateContextFromType_command::execute()
{
    cl_device_type device_type = message_.get_device_type();

    composite_context* context_ptr = reinterpret_cast<composite_context*>( opencl_composite::get_instance().get_platform().create_context( device_type ) );

    remote_id_t id;
    try
    {
        id = server_platform::get_instance().get_context_manager().get( context_ptr );
    }
    catch( dcl::library_exception& )
    {
        id = server_platform::get_instance().get_context_manager().add( context_ptr );
    }

    message_.set_remote_id( id );
}
//-----------------------------------------------------------------------------
void GetContextInfo_command::execute()
{
    remote_id_t id = message_.get_remote_id();

    composite_context* context_ptr = server_platform::get_instance().get_context_manager().get( id );

    const devices_t& devices_ref = context_ptr->get_devices();

    message_.set_device_count( devices_ref.size() );

    devices_t::const_iterator it;
    remote_id_t* devices_ptr = message_.get_devices();

    for( it = devices_ref.begin(); it != devices_ref.end(); it++ )
    {
        try
        {
            *devices_ptr = server_platform::get_instance().get_device_manager().get( *it );
        }
        catch( dcl::library_exception& )
        {
            *devices_ptr = server_platform::get_instance().get_device_manager().add( *it );
        }

        devices_ptr++;
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
