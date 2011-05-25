/*
 * Copyright (c) 2009-2010 André Tupinambá (andrelrt@gmail.com)
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
#include "client/session_manager.h"
#include "message/msg_device.h"
using dcl::network::client::session;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
remote_platform* remote_platform::instance_ptr_ = NULL;
//-----------------------------------------------------------------------------
void remote_platform::load_devices()
{
    session& client_session = dcl::network::client::session_manager::get_instance().get_session();

    dcl_message< msgGetDeviceIDs > message;

    session.send_message( &message );

    const transport_ids_t& device_ids = message.get_device_ids();

    for( transport_ids_t::const_iterator it = device_ids.begin(); it != device_ids.end(); it++ )
    {
        remote_device* device_ptr = new remote_device( *it );

        devices_.push_back( device_ptr );
    }


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
