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
#ifndef _DCL_REMOTE_OBJECT_H_
#define _DCL_REMOTE_OBJECT_H_

#include "distributedcl_internal.h"
//-----------------------------------------------------------------------------
namespace dcl { namespace network { namespace client { class session; }}}
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
template< typename CL_TYPE_T, typename CL_TYPE_INFO, int CL_TYPE_INVALID_VALUE, typename DCL_TYPE_INFO >
class remote_object
{
public:
    typedef CL_TYPE_T cl_type_t;
    typedef CL_TYPE_INFO cl_type_info_t;
    static const cl_int invalid_value_error = CL_TYPE_INVALID_VALUE;
    typedef DCL_TYPE_INFO dcl_info_t;

    remote_object( dcl::network::client::session& session_ref, dcl::remote_id_t remote_id ) :
        session_ref_( session_ref ), remote_id_( remote_id ) {}

    ~remote_object(){}

protected:
    dcl_info_t local_info_;
    dcl::remote_id_t remote_id_;
    dcl::network::client::session& session_ref_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_OBJECT_H_
