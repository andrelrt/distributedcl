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
#ifndef _DCL_REMOTE_OBJECT_H_
#define _DCL_REMOTE_OBJECT_H_

#include "distributedcl_internal.h"
#include "icd_object_manager.h"
#include "client/session_manager.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
enum dcl_object_types
{
    dcl_platform_id = 0,
    dcl_device_id = 1,
};
//-----------------------------------------------------------------------------
template< typename CL_TYPE_T, typename DCL_TYPE_T, uint32_t DCL_TYPE_ID >
class icd_object
{
public:
    static const uint32_t type_id = DCL_TYPE_ID;

    inline CL_TYPE_T get_icd_obj() const
    {
        return icd_obj_;
    }

protected:
    CL_TYPE_T icd_obj_;

    icd_object(){}

    inline void create_icd_obj( DCL_TYPE_T* ptr )
    {
        icd_obj_ = icd_object_manager::get_instance().get_cl_id< DCL_TYPE_T >( ptr );
    }
};
//-----------------------------------------------------------------------------
class remote_object
{
protected:
    dcl::remote_id_t remote_id_;
    dcl::network::client::session_manager::session_t& session_ref_;

    remote_object( dcl::network::client::session_manager::session_t& session_ref, dcl::remote_id_t remote_id ) :
        session_ref_( session_ref ), remote_id_( remote_id ) {}
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_OBJECT_H_
