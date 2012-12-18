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
    unsigned begin, end, step, iterations;
    boost::program_options::options_description param( "Parameters" );

    param.add_options()
        ( "help,h", "Show this message" )
        ( "begin,b",
          boost::program_options::value< unsigned >( &begin )->default_value( 10 ),
          "Matrix begin size" )
        ( "end,e",
          boost::program_options::value< unsigned >( &end ),
          "Matrix end size" )
        ( "step,s",
          boost::program_options::value< unsigned >( &step )->default_value( 1 ),
          "Matrix size step" )
        ( "run,r",
          boost::program_options::value< unsigned >( &iterations )->default_value( 30 ),
          "Iterations count" )
        ( "power", "Power 2 sizes" )
        ( "double,d", "Use double values" )
        ( "float,f", "Use float values (default)" )
        ( "int,i", "Use int values" )
        ;

    boost::program_options::variables_map config;
    try
    {
        boost::program_options::store( boost::program_options::parse_command_line( argc, argv, param ), config );
        boost::program_options::notify( config );
    }
    catch( ... )
    {
        std::cout << param << std::endl;
        return 1;
    }

    if( (config.count( "end" ) == 0) || (end < begin) )
    {
        end = begin;
    }

    if( config.count( "help" ) != 0 )
    {
        std::cout << param << std::endl;
        return 1;
    }

    std::vector<uint32_t> sizes;
    if( config.count( "power" ) != 0 )
    {
        uint32_t size = 2;

        do
        {
            sizes.push_back( size );

            size <<= 1;

        } while( size <= end );
    }
    else
    {
        for( uint32_t i = begin; i <= end; i += step )
        {
            sizes.push_back( i );
        }
    }

    if( config.count( "double" ) != 0 )
    {
        dcl::benchmark::clbench<cl_double> bench( sizes, iterations );
        bench.run();
    }
    else if( config.count( "int" ) != 0 )
    {
        dcl::benchmark::clbench<cl_int> bench( sizes, iterations );
        bench.run();
    }
    else
    {
        dcl::benchmark::clbench<cl_float> bench( sizes, iterations );
        bench.run();
    }

    return 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
