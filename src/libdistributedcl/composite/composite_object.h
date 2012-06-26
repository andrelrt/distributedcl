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
#ifndef _DCL_COMPOSITE_OBJECT_H_
#define _DCL_COMPOSITE_OBJECT_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <map>
#include "distributedcl_internal.h"
#include "info/context_info.h"
//-----------------------------------------------------------------------------
//namespace dcl {
//namespace info {
//class memory;
//struct memory_object_info;
//class image;
//struct image_info;
//class sampler;
//struct sampler_info;
//class generic_program;
//}}
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
class composite_context;
class composite_event;
//-----------------------------------------------------------------------------
template< class DCL_TYPE_T >
class composite_object
{
public:
    typedef std::map< dcl::info::generic_context*, DCL_TYPE_T* > composite_map_t;
    typedef typename composite_map_t::const_iterator const_iterator;

protected:
    const composite_context& context_ref_;
    composite_map_t composite_map_;

    typedef typename composite_map_t::iterator iterator;

    inline iterator begin()
    {
        return composite_map_.begin();
    }

    inline iterator end()
    {
        return composite_map_.end();
    }

public:
    composite_object< DCL_TYPE_T >( const composite_context& context_ref ) :
        context_ref_( context_ref )
    {
    }

    ~composite_object< DCL_TYPE_T >()
    {
        for( iterator it = composite_map_.begin(); it != composite_map_.end(); it++ )
        {
            delete it->second;
        }
    }

    inline void insert_context_object( dcl::info::generic_context* context_ptr, DCL_TYPE_T* new_obj )
    {
        composite_map_.insert( typename composite_map_t::value_type( context_ptr, new_obj ) );
    }

    inline const composite_context& get_context() const
    {
        return context_ref_;
    }

    inline const_iterator map_begin() const
    {
        return composite_map_.begin();
    }

    inline const_iterator map_end() const
    {
        return composite_map_.end();
    }

    inline DCL_TYPE_T* find( const dcl::info::generic_context* ctx ) const
    {
        const_iterator it = composite_map_.find( const_cast< dcl::info::generic_context* >( ctx ) );

        if( it != composite_map_.end() )
        {
            return it->second;
        }

        throw library_exception( CL_INVALID_CONTEXT );
    }
};
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
#endif //_DCL_COMPOSITE_OBJECT_H_
