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
#ifndef _DCL_LIBRARY_EXCEPTION_H_
#define _DCL_LIBRARY_EXCEPTION_H_

#include <string>
#include <exception>
//-----------------------------------------------------------------------------
namespace dcl {
//-----------------------------------------------------------------------------
class library_exception :  public std::exception
{
public:
    library_exception( int error ) throw() : error_( error ) { printf( "Error: %d\n", error ); }
    library_exception( const char* szText ) throw() : error_( -30 ), text_( szText ) { printf( "Error: %s\n", szText ); }
    library_exception( const char* szText, int error ) throw() : error_( error ), text_( szText ) { printf( "Error: %s (%d)\n", szText, error ); }

	~library_exception() throw(){};

    inline int get_error() const
    {
        return error_;
    }

    inline const std::string& text() const
    {
        return text_;
    }

private:
    int error_;
    std::string text_;
};
//-----------------------------------------------------------------------------
} // namespace dcl
//-----------------------------------------------------------------------------
#endif //_DCL_LIBRARY_EXCEPTION_H_
