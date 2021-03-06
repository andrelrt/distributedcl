/*
 * Copyright (c) 2009-2012 Andr� Tupinamb� (andrelrt@gmail.com)
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
#include "distributedcl_internal.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include "icd/icd_object_manager.h"
#include "composite/opencl_composite.h"
#include "composite/composite_platform.h"
#include "config/client_config.h"
using dcl::icd::icd_object_manager;
using dcl::composite::opencl_composite;
using dcl::composite::composite_platform;
using dcl::config::client_config;

#if defined( WIN32 )
#define OPENCL_LIBRARY  "C:\\WINDOWS\\System32\\OpenCL.dll"
#else
#define OPENCL_LIBRARY  "/usr/lib/libOpenCL.so"
#endif
//-----------------------------------------------------------------------------
void setup_library()
{
    try
    {
        // Load config ------------------------------------------------------------
        std::string filepath;
        client_config::get_config_path( filepath, "libdcl.conf" );

		std::cout << "DistributedCL Client 0.1" << std::endl;

        client_config config;

        try
        {
			std::cout << "Loading config file: " << filepath << std::endl;
            config.parse( filepath );
        }
        catch( boost::program_options::error& ex )
        {
            std::cerr << ex.what() << std::endl << std::endl;
            std::cerr << "Error parsing config file. Running with default parameters." << std::endl;
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

        for( it = servers.begin(); it != servers.end(); ++it )
        {
			std::cout << "Connecting remote DistributedCL server: " << *it << std::endl;
            opencl_composite::get_instance().add_remote( *it );
        }

        const composite_platform& platform_ref = opencl_composite::get_instance().get_platform();
        composite_platform* platform_ptr = const_cast< composite_platform* >( &platform_ref );

        icd_object_manager::get_instance().get_cl_id< composite_platform >( platform_ptr );

		std::cout << "DistributedCL client ready." << std::endl;
    }
	catch( dcl::library_exception& ex )
	{
        std::cerr << "Exception: " << ex.text() << " -- " << ex.get_error() << std::endl;
	}
}
//-----------------------------------------------------------------------------
void free_library()
{
    opencl_composite::get_instance().free_all();
}
//-----------------------------------------------------------------------------
#if defined(__GNUC__)

void __attribute__ ((constructor)) so_load()
{
    try
    {
        setup_library();
    }
    catch( dcl::library_exception& ex )
    {
    }
    catch( ... )
    {
    }
}

void __attribute__ ((destructor)) so_unload()
{
    try
    {
        free_library();
    }
    catch( dcl::library_exception& ex )
    {
    }
    catch( ... )
    {
    }
}

#elif defined(WIN32) // __GNUC__ -> WIN32

#define WINDOWS_MEAN_AND_LEAN
#include <windows.h>
//-----------------------------------------------------------------------------
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
            try
            {
                // Winsock startup
                WSADATA wsaData;
                WSAStartup( MAKEWORD(1, 1), &wsaData );

                setup_library();
            }
            catch( dcl::library_exception& )
            {
                return FALSE;
            }
            catch( ... )
            {
                return FALSE;
            }
            break;

        case DLL_PROCESS_DETACH:
            try
            {
                free_library();

                WSACleanup();
            }
            catch( dcl::library_exception& )
            {
                return FALSE;
            }
            catch( ... )
            {
                return FALSE;
            }
            break;
    }
    return TRUE;
}
//-----------------------------------------------------------------------------
#endif // WIN32
