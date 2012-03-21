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
#include "remote_program.h"
#include "remote_kernel.h"
#include "remote_device.h"
#include "message/msg_program.h"
#include "message/msg_kernel.h"
using dcl::info::generic_device;
using dcl::network::message::dcl_message;
using dcl::network::message::base_message;
using dcl::network::message::msgBuildProgram;
using dcl::network::message::msgCreateKernel;
using dcl::network::message::msgGetProgramBuildInfo;
using dcl::info::generic_kernel;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_program::build( const std::string& build_options, cl_bool blocking )
{
    build( devices_t(), build_options, blocking );
}
//-----------------------------------------------------------------------------
void remote_program::build( const devices_t& devices, const std::string& build_options, cl_bool blocking )
{
    dcl_message< msgBuildProgram >* msg_ptr = new dcl_message< msgBuildProgram >();

    msg_ptr->set_devices( devices );
    msg_ptr->set_program_id( get_remote_id() );
    msg_ptr->set_build_options( build_options );

    session_ref_.send_message( reinterpret_cast< base_message* >( msg_ptr ) );
}
//-----------------------------------------------------------------------------
generic_kernel* remote_program::create_kernel( const std::string& kernel_name )
{
    dcl_message< msgCreateKernel >* msg_ptr = new dcl_message< msgCreateKernel >();

    msg_ptr->set_name( kernel_name );
    msg_ptr->set_program_id( get_remote_id() );

    session_ref_.send_message( reinterpret_cast< base_message* >( msg_ptr ) );

    remote_kernel* kernel_ptr = new remote_kernel( context_, kernel_name );
    kernel_ptr->set_remote_id( msg_ptr->get_remote_id() );

    return reinterpret_cast< generic_kernel* >( kernel_ptr );
}
//-----------------------------------------------------------------------------
cl_build_status remote_program::get_build_status( const generic_device* device_ptr ) const
{
    dcl_message< msgGetProgramBuildInfo >* msg_ptr = new dcl_message< msgGetProgramBuildInfo >();

    msg_ptr->set_remote_id( get_remote_id() );
    msg_ptr->set_device_id( reinterpret_cast<const remote_device*>( device_ptr )->get_remote_id() );
    msg_ptr->set_build_info( CL_PROGRAM_BUILD_STATUS );

    session_ref_.send_message( reinterpret_cast< base_message* >( msg_ptr ) );

    return msg_ptr->get_build_status();
}
//-----------------------------------------------------------------------------
void remote_program::get_build_log( const generic_device* device_ptr, std::string& build_log ) const
{
    dcl_message< msgGetProgramBuildInfo >* msg_ptr = new dcl_message< msgGetProgramBuildInfo >();

    msg_ptr->set_remote_id( get_remote_id() );
    msg_ptr->set_device_id( reinterpret_cast<const remote_device*>( device_ptr )->get_remote_id() );
    msg_ptr->set_build_info( CL_PROGRAM_BUILD_LOG );

    session_ref_.send_message( reinterpret_cast< base_message* >( msg_ptr ) );

    build_log.assign( msg_ptr->get_build_log() );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
