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
#include "remote_platform.h"
#include "remote_device.h"
#include "remote_context.h"
#include "info/platform_info.h"
#include "network/session_manager.h"
#include "message/message.h"
#include "message/msg_device.h"
#include "message/msg_context.h"
using dcl::devices_t;
using dcl::info::generic_context;
using dcl::network::client::session_manager;
using dcl::network::message::base_message;
using dcl::network::message::dcl_message;
using dcl::network::message::msgGetDeviceIDs;
using dcl::network::message::msgCreateContext;
using dcl::network::message::msgCreateContextFromType;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
generic_context* remote_platform::create_context( const devices_t& devices ) const
{
    dcl_message< msgCreateContext >* msg_ptr = new dcl_message< msgCreateContext >();

    msg_ptr->set_device_count( static_cast<uint32_t>( devices.size() ) );

    for( devices_t::const_iterator it = devices.begin(); it != devices.end(); ++it )
    {
        msg_ptr->add_device( reinterpret_cast<remote_device*>( *it )->get_remote_id() );
    }

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    remote_context* context_ptr = new remote_context( this );
    context_ptr->set_remote_id( msg_ptr->get_remote_id() );

    return reinterpret_cast<generic_context*>( context_ptr );
}
//-----------------------------------------------------------------------------
generic_context* remote_platform::create_context( cl_device_type device_type ) const
{
    dcl_message< msgCreateContextFromType >* msg_ptr = new dcl_message< msgCreateContextFromType >();

    msg_ptr->set_device_type( device_type );

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    remote_context* context_ptr = new remote_context( this );
    context_ptr->set_remote_id( msg_ptr->get_remote_id() );

    return reinterpret_cast<generic_context*>( context_ptr );
}
//-----------------------------------------------------------------------------
void remote_platform::load_devices()
{
    dcl_message< msgGetDeviceIDs >* msg_ptr = new dcl_message< msgGetDeviceIDs >();

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    for( std::size_t i = 0; i < msg_ptr->get_cpu_count(); i++ )
    {
        remote_device* dev_ptr = remote_device::get_remote_device( this, msg_ptr->get_cpu_device( i ), static_cast< cl_device_type >( CL_DEVICE_TYPE_CPU ) );

        add_device( dev_ptr );
    }

    for( std::size_t i = 0; i < msg_ptr->get_gpu_count(); i++ )
    {
        remote_device* dev_ptr = remote_device::get_remote_device( this, msg_ptr->get_gpu_device( i ), static_cast< cl_device_type >( CL_DEVICE_TYPE_GPU ) );

        add_device( dev_ptr );
    }

    for( std::size_t i = 0; i < msg_ptr->get_accelerator_count(); i++ )
    {
        remote_device* dev_ptr = remote_device::get_remote_device( this, msg_ptr->get_accelerator_device( i ), static_cast< cl_device_type >( CL_DEVICE_TYPE_ACCELERATOR ) );

        add_device( dev_ptr );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
