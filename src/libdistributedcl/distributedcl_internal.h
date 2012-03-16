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
#ifndef _DCL_INTERNAL_H_
#define _DCL_INTERNAL_H_

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock.h>
#else
#include <arpa/inet.h>
#endif

#include <stdint.h>
#include <vector>
#include "library_exception.h"
//-----------------------------------------------------------------------------
namespace dcl { namespace info {
class generic_platform;
class generic_device;
}}
//-----------------------------------------------------------------------------
namespace dcl {
//-----------------------------------------------------------------------------
typedef uint16_t remote_id_t;
typedef std::vector< remote_id_t > remote_ids_t;
typedef std::vector< cl_image_format > image_formats_t;
typedef std::vector< dcl::info::generic_platform* > platforms_t;
typedef std::vector< dcl::info::generic_device* > devices_t;
//-----------------------------------------------------------------------------
static const union { long one; char little; } is_endian = {1};
//-----------------------------------------------------------------------------
static inline uint16_t host_to_network( uint16_t value )
{
    if( is_endian.little )
    {
        return value;
    }
    else
    {
        return (value >> 8) | (value << 8);
    }
}
//-----------------------------------------------------------------------------
static inline uint32_t host_to_network( uint32_t value )
{
    if( is_endian.little )
    {
        return value;
    }
    else
    {
        return ((value << 24) | (value >> 24)) |
               ((value << 8) & 0x00ff0000) |
               ((value >> 8) & 0x0000ff00);
    }
}
//-----------------------------------------------------------------------------
static inline uint64_t host_to_network( uint64_t value )
{
    if( is_endian.little )
    {
        return value;
    }
    else
    {
        return ((value << 56) | (value >> 56)) |
               ((value << 40) & 0x00ff000000000000) |
               ((value << 24) & 0x0000ff0000000000) |
               ((value << 8)  & 0x000000ff00000000) |
               ((value >> 8)  & 0x00000000ff000000) |
               ((value >> 24) & 0x0000000000ff0000) |
               ((value >> 40) & 0x000000000000ff00);
    }
}
//-----------------------------------------------------------------------------
static inline uint16_t network_to_host( uint16_t value )
{
    return host_to_network( value );
}
//-----------------------------------------------------------------------------
static inline uint32_t network_to_host( uint32_t value )
{
    return host_to_network( value );
}
//-----------------------------------------------------------------------------
static inline uint64_t network_to_host( uint64_t value )
{
    return host_to_network( value );
}
//-----------------------------------------------------------------------------
} // namespace dcl
//-----------------------------------------------------------------------------
#endif // _DCL_INTERNAL_H_
