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
#ifndef _DCL_PLATFORM_H_
#define _DCL_PLATFORM_H_

#include <string>
#include <vector>
#include "distributedcl_internal.h"
#include "info/dcl_objects.h"
#include "info/platform_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
class device;
class opencl_library;
typedef std::vector< device* > devices_t;
//-----------------------------------------------------------------------------
class platform :
    public dcl::info::cl_object< cl_platform_id, cl_platform_info, CL_INVALID_PLATFORM >,
    public dcl::info::dcl_object< dcl::info::platform_info >
{
public:
    platform(){}
	platform( const opencl_library& opencl, cl_platform_id platform_id = NULL );
    ~platform(){}

    inline void add_device( device* device_ptr )
    {
        devices_.push_back( device_ptr );
    }

    inline const devices_t& get_devices() const
    {
        return devices_;
    }

private:
    devices_t devices_;

	void load();
    void load_string( cl_platform_info info, std::string& out );
};
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
#endif // _DCL_PLATFORM_H_