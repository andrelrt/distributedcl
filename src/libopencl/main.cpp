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
#include "distributedcl_internal.h"
#include "composite/opencl_composite.h"
using dcl::composite::opencl_composite;
//-----------------------------------------------------------------------------
void setup_library()
{
    opencl_composite::get_instance().add_remote( "127.0.0.1:4791" );
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
            catch( dcl::library_exception& ex )
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
            catch( dcl::library_exception& ex )
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
