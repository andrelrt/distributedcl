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
#ifndef _DCL_INFO_PLATFORM_H_
#define _DCL_INFO_PLATFORM_H_

#include <string>
#include "distributedcl_internal.h"
#include "library_exception.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
struct platform_info
{
public:
    std::string profile_;
    std::string version_;
    std::string name_;
    std::string vendor_;
    std::string extensions_;

    platform_info()
    {
        profile_.assign( "FULL_PROFILE" );
        version_.assign( "OpenCL 1.1 DistributedCL 0.1" );
	    name_.assign( "DistributedCL" );
        vendor_.assign( "DistributedCL Project" );
        extensions_.assign( "cl_khr_icd" );
    }

    platform_info( const platform_info& other ) :
        profile_( other.profile_ ),
        version_( other.version_ ),
        name_( other.name_ ),
        vendor_( other.vendor_ ),
        extensions_( other.extensions_ )
    {}

    inline bool operator< ( const platform_info& other ) const
    {
        return name_ < other.name_;
    }

    inline platform_info& operator= ( const platform_info& other )
    {
        profile_ = other.profile_;
        version_ = other.version_;
        name_ = other.name_;
        vendor_ = other.vendor_;
        extensions_ = other.extensions_;

        return *this;
    }

    inline size_t get_info_size( cl_platform_info info ) const
    {
        switch( info )
        {
            case CL_PLATFORM_PROFILE:    return profile_.length() + 1;
            case CL_PLATFORM_VERSION:    return version_.length() + 1;
            case CL_PLATFORM_NAME:       return name_.length() + 1;
            case CL_PLATFORM_VENDOR:     return vendor_.length() + 1;
            case CL_PLATFORM_EXTENSIONS: return extensions_.length() + 1;

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }

    inline const void* get_info_pointer( cl_platform_info info ) const
    {
        switch( info )
        {
            case CL_PLATFORM_PROFILE:    return profile_.c_str();
            case CL_PLATFORM_VERSION:    return version_.c_str();
            case CL_PLATFORM_NAME:       return name_.c_str();
            case CL_PLATFORM_VENDOR:     return vendor_.c_str();
            case CL_PLATFORM_EXTENSIONS: return extensions_.c_str();

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_PLATFORM_H_
