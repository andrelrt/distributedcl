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
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include "distributedcl_internal.h"
#include "network/server.h"
#include "network/tcp_transport.h"
#include "composite/opencl_composite.h"
#include "config/server_config.h"
using dcl::network::server::server;
using dcl::network::platform::tcp_transport;
using dcl::composite::opencl_composite;
using dcl::config::server_config;

#define SERVER_CONFIG_FILE  "dcld.conf"

#if defined( WIN32 )
#define OPENCL_LIBRARY  "C:\\WINDOWS\\System32\\OpenCL.dll"
#else
#define OPENCL_LIBRARY  "/usr/lib/libOpenCL.so"
#endif
//-----------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
#if defined( WIN32 )

    // Winsock startup
    WSADATA wsaData;
    WSAStartup( MAKEWORD(1, 1), &wsaData );

#endif
	
	try
	{
        // Load config ------------------------------------------------------------
        std::string filepath;
        server_config::get_config_path( filepath, "dcld.conf" );

        server_config config;

		std::cout << "DistributedCL Server 0.1" << std::endl;

        try
        {
            config.parse( argc, argv );
        }
        catch( boost::program_options::error& ex )
        {
            std::cerr << ex.what() << std::endl << std::endl;
            std::cerr << config.get_description() << std::endl;
            return -1;
        }

        try
        {
			std::cout << "Loading config file: " << filepath << std::endl;
            config.parse( filepath );
        }
        catch( boost::program_options::error& ex )
        {
            std::cerr << ex.what() << std::endl << std::endl;
            std::cerr << "Error parsing config file." << std::endl << "Running with default parameters." << std::endl;
        }

		// Load OpenCL Libraries --------------------------------------------------
        if( config.load_local() )
        {
			std::cout << "Loading OpenCL library: " << OPENCL_LIBRARY << std::endl;
		    opencl_composite::get_instance().add_library( OPENCL_LIBRARY );
        }

	    // Connect remote DistributedCL servers --------------------------------------
        const std::vector< std::string >& servers = config.get_servers();
        std::vector< std::string >::const_iterator it;

        for( it = servers.begin(); it != servers.end(); it++ )
        {
			std::cout << "Connecting remote DistributedCL server: " << *it << std::endl;
            opencl_composite::get_instance().add_remote( *it );
        }

		// Start TCP Network Server -----------------------------------------------
		typedef server< tcp_transport > tcp_server_t;

		// default config info (bind ip 0.0.0.0, port 4791)
		tcp_server_t::config_info_t tcp_config;

		boost::scoped_ptr< tcp_server_t > tcp_server_ptr( new tcp_server_t( tcp_config ) );
		tcp_server_ptr->start_accept_thread();

		std::cout << "DistributedCL server started." << std::endl;

		tcp_server_ptr->wait();

		// Free OpenCL Libraries --------------------------------------------------
		opencl_composite::get_instance().free_all();
	}
	catch( dcl::library_exception& ex )
	{
        std::cerr << "Exception: " << ex.text() << " -- " << ex.get_error() << std::endl;
	}

#if defined( WIN32 )

    WSACleanup();

#endif

    return 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
