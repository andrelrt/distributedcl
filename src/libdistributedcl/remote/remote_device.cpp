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
#include "remote_device.h"
#include "network/client_session.h"
#include "message/msg_device.h"
using dcl::network::message::base_message;
using dcl::network::message::dcl_message;
using dcl::network::message::msgGetDeviceInfo;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
std::map<dcl::remote_id_t, remote_device*>* remote_device::loaded_devices_ = NULL;
//-----------------------------------------------------------------------------
remote_device* remote_device::get_remote_device( const remote_platform* platform_ptr, dcl::remote_id_t id )
{
    if( loaded_devices_ == NULL )
        loaded_devices_ = new std::map<dcl::remote_id_t, remote_device*>();

    if( loaded_devices_->find( id ) == loaded_devices_->end() )
    {
        (*loaded_devices_)[ id ] = new remote_device( platform_ptr, id );
    }
    return (*loaded_devices_)[ id ];
}
//-----------------------------------------------------------------------------
remote_device* remote_device::get_remote_device( const remote_platform* platform_ptr, dcl::remote_id_t id, cl_device_type type )
{
    if( loaded_devices_ == NULL )
        loaded_devices_ = new std::map<dcl::remote_id_t, remote_device*>();

    if( loaded_devices_->find( id ) == loaded_devices_->end() )
    {
        (*loaded_devices_)[ id ] = new remote_device( platform_ptr, type );
        (*loaded_devices_)[ id ]->set_remote_id( id );
    }
    return (*loaded_devices_)[ id ];
}
//-----------------------------------------------------------------------------
bool remote_device::load_device_info()
{
    dcl_message< msgGetDeviceInfo >* msg_ptr = new dcl_message< msgGetDeviceInfo >( get_remote_id() );

    message_sp_t message_sp( msg_ptr );
    session_ref_.send_message( message_sp );

    local_info_ = msg_ptr->get_info();
    //local_info_.name_.append( " (" );
    //local_info_.name_.append( session_ref_.get_connection_string() );
    //local_info_.name_.append( ")" );

    return true;
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
