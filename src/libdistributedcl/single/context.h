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
#ifndef _DCL_CONTEXT_H_
#define _DCL_CONTEXT_H_

#include "distributedcl_internal.h"
#include "opencl_single.h"
#include "info/dcl_objects.h"
#include "info/context_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
class platform;
//-----------------------------------------------------------------------------
class context : 
    public dcl::info::generic_context,
    public opencl_object< cl_context >
{
public:
	context( const context& ctx );
    context( const platform& platform_ref, const devices_t& devices_ref );
    context( const platform& platform_ref, cl_device_type device_type );

    ~context();

    //void add( command_queue& queue );

    //inline context& operator<<( command_queue& cmd_queue )
    //{
    //    add( cmd_queue );
    //    return *this;
    //}

    inline const image_formats_t& get_image2d_formats()
    {
        if( image2d_formats_.empty() )
        {
            load_image_formats( image2d_formats_, CL_MEM_OBJECT_IMAGE2D );
        }

        return( image2d_formats_ );
    }

    inline const image_formats_t& get_image3d_formats()
    {
        if( image3d_formats_.empty() )
        {
            load_image_formats( image3d_formats_, CL_MEM_OBJECT_IMAGE3D );
        }

        return( image3d_formats_ );
    }
	
private:
    virtual void load_devices();

    void load_image_formats( image_formats_t& image_formats, cl_mem_object_type image_type );

    virtual dcl::info::generic_program* do_create_program( const std::string& source_code );

    virtual dcl::info::generic_command_queue*
        do_create_command_queue( const dcl::info::generic_device* device_ptr,
                                 cl_command_queue_properties properties );

    image_formats_t image2d_formats_;
    image_formats_t image3d_formats_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
#endif //_DCL_CONTEXT_H_
