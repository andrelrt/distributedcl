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
#ifndef _DCL_CONFIG_COMMON_H_
#define _DCL_CONFIG_COMMON_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "library_exception.h"
#include <boost/program_options.hpp>
//-----------------------------------------------------------------------------
namespace dcl {
namespace config {
//-----------------------------------------------------------------------------
class common_config
{
public:
    void parse( int argc, char *argv[] );
    void parse( const std::string& filename );

    inline const boost::program_options::options_description& get_description() const
    {
        return parameters_;
    }

    inline bool load_local() const
    {
        return local_;
    }

    inline const std::vector< std::string > get_servers() const
    {
        return servers_;
    }

    static void get_config_path( std::string& filepath, const std::string& filename, bool user = false );

protected:
    bool local_;
    bool cpu_only_;
    bool gpu_only_;
    std::vector< std::string > servers_;

    common_config( const std::string& parameters_name );
    virtual ~common_config(){}

    inline boost::program_options::options_description& get_parameters()
    {
        return parameters_;
    }

private:
    boost::program_options::variables_map config_;
    boost::program_options::options_description parameters_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_CONFIG_COMMON_H_

