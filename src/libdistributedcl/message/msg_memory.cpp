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
    void* enqueue_ptr = enqueue_message::create_enqueue_request( payload_ptr );

    msgEnqueueWriteBuffer_request* request_ptr = 
        reinterpret_cast< msgEnqueueWriteBuffer_request* >( enqueue_ptr );

    request_ptr->id_ = host_to_network( id_ );
    request_ptr->command_queue_id_ = host_to_network( command_queue_id_ );
    request_ptr->buffer_len_ = host_to_network( static_cast<uint32_t>( buffer_len_ ) );

    memcpy( request_ptr->buffer_, buffer_ptr_, buffer_len_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueWriteBuffer >::parse_request( const void* payload_ptr )
{
    const void* enqueue_ptr = enqueue_message::parse_enqueue_request( payload_ptr );

    const msgEnqueueWriteBuffer_request* request_ptr = 
        reinterpret_cast< const msgEnqueueWriteBuffer_request* >( enqueue_ptr );

    id_ = network_to_host( request_ptr->id_ );
    command_queue_id_ = network_to_host( request_ptr->command_queue_id_ );
    buffer_len_ = network_to_host( request_ptr->buffer_len_ );

    buffer_.assign( request_ptr->buffer_, request_ptr->buffer_ + buffer_len_ );

    buffer_ptr_ = buffer_.data();
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueWriteBuffer >::create_response( void* payload_ptr )
{
    enqueue_message::create_enqueue_response( payload_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueWriteBuffer >::parse_response( const void* payload_ptr )
{
    enqueue_message::parse_enqueue_response( payload_ptr );
}
//-----------------------------------------------------------------------------
// msgEnqueueReadBuffer
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueReadBuffer >::create_request( void* payload_ptr )
{
    void* enqueue_ptr = enqueue_message::create_enqueue_request( payload_ptr );

    msgEnqueueReadBuffer_request* request_ptr = 
        reinterpret_cast< msgEnqueueReadBuffer_request* >( enqueue_ptr );

    request_ptr->id_ = host_to_network( id_ );
    request_ptr->command_queue_id_ = host_to_network( command_queue_id_ );
    request_ptr->size_ = host_to_network( static_cast<uint32_t>( size_ ) );
    request_ptr->offset_ = host_to_network( static_cast<uint32_t>( offset_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueReadBuffer >::parse_request( const void* payload_ptr )
{
    const void* enqueue_ptr = enqueue_message::parse_enqueue_request( payload_ptr );

    const msgEnqueueReadBuffer_request* request_ptr = 
        reinterpret_cast< const msgEnqueueReadBuffer_request* >( enqueue_ptr );

    id_ = network_to_host( request_ptr->id_ );
    command_queue_id_ = network_to_host( request_ptr->command_queue_id_ );
    size_ = network_to_host( request_ptr->size_ );
    offset_ = network_to_host( request_ptr->offset_ );

    set_response_size( size_ +
                       get_enqueue_response_size() +
                       sizeof(msgEnqueueReadBuffer_response) - 1 );
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueReadBuffer >::create_response( void* payload_ptr )
{
    void* enqueue_ptr = enqueue_message::create_enqueue_response( payload_ptr );

    msgEnqueueReadBuffer_response* response_ptr =
        reinterpret_cast<msgEnqueueReadBuffer_response*>( enqueue_ptr );

    response_ptr->size_ = host_to_network( static_cast<uint32_t>( size_ ) );

    memcpy( response_ptr->buffer_, buffer_.data(), size_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueReadBuffer >::parse_response( const void* payload_ptr )
{
    const void* enqueue_ptr = enqueue_message::parse_enqueue_response( payload_ptr );

    const msgEnqueueReadBuffer_response* response_ptr =
        reinterpret_cast<const msgEnqueueReadBuffer_response*>( enqueue_ptr );

    size_ = network_to_host( response_ptr->size_ );

    if( data_ptr_ != NULL )
        memcpy( data_ptr_, response_ptr->buffer_, size_ );
    else
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
    request_ptr->channel_order = channel_order_to_network();
    request_ptr->channel_type = channel_type_to_network();
    request_ptr->width_ = host_to_network( static_cast<uint32_t>( width_ ) );
    request_ptr->height_ = host_to_network( static_cast<uint32_t>( height_ ) );
    request_ptr->row_pitch_ = host_to_network( static_cast<uint32_t>( row_pitch_ ) );

    request_ptr->buffer_len_ = host_to_network( static_cast<uint32_t>( buffer_len_ ) );
    request_ptr->message_buffer_ = 0;

    if( buffer_ptr_ != NULL )
    {
        request_ptr->message_buffer_ = 1;
        memcpy( request_ptr->buffer_, buffer_ptr_, buffer_len_ );
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateImage2D >::parse_request( const void* payload_ptr )
{
    const msgCreateImage2D_request* request_ptr = 
        reinterpret_cast< const msgCreateImage2D_request* >( payload_ptr );

    context_id_ = network_to_host( request_ptr->context_id_ );
    flags_ = static_cast<cl_mem_flags>( request_ptr->flags_ );
    network_to_channel_order( request_ptr->channel_order );
    network_to_channel_type( request_ptr->channel_type );
    width_ = network_to_host( request_ptr->width_ );
    height_ = network_to_host( request_ptr->height_ );
    row_pitch_ = network_to_host( request_ptr->row_pitch_ );

    buffer_len_ = network_to_host( request_ptr->buffer_len_ );
    buffer_ptr_ = NULL;

    if( request_ptr->message_buffer_ != 0 )
    {
        const uint8_t* begin = reinterpret_cast<const uint8_t*>( request_ptr->buffer_ );

        std::cout << "request_ptr->message_buffer_ != 0, buffer_len_ = " << buffer_len_
                     << " width = " << width_
                     << " height = " << height_
                     << " row_pitch = " << row_pitch_
                     << " request_ptr->buffer_ = " << (void*)begin
                     << std::endl;
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
