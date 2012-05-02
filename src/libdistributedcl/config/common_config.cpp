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
#if defined( WIN32 )
#include <Shlobj.h>
#endif
#include <boost/filesystem.hpp>
#include "common_config.h"
using boost::program_options::error;
using boost::program_options::value;
using boost::program_options::store;
using boost::program_options::notify;
using boost::program_options::parse_command_line;
using boost::program_options::parse_config_file;
using boost::program_options::basic_parsed_options;
//-----------------------------------------------------------------------------
namespace dcl {
namespace config {
//-----------------------------------------------------------------------------
common_config::common_config( const std::string& parameters_name ) :
    cpu_only_( false ), gpu_only_( false ), parameters_( parameters_name )
{
    parameters_.add_options()
        ( "help,h", "Show this message" )
        ( "local,l", value< bool >( &local_ ), "Load local OpenCL libraries" )
        ( "server,s", value< std::vector<std::string> >( &servers_ )->multitoken(),
          "Connect DistributedCL servers (address:port)" )
        ( "cpu-only,c", value< bool >( &cpu_only_ ), "Use only CPU devices" )
        ( "gpu-only,g", value< bool >( &gpu_only_ ), "Use only GPU devices" )
        ;
}
//-----------------------------------------------------------------------------
void common_config::parse( int argc, char *argv[] )
{
    store( parse_command_line( argc, argv, parameters_ ), config_ );
    notify( config_ );

    if( cpu_only_ && gpu_only_ )
    {
        throw boost::program_options::error( "Cannot use --cpu-only and --gpu-only together." );
    }
}
//-----------------------------------------------------------------------------
void common_config::parse( const std::string& filename )
{
    if( boost::filesystem::exists( filename ) )
    {
        store( parse_config_file<char>( filename.c_str(), parameters_ ), config_ );
        notify( config_ );

        if( cpu_only_ && gpu_only_ )
        {
            throw boost::program_options::error( "Cannot use --cpu-only and --gpu-only together." );
        }
    }
}
//-----------------------------------------------------------------------------
void common_config::get_config_path( std::string& filepath, const std::string& filename, bool user )
{
#if defined( WIN32 )

    char path[ MAX_PATH ];

    HRESULT result = SHGetFolderPathA( NULL,
                                       user ? CSIDL_APPDATA : CSIDL_COMMON_APPDATA,
                                       NULL, 0, path );

    filepath.clear();

    if( result == S_OK )
    {
        filepath.assign( path );
        filepath.append( "\\DistributedCL\\" );
        filepath.append( filename );
    }

#else

    filepath.assign( user ? "~/.dcl/" : "/etc/dcl/" );
    filepath.append( filename );

#endif
}
//-----------------------------------------------------------------------------
}} // namespace dcl::config
//-----------------------------------------------------------------------------
