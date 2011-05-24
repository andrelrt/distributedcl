/*
 * Copyright (c) 2009-2010 Andr� Tupinamb� (andrelrt@gmail.com)
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
#include "client/session.h"
#include "message/message.h"
using dcl::info::platform_info;
using dcl::network::client::session;
using dcl::network::message::msg_platform;
using dcl::network::message::msg_get_devices;
using dcl::network::message::msg_get_platform_ids;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
remote_platform* remote_platform::instance_ptr_ = NULL;
//-----------------------------------------------------------------------------
void remote_platform::load_devices()
{
    //msg_get_devices msg( 0 );
    //session_ref_.send_message( msg );

    //std::vector< remote_id_t >::const_iterator it;

    //for( it = msg.get_ids().begin(); it !=msg.get_ids().end(); it++ )
    //{
    //    remote_devices_.push_back( new remote_device( session_ref_, *it ) );
    //}
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
