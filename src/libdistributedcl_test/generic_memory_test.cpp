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
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include "info/memory_info.h"
#include "info/event_info.h"
#include "info/command_queue_info.h"

#include "generic_memory_test.h"
#include "opencl_memory_test.h"
#include "remote_memory_test.h"

using testing::Types;
using dcl::platforms_t;
using dcl::devices_t;
using dcl::events_t;
using dcl::single::opencl_single;
using dcl::single::opencl_library;
using dcl::info::generic_context;
using dcl::info::generic_memory;
using dcl::info::generic_command_queue;
using dcl::info::generic_event;
//-----------------------------------------------------------------------------
typedef Types< opencl_memory_test, remote_memory_test > Implementations;

TYPED_TEST_CASE( generic_memory_test, Implementations );
//-----------------------------------------------------------------------------
TYPED_TEST( generic_memory_test, CreateRead )
{
    boost::scoped_array<uint8_t> original_buffer( new uint8_t[ 512 ] );
    boost::scoped_array<uint8_t> copy_buffer( new uint8_t[ 512 ] );

    for( uint32_t i = 0; i < 512; ++i )
    {
        original_buffer[ i ] = i & 0xff;
    }

    boost::scoped_ptr<generic_memory>
        memory_sp( context_ptr_->create_buffer( original_buffer.get(), 512,
                                                CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR ) );

    memory_sp->read( queue_ptr_, copy_buffer.get(), 512, 0, true, events_t(), NULL );

    for( uint32_t i = 0; i < 512; ++i )
    {
        ASSERT_EQ( original_buffer[ i ], copy_buffer[ i ] );
    }
}
//-----------------------------------------------------------------------------
TYPED_TEST( generic_memory_test, CreateWriteRead )
{
    boost::scoped_array<uint8_t> original_buffer( new uint8_t[ 512 ] );
    boost::scoped_array<uint8_t> copy_buffer( new uint8_t[ 512 ] );

    for( uint32_t i = 0; i < 512; ++i )
    {
        original_buffer[ i ] = i & 0xff;
    }

    boost::scoped_ptr<generic_memory>
        memory_sp( context_ptr_->create_buffer( NULL, 512, CL_MEM_READ_WRITE ) );

    memory_sp->write( queue_ptr_, original_buffer.get(), 512, 0, true, events_t(), NULL );
    memory_sp->read( queue_ptr_, copy_buffer.get(), 512, 0, true, events_t(), NULL );

    for( uint32_t i = 0; i < 512; ++i )
    {
        ASSERT_EQ( original_buffer[ i ], copy_buffer[ i ] );
    }
}
//-----------------------------------------------------------------------------
TYPED_TEST( generic_memory_test, CreateCopyRead )
{
    boost::scoped_array<uint8_t> original_buffer( new uint8_t[ 512 ] );
    boost::scoped_array<uint8_t> copy_buffer( new uint8_t[ 512 ] );

    for( uint32_t i = 0; i < 512; ++i )
    {
        original_buffer[ i ] = i & 0xff;
    }

    boost::scoped_ptr<generic_memory>
        memory_sp( context_ptr_->create_buffer( original_buffer.get(), 512,
                                                CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR ) );
    boost::scoped_ptr<generic_memory>
        copy_memory_sp( context_ptr_->create_buffer( NULL, 512, CL_MEM_READ_WRITE ) );

    copy_memory_sp->copy( queue_ptr_, memory_sp.get(), 512, 0, 0, events_t(), NULL );
    copy_memory_sp->read( queue_ptr_, copy_buffer.get(), 512, 0, true, events_t(), NULL );

    for( uint32_t i = 0; i < 512; ++i )
    {
        ASSERT_EQ( original_buffer[ i ], copy_buffer[ i ] );
    }
}
//-----------------------------------------------------------------------------
TYPED_TEST( generic_memory_test, CreateAsyncWriteCopyRead )
{
    boost::scoped_array<uint8_t> original_buffer( new uint8_t[ 512 ] );
    boost::scoped_array<uint8_t> copy_buffer( new uint8_t[ 512 ] );

    for( uint32_t i = 0; i < 512; ++i )
    {
        original_buffer[ i ] = i & 0xff;
    }

    boost::scoped_ptr<generic_memory>
        memory_sp( context_ptr_->create_buffer( NULL, 512, CL_MEM_WRITE_ONLY ) );

    boost::scoped_ptr<generic_memory>
        copy_memory_sp( context_ptr_->create_buffer( NULL, 512, CL_MEM_READ_ONLY ) );


    generic_event* write_event_ptr = NULL;
    memory_sp->write( queue_ptr_, original_buffer.get(), 512, 0, false, events_t(), &write_event_ptr );

    events_t wait_write;
    wait_write.push_back( write_event_ptr );
    generic_event* copy_event_ptr = NULL;
    copy_memory_sp->copy( queue_ptr_, memory_sp.get(), 512, 0, 0, wait_write, &copy_event_ptr );

    events_t wait_copy;
    wait_copy.push_back( copy_event_ptr );
    generic_event* read_event_ptr = NULL;
    copy_memory_sp->read( queue_ptr_, copy_buffer.get(), 512, 0, false, wait_copy, &read_event_ptr );

    read_event_ptr->wait();

    for( uint32_t i = 0; i < 512; ++i )
    {
        ASSERT_EQ( original_buffer[ i ], copy_buffer[ i ] );
    }
}
//-----------------------------------------------------------------------------
TYPED_TEST( generic_memory_test, CreateAsyncWriteWriteCopyRead )
{
    boost::scoped_array<uint8_t> original_buffer( new uint8_t[ 512 ] );
    boost::scoped_array<uint8_t> second_buffer( new uint8_t[ 512 ] );
    boost::scoped_array<uint8_t> copy_buffer( new uint8_t[ 512 ] );

    for( uint32_t i = 0; i < 512; ++i )
    {
        original_buffer[ i ] = i & 0xff;
        second_buffer[ i ] = 255 - original_buffer[ i ];
    }

    boost::scoped_ptr<generic_memory>
        memory_sp( context_ptr_->create_buffer( NULL, 512, CL_MEM_WRITE_ONLY ) );

    generic_event* write_event_ptr = NULL;
    memory_sp->write( queue_ptr_, original_buffer.get(), 512, 0, false, events_t(), &write_event_ptr );

    queue_ptr_->flush();

    boost::scoped_ptr<generic_memory>
        second_memory_sp( context_ptr_->create_buffer( NULL, 512, CL_MEM_WRITE_ONLY ) );

    generic_event* second_event_ptr = NULL;
    second_memory_sp->write( queue_ptr_, second_buffer.get(), 512, 0, false, events_t(), &second_event_ptr );

    boost::scoped_ptr<generic_memory>
        copy_memory_sp( context_ptr_->create_buffer( NULL, 512, CL_MEM_READ_ONLY ) );

    events_t wait_write;
    wait_write.push_back( write_event_ptr );
    wait_write.push_back( second_event_ptr );
    generic_event* copy_event_ptr = NULL;
    copy_memory_sp->copy( queue_ptr_, memory_sp.get(), 512, 0, 0, wait_write, &copy_event_ptr );

    events_t wait_copy;
    wait_copy.push_back( copy_event_ptr );
    generic_event* read_event_ptr = NULL;
    copy_memory_sp->read( queue_ptr_, copy_buffer.get(), 512, 0, false, wait_copy, &read_event_ptr );

    read_event_ptr->wait();

    for( uint32_t i = 0; i < 512; ++i )
    {
        ASSERT_EQ( original_buffer[ i ], copy_buffer[ i ] );
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
