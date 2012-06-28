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
#define THROW_IF(b,ex) if(b) throw dcl::library_exception(ex)

uint32_t packet::parse_header()
{
    packet_header* header_ptr = reinterpret_cast< packet_header* >( buffer_ptr_ );

    uint32_t packet_len = network_to_host( header_ptr->length );

    THROW_IF( buffer_size_ < sizeof( packet_header ), "Invalid packet size" );
    THROW_IF( header_ptr->version != packet_v1_0, "Invalid packet version" );

    if( (header_ptr->message_count == 0) &&
        (packet_len != sizeof( packet_header )) )
    {
        std::cerr << "Message count invalid: " << header_ptr->message_count
                  << " packet_len = " << packet_len
                  << " sequence number = " << header_ptr->sequence_number
                  << std::endl;
    }

    THROW_IF( (header_ptr->message_count == 0) && 
              (packet_len != sizeof( packet_header )), "Message count invalid" );

    return packet_len;
}
//-----------------------------------------------------------------------------
void packet::parse( bool is_request )
{
    uint32_t packet_len = parse_header();

    uint32_t length = packet_len - sizeof( packet_header );
    uint8_t* it = buffer_ptr_ + sizeof( packet_header );

    messages_.clear();
    message_map_.clear();

    while( length != 0 )
    {
        message_sp_t message_sp( base_message::parse_message( it, length, is_request ) );

        add_message( message_sp );

        it += message_sp->get_size();
        length -= static_cast< uint32_t >( message_sp->get_size() );

        THROW_IF( length < 0, "Message parse error" );
    }
}

#undef THROW_IF
//-----------------------------------------------------------------------------
void packet::add( message_sp_t message_sp )
{
    length_ += message_sp->get_size();

    add_message( message_sp );
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

        uint32_t offset = sizeof( packet_header );

        message_vector_t::iterator it;
        for( it = messages_.begin(); it != messages_.end(); it++ )
        {
            (*it)->get_buffer( buffer_ptr_ + offset );
            offset += static_cast< uint32_t >( (*it)->get_size() );
        }

        header_ptr_->length = host_to_network( static_cast< uint32_t >( length_ ) );
        header_ptr_->message_count = static_cast<uint16_t>( messages_.size() );
    }

    //{
    //    std::stringstream file;

    //    file << "packet_" << header_ptr_->sequence_number << ".dump";

    //    FILE* fp = fopen( file.str().c_str(), "wb" );
    //    fwrite( buffer_ptr_, 1, length_, fp );
    //    fclose( fp );
    //}
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
