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
#include "remote_context.h"
#include "remote_program.h"
#include "remote_device.h"
#include "message/msg_program.h"
#include "message/msg_context.h"
using dcl::info::generic_program;
using dcl::network::message::base_message;
using dcl::network::message::dcl_message;
using dcl::network::message::msgGetContextInfo;
using dcl::network::message::msgCreateProgramWithSource;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_context::load_devices()
{
    dcl_message< msgGetContextInfo > msg;

    msg.set_remote_id( get_remote_id() );

    session_ref_.send_message( reinterpret_cast< base_message* >( &msg ) );

    for( uint32_t i = 0; i < msg.get_device_count(); i++ )
    {
        devices_.push_back( new remote_device( platform_ptr_, msg.get_devices()[ i ] ) );
    }
}
//-----------------------------------------------------------------------------
generic_program* remote_context::do_create_program( const std::string& source_code )
{
    dcl_message< msgCreateProgramWithSource > msg;

    msg.set_source_code( source_code );
    msg.set_context_id( get_remote_id() );

    session_ref_.send_message( reinterpret_cast< base_message* >( &msg ) );

    remote_program* program_ptr = new remote_program( *this, source_code );
    program_ptr->set_remote_id( msg.get_remote_id() );

    return reinterpret_cast< generic_program* >( program_ptr );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
