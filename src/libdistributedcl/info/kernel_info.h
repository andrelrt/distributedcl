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
#ifndef _DCL_INFO_KERNEL_H_
#define _DCL_INFO_KERNEL_H_

#include <string>
#include "distributedcl_internal.h"
#include "library_exception.h"
#include "dcl_objects.h"
#include "icd_object.h"
#include "command_queue_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
class ndrange
{
private:
    size_t dimensions_;
    size_t range_[ 3 ];

public:
    ndrange() : 
        dimensions_( 0 ) 
    {
        range_[ 0 ] = 0;
        range_[ 1 ] = 0;
        range_[ 2 ] = 0;
    }

    ndrange( cl_uint dim, const size_t* range ) : 
        dimensions_( dim )
    {
        range_[ 0 ] = 0;
        range_[ 1 ] = 0;
        range_[ 2 ] = 0;

        if( range != NULL )
        {
            switch( dim )
            {
                case 3:
                    range_[ 2 ] = range[ 2 ];
                    // fall through

                case 2:
                    range_[ 1 ] = range[ 1 ];
                    // fall through

                case 1:
                    range_[ 0 ] = range[ 0 ];
                    break;

                default:
                    throw library_exception( CL_INVALID_WORK_DIMENSION );
            }
        }
        else
        {
            dimensions_ = 0;
        }
    }

    ndrange( const ndrange& range ) : 
        dimensions_( range.dimensions_ ) 
    {
        range_[ 0 ] = range.range_[ 0 ];
        range_[ 1 ] = range.range_[ 1 ];
        range_[ 2 ] = range.range_[ 2 ];
    }

    inline void copy( const ndrange& range )
    {
        dimensions_ = range.dimensions_;
        range_[ 0 ] = range.range_[ 0 ];
        range_[ 1 ] = range.range_[ 1 ];
        range_[ 2 ] = range.range_[ 2 ];
    }

    inline const size_t* get_pointer() const
    {
        return (dimensions_ == 0) ? NULL : range_;
    }

    inline const size_t* get_range() const
    {
        return range_;
    }

    inline size_t get_dimensions() const
    {
        return dimensions_;
    }
};
//-----------------------------------------------------------------------------
struct kernel_info
{
public:
    std::string name_;

    inline kernel_info(){}
    inline kernel_info( const std::string& name ) : name_( name ) {}
};
//-----------------------------------------------------------------------------
class generic_kernel :
    public cl_object< cl_kernel, cl_kernel_info, CL_INVALID_KERNEL >,
    public icd_object< cl_kernel, dcl_kernel_id >,
    public dcl_object< kernel_info >
{
public:
    virtual ~generic_kernel(){}

    generic_kernel( const std::string& name )
    {
        local_info_.name_.assign( name );
    }

    inline const std::string& get_name() const
    {
        return local_info_.name_;
    }

    virtual void execute( const generic_command_queue* queue_ptr, const ndrange& offset, 
                          const ndrange& global, const ndrange& local ) = 0;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_KERNEL_H_

