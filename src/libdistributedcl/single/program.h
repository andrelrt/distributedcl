/*
 * Copyright (c) 2009-2012 Andr� Tupinamb� (andrelrt@gmail.com)
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
#ifndef _DCL_PROGRAM_H_
#define _DCL_PROGRAM_H_

#include <map>
#include <string>
#include "distributedcl_internal.h"
#include "single_object.h"
#include "opencl_library.h"
#include "info/program_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
class program :
    public dcl::info::generic_program,
    public opencl_object< cl_program >,
    public context_object< program >
{
public:
    program( const context& context_ref, const std::string& source_code );
    program( const context& context_ref, const dcl::devices_t& devices,
             const size_t* lengths, const unsigned char** binaries, cl_int* binary_status );
    ~program();

    // TODO: Create a version of build method using the pfn_notify callback
    virtual void build( const std::string& build_options, cl_bool blocking = CL_TRUE );
    virtual void build( const devices_t& devices, const std::string& build_options, cl_bool blocking = CL_TRUE );
    virtual dcl::info::generic_kernel* create_kernel( const std::string& kernel_name );
    virtual cl_build_status get_build_status( const dcl::info::generic_device* device_ptr ) const;
    virtual void get_build_log( const dcl::info::generic_device* device_ptr, std::string& build_log ) const;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
#endif //_DCL_PROGRAM_H_
