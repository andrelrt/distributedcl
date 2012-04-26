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
#include <string.h>
#include "msg_memory.h"
using dcl::remote_id_t;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
// msgCreateBuffer
//-----------------------------------------------------------------------------
void dcl_message< msgCreateBuffer >::create_request( void* payload_ptr )
{
    msgCreateBuffer_request* request_ptr = 
        reinterpret_cast< msgCreateBuffer_request* >( payload_ptr );

    request_ptr->context_id_ = host_to_network( context_id_ );
    request_ptr->flags_ = host_to_network( static_cast<uint16_t>( flags_ ) );
    request_ptr->buffer_len_ = host_to_network( static_cast<uint32_t>( buffer_len_ ) );
    request_ptr->message_buffer_ = host_to_network( static_cast<uint16_t>( 0 ) );

    if( buffer_ptr_ != NULL )
    {
        request_ptr->message_buffer_ = host_to_network( static_cast<uint16_t>( 1 ) );
        memcpy( request_ptr->buffer_, buffer_ptr_, buffer_len_ );
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateBuffer >::parse_request( const void* payload_ptr )
{
    const msgCreateBuffer_request* request_ptr = 
        reinterpret_cast< const msgCreateBuffer_request* >( payload_ptr );

    buffer_ptr_ = NULL;
    context_id_ = network_to_host( request_ptr->context_id_ );
    buffer_len_ = network_to_host( request_ptr->buffer_len_ );
    flags_ = static_cast<cl_mem_flags>( network_to_host( request_ptr->flags_ ) );

    if( network_to_host( request_ptr->message_buffer_ ) != 0 )
    {
        const uint8_t* begin = reinterpret_cast<const uint8_t*>( request_ptr->buffer_ );

        buffer_.assign( begin, begin + buffer_len_ );
        buffer_ptr_ = buffer_.data();
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateBuffer >::create_response( void* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateBuffer >::parse_response( const void* payload_ptr )
{
    const remote_id_t* response_ptr = 
        reinterpret_cast< const remote_id_t* >( payload_ptr );

    id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
// msgEnqueueWriteBuffer
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueWriteBuffer >::create_request( void* payload_ptr )
{
    msgEnqueueWriteBuffer_request* request_ptr = 
        reinterpret_cast< msgEnqueueWriteBuffer_request* >( payload_ptr );

    request_ptr->id_ = host_to_network( id_ );
    request_ptr->command_queue_id_ = host_to_network( command_queue_id_ );
    request_ptr->buffer_len_ = host_to_network( static_cast<uint32_t>( buffer_len_ ) );
    request_ptr->blocking_ = host_to_network( static_cast<uint32_t>( blocking_? 1 : 0 ) );
    request_ptr->return_event_ = host_to_network( static_cast<uint16_t>( return_event_ ? 1 : 0 ) );
    request_ptr->event_count_ = host_to_network( static_cast<uint16_t>( events_.size() ) );

    dcl::remote_id_t* events_ptr = reinterpret_cast<dcl::remote_id_t*>( request_ptr->buffer_ );

    for( uint32_t i = 0; i < events_.size(); i++ )
    {
        events_ptr[ i ] = host_to_network( events_[ i ] );
    }

    uint8_t* buffer_ptr = reinterpret_cast<uint8_t*>( events_ptr + events_.size() );

    memcpy( buffer_ptr, buffer_ptr_, buffer_len_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueWriteBuffer >::parse_request( const void* payload_ptr )
{
    const msgEnqueueWriteBuffer_request* request_ptr = 
        reinterpret_cast< const msgEnqueueWriteBuffer_request* >( payload_ptr );

    id_ = network_to_host( request_ptr->id_ );
    command_queue_id_ = network_to_host( request_ptr->command_queue_id_ );
    buffer_len_ = network_to_host( request_ptr->buffer_len_ );
    blocking_ = (network_to_host( request_ptr->blocking_ ) == 1) ? true : false;
    return_event_ = (network_to_host( request_ptr->return_event_ ) == 1) ? true : false;

    events_.clear();
    uint32_t event_count = network_to_host( request_ptr->event_count_ );

    const dcl::remote_id_t* events_ptr =
        reinterpret_cast<const dcl::remote_id_t*>( request_ptr->buffer_ );

    if( event_count != 0 )
    {
        events_.reserve( event_count );

        for( uint32_t i = 0; i < event_count; i++ )
        {
            events_.push_back( network_to_host( events_ptr[ i ] ) );
        }
    }

    const uint8_t* begin =
        reinterpret_cast<const uint8_t*>( events_ptr + event_count );

    buffer_.assign( begin, begin + buffer_len_ );

    buffer_ptr_ = buffer_.data();
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueWriteBuffer >::create_response( void* payload_ptr )
{
    dcl::remote_id_t* response_ptr =
        reinterpret_cast<dcl::remote_id_t*>( payload_ptr );

    *response_ptr = host_to_network( event_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueWriteBuffer >::parse_response( const void* payload_ptr )
{
    const dcl::remote_id_t* response_ptr =
        reinterpret_cast<const dcl::remote_id_t*>( payload_ptr );

    event_id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
// msgEnqueueReadBuffer
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueReadBuffer >::create_request( void* payload_ptr )
{
    msgEnqueueReadBuffer_request* request_ptr = 
        reinterpret_cast< msgEnqueueReadBuffer_request* >( payload_ptr );

    request_ptr->id_ = host_to_network( id_ );
    request_ptr->command_queue_id_ = host_to_network( command_queue_id_ );
    request_ptr->size_ = host_to_network( static_cast<uint32_t>( size_ ) );
    request_ptr->offset_ = host_to_network( static_cast<uint32_t>( offset_ ) );
    request_ptr->blocking_ = host_to_network( static_cast<uint32_t>( blocking_? 1 : 0 ) );

    request_ptr->return_event_ = host_to_network( static_cast<uint16_t>( return_event_ ? 1 : 0 ) );
    request_ptr->event_count_ = host_to_network( static_cast<uint16_t>( events_.size() ) );

    for( uint32_t i = 0; i < events_.size(); i++ )
    {
        request_ptr->events_[ i ] = host_to_network( events_[ i ] );
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueReadBuffer >::parse_request( const void* payload_ptr )
{
    const msgEnqueueReadBuffer_request* request_ptr = 
        reinterpret_cast< const msgEnqueueReadBuffer_request* >( payload_ptr );

    id_ = network_to_host( request_ptr->id_ );
    command_queue_id_ = network_to_host( request_ptr->command_queue_id_ );
    size_ = network_to_host( request_ptr->size_ );
    offset_ = network_to_host( request_ptr->offset_ );
    blocking_ = (network_to_host( request_ptr->blocking_ ) == 1) ? true : false;
    return_event_ = (network_to_host( request_ptr->return_event_ ) == 1) ? true : false;

    set_response_size( size_ + sizeof(msgEnqueueReadBuffer_response) - 1 );

    events_.clear();
    uint32_t event_count = network_to_host( request_ptr->event_count_ );

    if( event_count != 0 )
    {
        events_.reserve( event_count );

        for( uint32_t i = 0; i < event_count; i++ )
        {
            events_.push_back( network_to_host( request_ptr->events_[ i ] ) );
        }
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueReadBuffer >::create_response( void* payload_ptr )
{
    msgEnqueueReadBuffer_response* response_ptr =
        reinterpret_cast<msgEnqueueReadBuffer_response*>( payload_ptr );

    response_ptr->size_ = host_to_network( static_cast<uint32_t>( size_ ) );
    response_ptr->event_id_ = host_to_network( event_id_ );

    memcpy( response_ptr->buffer_, buffer_.data(), size_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueReadBuffer >::parse_response( const void* payload_ptr )
{
    const msgEnqueueReadBuffer_response* response_ptr =
        reinterpret_cast<const msgEnqueueReadBuffer_response*>( payload_ptr );

    size_ = network_to_host( response_ptr->size_ );
    event_id_ = network_to_host( response_ptr->event_id_ );

    buffer_.assign( response_ptr->buffer_, response_ptr->buffer_ + size_ );
}
//-----------------------------------------------------------------------------
// msgCreateImage2D
//-----------------------------------------------------------------------------
void dcl_message< msgCreateImage2D >::create_request( void* payload_ptr )
{
    msgCreateImage2D_request* request_ptr = 
        reinterpret_cast< msgCreateImage2D_request* >( payload_ptr );

    request_ptr->context_id_ = host_to_network( context_id_ );
    request_ptr->flags_ = static_cast<uint8_t>( flags_ );
    request_ptr->buffer_len_ = host_to_network( static_cast<uint32_t>( buffer_len_ ) );

    if( buffer_ptr_ != NULL )
    {
        request_ptr->message_buffer_ = host_to_network( static_cast<uint16_t>( 1 ) );
        memcpy( request_ptr->buffer_, buffer_ptr_, buffer_len_ );
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateImage2D >::parse_request( const void* payload_ptr )
{
    const msgCreateImage2D_request* request_ptr = 
        reinterpret_cast< const msgCreateImage2D_request* >( payload_ptr );

    buffer_ptr_ = NULL;
    context_id_ = network_to_host( request_ptr->context_id_ );
    buffer_len_ = network_to_host( request_ptr->buffer_len_ );
    flags_ = static_cast<cl_mem_flags>( request_ptr->flags_ );

    if( network_to_host( request_ptr->message_buffer_ ) != 0 )
    {
        const uint8_t* begin = reinterpret_cast<const uint8_t*>( request_ptr->buffer_ );

        buffer_.assign( begin, begin + buffer_len_ );
        buffer_ptr_ = buffer_.data();
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateImage2D >::create_response( void* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateImage2D >::parse_response( const void* payload_ptr )
{
    const remote_id_t* response_ptr = 
        reinterpret_cast< const remote_id_t* >( payload_ptr );

    id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
