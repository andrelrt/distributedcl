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
#include <boost/program_options.hpp>
#include "clbench.h"
//-----------------------------------------------------------------------------
int main( int argc, char* argv[])
{
    int begin, end, iterations;
    boost::program_options::options_description param( "Parameters" );

    param.add_options()
        ( "help,h", "Show this message" )
        ( "begin,b",
          boost::program_options::value< int >( &begin )->default_value( 4 ),
          "Matrix begin size" )
        ( "end,e",
          boost::program_options::value< int >( &end )->default_value( 4 ),
          "Matrix end size" )
        ( "run,r",
          boost::program_options::value< int >( &iterations )->default_value( 30 ),
          "Iterations count" )
        ( "double,d", "Use double values" )
        ( "float,f", "Use float values (default)" )
        ( "int,i", "Use int values" )
        ;

    boost::program_options::variables_map config;
    boost::program_options::store( boost::program_options::parse_command_line( argc, argv, param ), config );
    boost::program_options::notify( config );

    if( config.count( "help" ) != 0 )
    {
        std::cout << param << std::endl;
        return 1;
    }

    if( config.count( "double" ) != 0 )
    {
        dcl::benchmark::clbench<cl_double> bench( begin, end, iterations );
        bench.run();
    }
    else if( config.count( "int" ) != 0 )
    {
        dcl::benchmark::clbench<cl_int> bench( begin, end, iterations );
        bench.run();
    }
    else
    {
        dcl::benchmark::clbench<cl_float> bench( begin, end, iterations );
        bench.run();
    }

    return 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
