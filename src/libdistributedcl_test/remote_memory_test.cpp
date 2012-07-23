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
#include <gtest/gtest.h>
#include "info/device_info.h"
#include "remote_memory_test.h"
#include "remote/remote_opencl.h"
#include "network/session_manager.h"
using dcl::platforms_t;
using dcl::devices_t;
using dcl::remote::remote_opencl;
using dcl::network::client::session_manager;
using dcl::info::generic_context;
using dcl::info::generic_memory;
using dcl::info::generic_command_queue;

#define CONNECTION_STRING  "127.0.0.1:4791"
//-----------------------------------------------------------------------------
void remote_memory_test::init()
{
    session_manager::session_t& session_ref = session_manager::create_session( CONNECTION_STRING );

    remote_ptr_ = new remote_opencl( session_ref );
}
//-----------------------------------------------------------------------------
void remote_memory_test::finish()
{
    delete remote_ptr_;
}
//-----------------------------------------------------------------------------
void remote_memory_test::create_context_and_queue( generic_context** context_pptr,
                                                   generic_command_queue** queue_pptr )
{
    const platforms_t& platforms = remote_ptr_->get_platforms();
    const devices_t& devices = platforms[ 0 ]->get_devices();

    *context_pptr = platforms[ 0 ]->create_context( devices );
    *queue_pptr = (*context_pptr)->create_command_queue( devices[ 1 ], 0 );
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


