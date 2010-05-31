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
#include "remote_device.h"
#include "client/session.h"
#include "message/message.h"
using dcl::network::message::msg_device;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
const remote_device::dcl_info_t& remote_device::get_info()
{
    if( !data_loaded_ )
    {
        msg_device msg( remote_id_ );
        session_ref_.send_message( msg );

        //TODO: Fill the device_info structure

        data_loaded_ = true;
    }

    return local_info_;
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
 