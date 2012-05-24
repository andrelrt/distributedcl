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
#ifndef _DCL_TCP_TRANSPORT_H_
#define _DCL_TCP_TRANSPORT_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#if !defined( WIN32 )
#include <netinet/tcp.h>
#endif
#include <string.h>
#include <boost/thread.hpp>
#include <boost/tokenizer.hpp>
#include <boost/scoped_array.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "distributedcl_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace platform {
//-----------------------------------------------------------------------------
template< class OBSERVER >
class tcp_transport
{
public:
    typedef OBSERVER* observer_ptr_t;
    typedef sockaddr endpoint_t;

#if defined( WIN32 )
    typedef SOCKET connection_t;
#else
    typedef int connection_t;
#endif

    typedef struct tcp_info
    {
        endpoint_t bind_addr;
        connection_t connected_socket;

        tcp_info( connection_t& connection, endpoint_t& endpoint ) :
            bind_addr( endpoint ), connected_socket( connection )
        {}

        tcp_info()
        {
            connected_socket = -1;

            memset( &bind_addr, 0, sizeof( sockaddr ) );
            sockaddr_in* sin = reinterpret_cast< sockaddr_in* >( &bind_addr );

            sin->sin_family = AF_INET;
            sin->sin_port = htons( 4791 );
        }

        tcp_info( const std::string& connection_string )
        {
            connected_socket = -1;

            memset( &bind_addr, 0, sizeof( sockaddr ) );
            sockaddr_in* sin = reinterpret_cast< sockaddr_in* >( &bind_addr );

            sin->sin_family = AF_INET;
            sin->sin_port = htons( 4791 );

            typedef boost::tokenizer<boost::char_separator<char> > tokenizer_t;

            tokenizer_t tokens( connection_string, boost::char_separator<char>( ":" ) );

            tokenizer_t::iterator it = tokens.begin();
            if( it != tokens.end() )
            {
                sin->sin_addr.s_addr = inet_addr( it->c_str() );
                it++;
            }

            if( it != tokens.end() )
            {
                sin->sin_port = htons( atoi( it->c_str() ) );
            }
        }

    } config_info_t;

    inline tcp_transport( const config_info_t& config ) :
		client_( config.bind_addr ), socket_( config.connected_socket ),
        config_( config ), observer_( NULL ),
        accept_thread_ptr_( NULL ), buffer_size_( default_buffer_size ),
        base_message_buffer_ptr_( NULL )
    {}

    inline ~tcp_transport()
    {
        //FIXME: Delete buffers
        //delete accept_thread_ptr_;
        //delete base_message_buffer_ptr_;
    }

    const config_info_t get_config() const
    {
        return config_;
    }

    inline void startup( observer_ptr_t observer )
    {
        observer_ = observer;

        // Allocate the internal buffer
        get_message_buffer();
    }

    inline void shutdown()
    {
        if( accept_thread_ptr_ != NULL )
        {
            accept_thread_ptr_->interrupt();

            close();

            accept_thread_ptr_->join();
        }
    }

    inline void start_accept_thread()
    {
        if( accept_thread_ptr_ == NULL )
        {
            accept_thread_ptr_ = new boost::thread( &dcl::network::platform::tcp_transport< OBSERVER >::accept_loop, this );

            boost::this_thread::sleep( boost::posix_time::milliseconds( static_cast<int64_t>( 10 ) ) );
        }
    }

    inline void connect()
    {
        create_socket();

        int error_code = ::connect( socket_, &(config_.bind_addr), sizeof( sockaddr_in ) );
        if( error_code == -1 )
        {
            throw dcl::library_exception( "Error connecting server", error_code );
        }
    }

    inline void close()
    {
#if defined( WIN32 )
        ::closesocket( socket_ );
#else
        ::close( socket_ );
#endif
        socket_ = -1;
    }

    inline void send_message( std::size_t size, bool blocking )
    {
        if( (size == 0) ||
            (size > buffer_size_) )
        {
            throw dcl::library_exception( "Invalid buffer size" );
        }

        int flags = 0;

#if !defined( WIN32 )
        flags = blocking ? 0 : MSG_DONTWAIT;
#endif
        int32_t ret_size = ::send( socket_,
                                   reinterpret_cast< const char * >( base_message_buffer_ptr_ ),
                                   static_cast< int >( size ), flags );
#if defined( WIN32 )
        if( ret_size != size )
#else
        if( ((blocking) && (ret_size != static_cast<int32_t>(size))) ||
            ((!blocking) && (ret_size == -1) && (errno != EAGAIN) && (errno != EWOULDBLOCK)) )
#endif
        {
            throw dcl::library_exception( "Error sending data" );
        }
    }


    inline uint8_t* receive_message( std::size_t* size_ptr )
    {
        scoped_lock_t lock( mutex_ );

        receive_size( base_message_buffer_ptr_, dcl::network::message::packet::get_header_size() );

        std::size_t total_size = dcl::network::message::packet::get_packet_size( base_message_buffer_ptr_ );

        if( total_size > buffer_size_ )
        {
            boost::scoped_array<uint8_t> header( new uint8_t[ dcl::network::message::packet::get_header_size() ] );

            memcpy( header.get(), base_message_buffer_ptr_, dcl::network::message::packet::get_header_size() );

            get_message_buffer( &total_size );

            memcpy( base_message_buffer_ptr_, header.get(), dcl::network::message::packet::get_header_size() );
        }

        receive_size( base_message_buffer_ptr_ + dcl::network::message::packet::get_header_size(),
                      total_size - dcl::network::message::packet::get_header_size() );

        if( size_ptr != NULL )
        {
            *size_ptr = total_size;
        }

        return base_message_buffer_ptr_;
    }

    inline uint8_t* get_message_buffer( std::size_t* buffer_size = NULL )
    {
        if( (buffer_size != NULL) &&
            (*buffer_size > buffer_size_) &&
            (*buffer_size <= max_buffer_size) )
        {
            buffer_size_ = *buffer_size;

            if( base_message_buffer_ptr_ != NULL )
            {
                delete base_message_buffer_ptr_;
                base_message_buffer_ptr_ = NULL;
            }
        }

        if( base_message_buffer_ptr_ == NULL )
        {
            base_message_buffer_ptr_ = new uint8_t[ buffer_size_ ];
        }

        if( buffer_size != NULL )
        {
            *buffer_size = buffer_size_;
        }

        return( base_message_buffer_ptr_ );
    }

    inline void wait()
    {
        accept_thread_ptr_->join();
    }

private:
    endpoint_t client_;
    connection_t socket_;
    config_info_t config_;
    observer_ptr_t observer_;

    boost::thread* accept_thread_ptr_;

    std::size_t buffer_size_;
    uint8_t* base_message_buffer_ptr_;

    boost::interprocess::interprocess_mutex mutex_;

    enum
    {
        default_buffer_size = 0x00400000, //4MiB
        max_buffer_size = UINT32_MAX
    };

    inline void set_socket_options( connection_t sock )
    {
        int flag_true = 1;
        setsockopt( sock, SOL_SOCKET, SO_REUSEADDR,
                    reinterpret_cast< const char* >( &flag_true ),
                    sizeof( int ) );

        //setsockopt( sock, SOL_SOCKET, SO_KEEPALIVE,
        //            reinterpret_cast< const char* >( &flag_true ),
        //            sizeof( int ) );

        //setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, 
        //            reinterpret_cast< const char* >( &flag_true ),
        //            sizeof( int ) );
    }

    inline void create_socket()
    {
        socket_ = socket( AF_INET, SOCK_STREAM, 0 );

        set_socket_options( socket_ );
    }

    inline void receive_size( uint8_t* buffer_ptr, std::size_t size )
    {
        std::size_t recv_size = 0;

        while( recv_size != size )
        {
            std::size_t diff_size = size - recv_size;

            int chunk_size = ::recv( socket_,
                                     reinterpret_cast< char* >( buffer_ptr + recv_size ),
                                     static_cast< int >( diff_size ), 0 );
            if( chunk_size == -1 )
            {
                throw dcl::library_exception( "Recv Error" );
            }

            if( chunk_size == 0 )
            {
                throw dcl::library_exception( "Orderly shutdown" );
            }

            recv_size += chunk_size;
        }
    }

    void accept_loop()
    {
        create_socket();

        int error_code = bind( socket_, &config_.bind_addr, sizeof( sockaddr ) );
        if( error_code == -1 )
        {
            throw dcl::library_exception( "Error binding socket", error_code );
        }

        error_code = listen( socket_, 10 );
        if( error_code == -1 )
        {
            throw dcl::library_exception( "Error starting listener", error_code );
        }

#if defined( WIN32 )
        int addr_len;
#else
        socklen_t addr_len;
#endif
        sockaddr client_addr;
        connection_t client_socket;

        while( !boost::this_thread::interruption_requested() )
        {
            addr_len = sizeof( sockaddr_in );

            client_socket = accept( socket_, &client_addr, &addr_len );

            if( client_socket == -1 )
            {
                break;
            }

            set_socket_options( client_socket );

            observer_->on_accept( client_socket, client_addr );
        }
    }
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::platform;
//-----------------------------------------------------------------------------
#endif // _DCL_TCP_TRANSPORT_H_
