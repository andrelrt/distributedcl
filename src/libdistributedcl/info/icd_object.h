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
#ifndef _DCL_ICD_OBJECT_H_
#define _DCL_ICD_OBJECT_H_

#include <set>
#include <map>
#include "distributedcl_internal.h"
#include "opencl_functions.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
enum dcl_object_types
{
    dcl_platform_id = 0,
    dcl_device_id = 1,
    dcl_context_id = 2,
    dcl_program_id = 3,
    dcl_kernel_id = 4,
    dcl_command_queue_id = 5,
    dcl_memory_id = 6,
};
//-----------------------------------------------------------------------------
template< typename CL_TYPE_T, uint32_t DCL_TYPE_ID >
class icd_object
{
public:
    static const uint32_t type_id = DCL_TYPE_ID;

    inline CL_TYPE_T get_icd_obj() const
    {
        return icd_obj_;
    }

    inline void set_icd_obj( CL_TYPE_T icd_obj )
    {
        icd_obj_ = icd_obj;
    }

protected:
    CL_TYPE_T icd_obj_;

    icd_object() : icd_obj_( NULL ){}
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_ICD_OBJECT_H_
