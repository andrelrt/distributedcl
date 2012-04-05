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
#include "server_platform.h"
#include "message/message.h"
#include "message/msg_device.h"
#include "composite/opencl_composite.h"
#include "composite/composite_platform.h"
#include "composite/composite_device.h"
using dcl::network::message::dcl_message;
using dcl::network::message::msgGetDeviceIDs;
using dcl::composite::opencl_composite;
using dcl::composite::composite_platform;
using dcl::composite::composite_device;
using dcl::remote::remote_device;
using dcl::remote_id_t;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
server_platform server_platform::instance_;
//-----------------------------------------------------------------------------
void msgGetDeviceIDs_command::execute()
{
    const composite_platform& platform = opencl_composite::get_instance().get_platform();
    const devices_t& devs = platform.get_devices();

    for( devices_t::const_iterator it = devs.begin(); it != devs.end(); it++ )
    {
        composite_device* device_ptr = reinterpret_cast< composite_device* >( *it );
        remote_id_t id = server_platform::get_instance().get_device_manager().add( device_ptr );

        switch( device_ptr->get_type() )
        {
            case CL_DEVICE_TYPE_CPU:
                message_.add_cpu_device( id );
                break;

            case CL_DEVICE_TYPE_GPU:
                message_.add_gpu_device( id );
                break;

            case CL_DEVICE_TYPE_ACCELERATOR:
                message_.add_accelerator_device( id );
                break;

            default:
                message_.add_other_device( id );
                break;
        }
    }

    message_.update_response_size();
}
//-----------------------------------------------------------------------------
void msgGetDeviceInfo_command::execute()
{
    remote_id_t remote_id = message_.get_remote_id();

    const composite_device* device_ptr = server_platform::get_instance().get_device_manager().get( remote_id );

    message_.set_info( device_ptr->get_info() );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
