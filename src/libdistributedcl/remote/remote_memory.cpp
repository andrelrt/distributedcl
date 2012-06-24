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
#include "remote_memory.h"
#include "remote_command_queue.h"
#include "remote_event.h"
#include "message/msg_memory.h"
using dcl::info::generic_event;
using dcl::info::generic_image;
using dcl::info::generic_memory_object;
using dcl::info::generic_command_queue;
using dcl::network::message::dcl_message;
using dcl::network::message::base_message;
using dcl::network::message::msgEnqueueWriteBuffer;
using dcl::network::message::msgEnqueueReadBuffer;
using dcl::network::message::msgReleaseMemObject;
using dcl::network::message::msgEnqueueCopyBuffer;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
remote_memory::remote_memory( const remote_context& context_ref, const void* host_ptr, 
                              size_t size, cl_mem_flags flags ) :
        remote_object< remote_memory, msgReleaseMemObject >( context_ref.get_session() ),
        context_( context_ref )
{
    set_info( host_ptr, size, flags );
}
//-----------------------------------------------------------------------------
void remote_memory::write( generic_command_queue* queue_ptr, const void* data_ptr, 
                           size_t size, size_t offset, cl_bool blocking,
                           events_t& wait_events, generic_event** ret_event_ptr )
{
    dcl_message< msgEnqueueWriteBuffer >* msg_ptr = new dcl_message< msgEnqueueWriteBuffer >();

    msg_ptr->set_remote_id( get_remote_id() );
    msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_remote_id() );
    msg_ptr->set_buffer( reinterpret_cast<const uint8_t*>( data_ptr ), size, offset );
    msg_ptr->set_blocking( (blocking == CL_TRUE) );

    for( events_t::iterator it = wait_events.begin(); it != wait_events.end(); it++ )
    {
        msg_ptr->add_event( reinterpret_cast<remote_event*>( *it )->get_remote_id() );
    }

    message_sp_t message_sp( msg_ptr );

    if( ret_event_ptr != NULL )
    {
        remote_event* ptr = new remote_event( context_, queue_ptr, message_sp );
        *ret_event_ptr = reinterpret_cast<generic_event*>( ptr );

        ptr->set_remote_id( msg_ptr->get_event_id( *ret_event_ptr ) );
    }

    //if( blocking == CL_TRUE )
    //{
    //    reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_queue_session().send_message( message_sp );
    //}
    //else
    {
        reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_queue_session().enqueue_message( message_sp );
    }
}
//-----------------------------------------------------------------------------
void remote_memory::read( generic_command_queue* queue_ptr, void* data_ptr, 
                          size_t size, size_t offset, cl_bool blocking,
                          events_t& wait_events, generic_event** ret_event_ptr )
{
    dcl_message< msgEnqueueReadBuffer >* msg_ptr = new dcl_message< msgEnqueueReadBuffer >();

    msg_ptr->set_remote_id( get_remote_id() );
    msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_remote_id() );
    msg_ptr->set_buffer_size( size );
    msg_ptr->set_offset( offset );
    msg_ptr->set_blocking( (blocking == CL_TRUE) );

    msg_ptr->set_data_pointer( data_ptr );

    for( events_t::iterator it = wait_events.begin(); it != wait_events.end(); it++ )
    {
        msg_ptr->add_event( reinterpret_cast<remote_event*>( *it )->get_remote_id() );
    }

    message_sp_t message_sp( msg_ptr );

    if( ret_event_ptr != NULL )
    {
        remote_event* ptr = new remote_event( context_, queue_ptr, message_sp );
        *ret_event_ptr = reinterpret_cast<generic_event*>( ptr );

        ptr->set_remote_id( msg_ptr->get_event_id( *ret_event_ptr ) );
    }


    if( blocking == CL_TRUE )
    {
        reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_queue_session().send_message( message_sp );
    }
    else
    {
        reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_queue_session().enqueue_message( message_sp );
    }
}
//-----------------------------------------------------------------------------
void* remote_memory::map( generic_command_queue* queue_ptr, cl_map_flags flags,
                          size_t size, size_t offset, cl_bool blocking,
                          events_t& wait_events, generic_event** ret_event_ptr )
{
    std::cerr << "map size: " << size;

    uint8_t* ret_ptr = new uint8_t[ size ];

    if( flags & CL_MAP_READ )
    {
        std::cerr << " - read - ";
        read( queue_ptr, ret_ptr, size, offset, blocking, wait_events, ret_event_ptr );
    }

    std::cerr << std::endl;

    map_data data( flags, size, offset );
    map_pointers_.insert( map_pointer_flags_t::value_type( ret_ptr, data ) );

    return ret_ptr;
}
//-----------------------------------------------------------------------------
void remote_memory::unmap( generic_command_queue* queue_ptr, void* data_ptr,
                           events_t& wait_events, generic_event** ret_event_ptr )
{
    std::cerr << "unmap ptr: " << data_ptr
              << std::endl;

    map_pointer_flags_t::iterator it =
        map_pointers_.find( reinterpret_cast<uint8_t*>( data_ptr ) );

    if( it == map_pointers_.end() )
        throw dcl::library_exception( CL_INVALID_VALUE );

    if( it->second.flags_ & CL_MAP_WRITE )
    {
        // TODO: change the blocking mode to false and delete the pointer when the message was sent
        write( queue_ptr, data_ptr, it->second.size_, it->second.offset_, true, wait_events, ret_event_ptr );
    }

    delete[] it->first;

    map_pointers_.erase( it );
}
//-----------------------------------------------------------------------------
void remote_memory::copy( generic_command_queue* queue_ptr, generic_memory* src_ptr,
                          size_t size, size_t src_offset, size_t dst_offset,
                          events_t& wait_events, dcl::info::generic_event** ret_event_ptr )
{
    dcl_message< msgEnqueueCopyBuffer >* msg_ptr = new dcl_message< msgEnqueueCopyBuffer >();

    msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_remote_id() );
    msg_ptr->set_src_remote_id( reinterpret_cast<remote_memory*>( src_ptr )->get_remote_id() );
    msg_ptr->set_dst_remote_id( get_remote_id() );
    msg_ptr->set_buffer_size( size );
    msg_ptr->set_src_offset( src_offset );
    msg_ptr->set_dst_offset( src_offset );

    for( events_t::iterator it = wait_events.begin(); it != wait_events.end(); it++ )
    {
        msg_ptr->add_event( reinterpret_cast<remote_event*>( *it )->get_remote_id() );
    }

    message_sp_t message_sp( msg_ptr );

    if( ret_event_ptr != NULL )
    {
        remote_event* ptr = new remote_event( context_, queue_ptr, message_sp );
        *ret_event_ptr = reinterpret_cast<generic_event*>( ptr );

        ptr->set_remote_id( msg_ptr->get_event_id( *ret_event_ptr ) );
    }

    reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_queue_session().enqueue_message( message_sp );
}
//-----------------------------------------------------------------------------
// remote_image
//-----------------------------------------------------------------------------
remote_image::remote_image( const remote_context& context_ref, const void* host_ptr,
                            cl_mem_flags flags, const cl_image_format* format,
                            size_t width, size_t height, size_t row_pitch ) :
        remote_object< remote_image, msgReleaseMemObject >( context_ref.get_session() ),
        context_( context_ref )
{
    set_info( host_ptr, flags, format, width, height, row_pitch );
}
//-----------------------------------------------------------------------------
void remote_image::write( generic_command_queue* queue_ptr, const void* data_ptr,
                          const size_t origin[3], const size_t region[3],
                          size_t row_pitch, size_t slice_pitch, bool blocking,
                          events_t& wait_events, generic_event** ret_event_ptr )
{
    //dcl_message< msgEnqueueWriteImage >* msg_ptr = new dcl_message< msgEnqueueWriteImage >();

    //msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_remote_id() );
    //msg_ptr->set_remote_id( get_remote_id() );
    //msg_ptr->set_buffer( data_ptr );
    //msg_ptr->set_origin( origin );
    //msg_ptr->set_region( region );
    //msg_ptr->set_row_pitch( row_pitch );
    //msg_ptr->set_slice_pitch( slice_pitch );
    //msg_ptr->set_blocking( blocking );

    //for( events_t::iterator it = wait_events.begin(); it != wait_events.end(); it++ )
    //{
    //    msg_ptr->add_event( reinterpret_cast<remote_event*>( *it )->get_remote_id() );
    //}

    //message_sp_t message_sp( msg_ptr );

    //if( ret_event_ptr != NULL )
    //{
    //    remote_event* ptr = new remote_event( context_, queue_ptr, message_sp );
    //    *ret_event_ptr = reinterpret_cast<generic_event*>( ptr );

    //    ptr->set_remote_id( msg_ptr->get_event_id( *ret_event_ptr ) );
    //}

    //reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_queue_session().enqueue_message( message_sp );
}
//-----------------------------------------------------------------------------
void remote_image::unmap( generic_command_queue* queue_ptr, void* data_ptr,
                          events_t& wait_events, generic_event** ret_event_ptr )
{
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
