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
#ifndef _DCL_REMOTE_PROGRAM_H_
#define _DCL_REMOTE_PROGRAM_H_

#include "distributedcl_internal.h"
#include "remote_object.h"
#include "remote_context.h"
#include "info/program_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_program :
    public dcl::info::generic_program,
    public remote_object< remote_program, dcl::network::message::msgReleaseProgram >
{
public:
    remote_program( const remote_context& context_ref, const std::string& source_code ) :
        dcl::info::generic_program( source_code ), 
        remote_object< remote_program, dcl::network::message::msgReleaseProgram >( context_ref.get_session() ),
        context_( context_ref ){}

    ~remote_program(){}

    virtual void build( const std::string& build_options, cl_bool blocking = CL_TRUE );
    virtual void build( const devices_t& devices, const std::string& build_options, cl_bool blocking = CL_TRUE );
    virtual dcl::info::generic_kernel* create_kernel( const std::string& kernel_name );
    virtual cl_build_status get_build_status( const dcl::info::generic_device* device_ptr ) const;
    virtual void get_build_log( const dcl::info::generic_device* device_ptr, std::string& build_log ) const;

private:
    const remote_context& context_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_PROGRAM_H_
