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
#include <boost/scoped_ptr.hpp>
#include "distributedcl_internal.h"
#include "network/server.h"
#include "network/tcp_transport.h"
#include "composite/opencl_composite.h"
using dcl::network::server::server;
using dcl::network::platform::tcp_transport;
using dcl::composite::opencl_composite;

#if defined( WIN32 )
#define OPENCL_LIBRARY  "OpenCL.dll"
#else
#define OPENCL_LIBRARY  "libOpenCL.so"
#endif
//-----------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
#if defined( WIN32 )

    // Winsock startup
    WSADATA wsaData;
    WSAStartup( MAKEWORD(1, 1), &wsaData );

#endif

    // Load OpenCL Libraries --------------------------------------------------
    opencl_composite::get_instance().add_library( OPENCL_LIBRARY );

    // Start TCP Network Server -----------------------------------------------
    typedef server< tcp_transport > tcp_server_t;

    // default config info (bind ip 0.0.0.0, port 4791)
    tcp_server_t::config_info_t tcp_config;

    boost::scoped_ptr< tcp_server_t > tcp_server_ptr( new tcp_server_t( tcp_config ) );
    tcp_server_ptr->start_accept_thread();

    tcp_server_ptr->wait();

    // Free OpenCL Libraries --------------------------------------------------
    opencl_composite::get_instance().free_all();

#if defined( WIN32 )

    WSACleanup();

#endif

    return 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
