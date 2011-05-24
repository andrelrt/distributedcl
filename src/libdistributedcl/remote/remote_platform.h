/*
 * Copyright (c) 2009-2010 André Tupinambá (andrelrt@gmail.com)
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
#ifndef _DCL_REMOTE_PLATFORM_H_
#define _DCL_REMOTE_PLATFORM_H_

#include <vector>
#include "distributedcl_internal.h"
#include "library_exception.h"
#include "remote_object.h"
#include "info/platform_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_device;
typedef std::vector< remote_device* > remote_devices_t;
//-----------------------------------------------------------------------------
class remote_platform :
    public cl_object< cl_platform_id, cl_platform_info, CL_INVALID_PLATFORM >,
    public icd_object< cl_platform_id, remote_platform, dcl_platform_id >,
    public dcl_object< dcl::info::platform_info >
{
public:
    static const remote_platform& get_instance()
    {
        if( instance_ptr_ == NULL )
        {
            instance_ptr_ = new remote_platform();
        }
        return( *instance_ptr_ );
    }

    inline const remote_devices_t& get_devices() const
    {
        if( remote_devices_.empty() )
        {
            const_cast< remote_platform* >( this )->load_devices();
        }
        return remote_devices_;
    }

private:
    static remote_platform* instance_ptr_;
    remote_devices_t remote_devices_;

    remote_platform()
    {
        create_icd_obj( this );

        local_info_.profile_.assign( "FULL_PROFILE" );
        local_info_.version_.assign( "OpenCL 1.1 DistributedCL 0.1" );
	    local_info_.name_.assign( "DistributedCL" );
        local_info_.vendor_.assign( "DistributedCL Project" );
        local_info_.extensions_.assign( "cl_khr_icd" );
    }

    ~remote_platform(){}

    void load_devices();
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_PLATFORM_H_
