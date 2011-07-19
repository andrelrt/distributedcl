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
#include "remote_platform.h"
#include "remote_device.h"
#include "info/platform_info.h"
#include "client/session_manager.h"
#include "message/message.h"
#include "message/msg_device.h"
using dcl::info::generic_context;
using dcl::network::client::session_manager;
using dcl::network::message::base_message;
using dcl::network::message::dcl_message;
using dcl::network::message::msgGetDeviceIDs;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
generic_context* remote_platform::create_context( const dcl::devices_t& devices ) const
{
    throw library_exception( "Not implemented" );
    return NULL;
}
//-----------------------------------------------------------------------------
//void remote_platform::load_devices()
//{
//    session_manager::session_t& client_session = session_manager::get_session();
//
//    dcl_message< msgGetDeviceIDs > msg;
//
//    client_session.send_message( reinterpret_cast< base_message* >( &msg ) );
//
//    std::size_t device_count = msg.get_device_count();
//
//    for( std::size_t i = 0; i < device_count; i++ )
//    {
//        remote_devices_.push_back( new remote_device( client_session ) );
//    }
//}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
