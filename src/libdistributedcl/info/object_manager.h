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
#ifndef _DCL_INFO_OBJECT_MANAGER_H_
#define _DCL_INFO_OBJECT_MANAGER_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <time.h>
#include <map>
#include "distributedcl_internal.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/thread.hpp>
#if defined WIN32
#pragma intrinsic(__rdtsc)
#endif
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
template< typename DCL_TYPE_T >
class object_manager
{
public:
    object_manager() :
        rand_(), dist_( 1, 0x7FFFFFFF ), random_( rand_, dist_ )
    {
#if defined WIN32
        rand_.seed( static_cast<uint32_t>( __rdtsc() ) );
#else
        timeval tv;
        gettimeofday( &tv, NULL );
        rand_.seed( tv.tv_usec );
#endif

        boost::this_thread::yield();
    }

    ~object_manager(){}

    inline void add( DCL_TYPE_T* object_ptr, remote_id_t object_id )
    {
        object_map_[ object_id ] = object_ptr;
    }

    inline remote_id_t add( DCL_TYPE_T* object_ptr )
    {
        remote_id_t object_id;

        do
        {
            object_id = static_cast< remote_id_t >( random_() );

        } while( object_map_.find( object_id ) != object_map_.end() );

        {
            dcl::scoped_lock_t lock(mutex_);
            object_map_.insert( typename object_map_t::value_type( object_id, object_ptr ) );
        }

        //object_ptr->set_remote_id( object_id );

        return( object_id );
    }

    inline void remove( remote_id_t object_id )
    {
        DCL_TYPE_T* ptr = get( object_id );
        object_map_.erase( object_id );
        delete ptr;
    }

    inline void remove( const DCL_TYPE_T* object_ptr )
    {
        typename object_map_t::const_iterator it;

        for( it = object_map_.begin; it != object_map_.end(); ++it )
        {
            if( it->second == object_ptr )
            {
                remove( it->first );

                break;
            }
        }
    }

    inline remote_id_t get( DCL_TYPE_T* object_ptr, bool create_new = false )
    {
        typename object_map_t::const_iterator it;

        for( it = object_map_.begin(); it != object_map_.end(); ++it )
        {
            if( it->second == object_ptr )
            {
                break;
            }
        }

        if( it != object_map_.end() )
        {
            return it->first;
        }

        if( !create_new )
            throw library_exception( (std::string( "object_manager<" ) + DCL_TYPE_T::get_name() + ">::get : Invalid object pointer").c_str() );

        return add( object_ptr );
    }

    inline DCL_TYPE_T* get( remote_id_t object_id ) const
    {
        //std::cerr << "get object: " << object_id << std::endl;
        typename object_map_t::const_iterator it = object_map_.find( object_id );

        if( it == object_map_.end() )
        {
            throw library_exception( (std::string( "object_manager<" ) + DCL_TYPE_T::get_name() + ">::get : Invalid object id").c_str(), object_id );
        }

        return it->second;
    }

    inline bool has( remote_id_t object_id ) const
    {
        typename object_map_t::const_iterator it = object_map_.find( object_id );

        return( it != object_map_.end() );
    }

    inline bool empty() const
    {
        return object_map_.empty();
    }

    inline void clear()
    {
        typename object_map_t::const_iterator it;

        for( it = object_map_.begin(); it != object_map_.end(); ++it )
        {
            try
            {
                delete it->second;
            }
            catch( ... )
            {
            }
        }

        object_map_.clear();
    }

private:
    typedef std::map< remote_id_t, DCL_TYPE_T* > object_map_t;

    dcl::mutex_t mutex_;
    object_map_t object_map_;
    boost::mt19937 rand_;
    boost::uniform_int<> dist_;
    boost::variate_generator< boost::mt19937&, boost::uniform_int<> > random_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_OBJECT_MANAGER_H_
