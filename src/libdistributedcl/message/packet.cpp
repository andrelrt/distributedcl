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
#include "packet.h"
#include "message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
packet::~packet()
{
    message_vector_t::iterator it;

    for( it = messages_.begin(); it != messages_.end(); it++ )
    {
        delete *it;
    }
}
//-----------------------------------------------------------------------------
#define THROW_IF(b,ex) if(b) throw dcl::library_exception(ex)

void packet::parse()
{
    packet_header* header_ptr = reinterpret_cast< packet_header* >( buffer_ptr_ );

    uint16_t packet_len = network_to_host( header_ptr->length );

    THROW_IF( buffer_size_ < sizeof( packet_header ), "Invalid packet size" );
    THROW_IF( header_ptr->version != packet_v1_0, "Invalid packet version" );

    THROW_IF( (header_ptr->message_count == 0) && 
              (packet_len != sizeof( packet_header )), "Message count invalid" );

    uint32_t length = packet_len - sizeof( packet_header );
    uint8_t* it = buffer_ptr_ + sizeof( packet_header );

    while( length != 0 )
    {
        base_message* message_ptr = base_message::parse_message( it, length );

        messages_.push_back( message_ptr );

        it += message_ptr->get_size();
        length -= static_cast< uint32_t >( message_ptr->get_size() );

        THROW_IF( length < 0, "Message parse error" );
    }
}
//-----------------------------------------------------------------------------
void packet::parse_messages()
{
    packet_header* header_ptr = reinterpret_cast< packet_header* >( buffer_ptr_ );
    uint16_t packet_len = network_to_host( header_ptr->length );

    uint32_t length = packet_len - sizeof( packet_header );
    uint8_t* it = buffer_ptr_ + sizeof( packet_header );

    messages_.clear();

    while( length != 0 )
    {
        base_message* message_ptr = base_message::parse_message( it, length );

        messages_.push_back( message_ptr );

        it += message_ptr->get_size();
        length -= static_cast< uint32_t >( message_ptr->get_size() );

        THROW_IF( length < 0, "Message parse error" );
    }
}

#undef THROW_IF
//-----------------------------------------------------------------------------
void packet::add( base_message* message_ptr )
{
    length_ += message_ptr->get_size();

    messages_.push_back( message_ptr );
}
//-----------------------------------------------------------------------------
void packet::create_packet()
{
    if( messages_.size() != 0 )
    {
        if( length_ > buffer_size_ )
        {
            throw dcl::library_exception( "buffer overflow" );
        }

        uint16_t offset = sizeof( packet_header );

        message_vector_t::iterator it;
        for( it = messages_.begin(); it != messages_.end(); it++ )
        {
            (*it)->get_buffer( buffer_ptr_ + offset );
            offset += static_cast< uint16_t >( (*it)->get_size() );
        }

        header_ptr_->length = static_cast< uint16_t >( host_to_network( static_cast< u_short >( length_ ) ) );
        header_ptr_->message_count = messages_.size();
    }
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
