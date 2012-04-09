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
#include "distributedcl_internal.h"
#include <boost/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include "icd/icd_object_manager.h"
#include "composite/opencl_composite.h"
#include "composite/composite_platform.h"
using dcl::icd::icd_object_manager;
using dcl::composite::opencl_composite;
using dcl::composite::composite_platform;

#if defined( WIN32 )
#define OPENCL_LIBRARY  "C:\\WINDOWS\\System32\\OpenCL.dll"
#else
#define OPENCL_LIBRARY  "libOpenCL.so"
#endif
//-----------------------------------------------------------------------------
static boost::thread* library_thread_ptr = NULL;
static boost::interprocess::interprocess_semaphore* wait_ptr = NULL;
void library_thread();
//-----------------------------------------------------------------------------
void setup_library()
{
    wait_ptr = new boost::interprocess::interprocess_semaphore( 0 );
    library_thread_ptr = new boost::thread( library_thread );
}
//-----------------------------------------------------------------------------
void free_library()
{
    wait_ptr->post();
    library_thread_ptr->join();

    delete library_thread_ptr;
    delete wait_ptr;
}
//-----------------------------------------------------------------------------
void library_thread()
{
	//opencl_composite::get_instance().add_library( OPENCL_LIBRARY );
    opencl_composite::get_instance().add_remote( "127.0.0.1:4791" );

    const composite_platform& platform_ref = opencl_composite::get_instance().get_platform();
    composite_platform* platform_ptr = const_cast< composite_platform* >( &platform_ref );

    icd_object_manager::get_instance().get_cl_id< composite_platform >( platform_ptr );

    wait_ptr->wait();

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
