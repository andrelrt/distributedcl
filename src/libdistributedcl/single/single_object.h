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
#ifndef _DCL_SINGLE_OBJECT_H_
#define _DCL_SINGLE_OBJECT_H_

#include "distributedcl_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
class context;
//-----------------------------------------------------------------------------
template< class CONTEXT_WRAPPER_T >
struct context_wrapper{};
//-----------------------------------------------------------------------------
template< class CONTEXT_WRAPPER_T >
class context_object : 
    public context_wrapper< CONTEXT_WRAPPER_T >
{
private:
    context* context_ptr_;

public:
    context_object()
        : context_ptr_( NULL )
    {
    }

    context_object( const context_object& copy )
        : context_ptr_( copy.context_ptr_ )
    {
    }

    ~context_object(){}

    inline void set_context( context* context_ptr, CONTEXT_WRAPPER_T* obj_ptr )
    {
        context_wrapper< CONTEXT_WRAPPER_T >::context_attach( context_ptr, obj_ptr );

        context_ptr_ = context_ptr;
    }

    inline const context* get_context() const
    {
        return context_ptr_;
    }
};
//-----------------------------------------------------------------------------
template< class OBJECT_INFO_T >
struct single_object
{
    typedef OBJECT_INFO_T object_info_t;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
#endif //_DCL_SINGLE_OBJECT_H_
