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
#ifndef _DCL_ICD_OBJECT_MANAGER_H_
#define _DCL_ICD_OBJECT_MANAGER_H_

#include <set>
#include <map>
#include "distributedcl_internal.h"
#include "info/opencl_functions.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace icd {
//-----------------------------------------------------------------------------
#define DECLARE_FUNCTION(x) dcl::info::x##_t x

struct icd_dispatch_table
{
    void* dummy;

    DECLARE_FUNCTION( clGetPlatformIDs );
    DECLARE_FUNCTION( clGetPlatformInfo );
    DECLARE_FUNCTION( clGetDeviceIDs );
    DECLARE_FUNCTION( clGetDeviceInfo );
    DECLARE_FUNCTION( clCreateContext  );
    DECLARE_FUNCTION( clCreateContextFromType );
    DECLARE_FUNCTION( clRetainContext );
    DECLARE_FUNCTION( clReleaseContext );
    DECLARE_FUNCTION( clGetContextInfo );
    DECLARE_FUNCTION( clCreateCommandQueue );
    DECLARE_FUNCTION( clRetainCommandQueue );
    DECLARE_FUNCTION( clReleaseCommandQueue );
    DECLARE_FUNCTION( clGetCommandQueueInfo );
    DECLARE_FUNCTION( clSetCommandQueueProperty );
    DECLARE_FUNCTION( clCreateBuffer );
    DECLARE_FUNCTION( clCreateImage2D );
    DECLARE_FUNCTION( clCreateImage3D );
    DECLARE_FUNCTION( clRetainMemObject );
    DECLARE_FUNCTION( clReleaseMemObject );
    DECLARE_FUNCTION( clGetSupportedImageFormats );
    DECLARE_FUNCTION( clGetMemObjectInfo );
    DECLARE_FUNCTION( clGetImageInfo );
    DECLARE_FUNCTION( clCreateSampler );
    DECLARE_FUNCTION( clRetainSampler );
    DECLARE_FUNCTION( clReleaseSampler );
    DECLARE_FUNCTION( clGetSamplerInfo );
    DECLARE_FUNCTION( clCreateProgramWithSource );
    DECLARE_FUNCTION( clCreateProgramWithBinary );
    DECLARE_FUNCTION( clRetainProgram );
    DECLARE_FUNCTION( clReleaseProgram );
    DECLARE_FUNCTION( clBuildProgram );
    DECLARE_FUNCTION( clUnloadCompiler );
    DECLARE_FUNCTION( clGetProgramInfo );
    DECLARE_FUNCTION( clGetProgramBuildInfo );
    DECLARE_FUNCTION( clCreateKernel );
    DECLARE_FUNCTION( clCreateKernelsInProgram );
    DECLARE_FUNCTION( clRetainKernel );
    DECLARE_FUNCTION( clReleaseKernel );
    DECLARE_FUNCTION( clSetKernelArg );
    DECLARE_FUNCTION( clGetKernelInfo );
    DECLARE_FUNCTION( clGetKernelWorkGroupInfo );
    DECLARE_FUNCTION( clWaitForEvents );
    DECLARE_FUNCTION( clGetEventInfo );
    DECLARE_FUNCTION( clRetainEvent );
    DECLARE_FUNCTION( clReleaseEvent );
    DECLARE_FUNCTION( clGetEventProfilingInfo );
    DECLARE_FUNCTION( clFlush );
    DECLARE_FUNCTION( clFinish );
    DECLARE_FUNCTION( clEnqueueReadBuffer );
    DECLARE_FUNCTION( clEnqueueWriteBuffer );
    DECLARE_FUNCTION( clEnqueueCopyBuffer );
    DECLARE_FUNCTION( clEnqueueReadImage );
    DECLARE_FUNCTION( clEnqueueWriteImage );
    DECLARE_FUNCTION( clEnqueueCopyImage );
    DECLARE_FUNCTION( clEnqueueCopyImageToBuffer );
    DECLARE_FUNCTION( clEnqueueCopyBufferToImage );
    DECLARE_FUNCTION( clEnqueueMapBuffer );
    DECLARE_FUNCTION( clEnqueueMapImage );
    DECLARE_FUNCTION( clEnqueueUnmapMemObject );
    DECLARE_FUNCTION( clEnqueueNDRangeKernel );
    DECLARE_FUNCTION( clEnqueueTask );
    DECLARE_FUNCTION( clEnqueueNativeKernel );
    DECLARE_FUNCTION( clEnqueueMarker );
    DECLARE_FUNCTION( clEnqueueWaitForEvents );
    DECLARE_FUNCTION( clEnqueueBarrier );
    DECLARE_FUNCTION( clGetExtensionFunctionAddress );
    DECLARE_FUNCTION( clCreateFromGLBuffer );
    DECLARE_FUNCTION( clCreateFromGLTexture2D );
    DECLARE_FUNCTION( clCreateFromGLTexture3D );
    DECLARE_FUNCTION( clCreateFromGLRenderbuffer );
    DECLARE_FUNCTION( clGetGLObjectInfo );
    DECLARE_FUNCTION( clGetGLTextureInfo );
    DECLARE_FUNCTION( clEnqueueAcquireGLObjects );
    DECLARE_FUNCTION( clEnqueueReleaseGLObjects );
    void* clUnknow75; //Not Found
    void* clUnknow76; //Not Found
    void* clUnknow77; //Not Found
    void* clUnknow78; //Not Found
    void* clUnknow79; //Not Found
    void* clUnknow80; //Not Found
    void* clUnknow81; //Not Found
    DECLARE_FUNCTION( clSetEventCallback );
    DECLARE_FUNCTION( clCreateSubBuffer );
    DECLARE_FUNCTION( clSetMemObjectDestructorCallback );
    DECLARE_FUNCTION( clCreateUserEvent );
    DECLARE_FUNCTION( clSetUserEventStatus );
    DECLARE_FUNCTION( clEnqueueReadBufferRect );
    DECLARE_FUNCTION( clEnqueueWriteBufferRect );
    DECLARE_FUNCTION( clEnqueueCopyBufferRect );
    void* clUnknow90; //Not Found
    void* clUnknow91; //Not Found
    void* clUnknow92; //Not Found
    void* clUnknow93; //Not Found
    void* clUnknow94; //Not Found
    void* clUnknow95; //Not Found
    void* clUnknow96; //Not Found
    void* clUnknow97; //Not Found
    void* clUnknow98; //Not Found
    void* clUnknow99; //Not Found
};

#undef DECLARE_FUNCTION
//-----------------------------------------------------------------------------
class icd_object_manager
{
private:
    struct cl_object
    {
        icd_dispatch_table* table;
        uint32_t dcl_type;
        uint32_t reference_count;
        void* dcl_object;
    };

    typedef std::set< cl_object* > cl_object_set_t;
    typedef std::map< void*, cl_object* > dcl_object_map_t;

    static icd_object_manager* instance_ptr_;

    cl_object_set_t object_set_;
    dcl_object_map_t object_map_;
    icd_dispatch_table dispatch_table_;

#define FILL_DISPATCH_ITEM(x) dispatch_table_. x = x

    icd_object_manager()
    {
        FILL_DISPATCH_ITEM( clBuildProgram );
        //FILL_DISPATCH_ITEM( clCreateBuffer );
        FILL_DISPATCH_ITEM( clCreateCommandQueue );
        FILL_DISPATCH_ITEM( clCreateContext  );
        FILL_DISPATCH_ITEM( clCreateContextFromType );
        //FILL_DISPATCH_ITEM( clCreateFromGLBuffer );
        //FILL_DISPATCH_ITEM( clCreateFromGLRenderbuffer );
        //FILL_DISPATCH_ITEM( clCreateFromGLTexture2D );
        //FILL_DISPATCH_ITEM( clCreateFromGLTexture3D );
        //FILL_DISPATCH_ITEM( clCreateImage2D );
        //FILL_DISPATCH_ITEM( clCreateImage3D );
        FILL_DISPATCH_ITEM( clCreateKernel );
        //FILL_DISPATCH_ITEM( clCreateKernelsInProgram );
        //FILL_DISPATCH_ITEM( clCreateProgramWithBinary );
        FILL_DISPATCH_ITEM( clCreateProgramWithSource );
        //FILL_DISPATCH_ITEM( clCreateSampler );
        //FILL_DISPATCH_ITEM( clCreateSubBuffer );
        //FILL_DISPATCH_ITEM( clCreateUserEvent );
        //FILL_DISPATCH_ITEM( clEnqueueAcquireGLObjects );
        //FILL_DISPATCH_ITEM( clEnqueueBarrier );
        //FILL_DISPATCH_ITEM( clEnqueueCopyBuffer );
        //FILL_DISPATCH_ITEM( clEnqueueCopyBufferRect );
        //FILL_DISPATCH_ITEM( clEnqueueCopyBufferToImage );
        //FILL_DISPATCH_ITEM( clEnqueueCopyImage );
        //FILL_DISPATCH_ITEM( clEnqueueCopyImageToBuffer );
        //FILL_DISPATCH_ITEM( clEnqueueMapBuffer );
        //FILL_DISPATCH_ITEM( clEnqueueMapImage );
        //FILL_DISPATCH_ITEM( clEnqueueMarker );
        FILL_DISPATCH_ITEM( clEnqueueNDRangeKernel );
        //FILL_DISPATCH_ITEM( clEnqueueNativeKernel );
        //FILL_DISPATCH_ITEM( clEnqueueReadBuffer );
        //FILL_DISPATCH_ITEM( clEnqueueReadBufferRect );
        //FILL_DISPATCH_ITEM( clEnqueueReadImage );
        //FILL_DISPATCH_ITEM( clEnqueueReleaseGLObjects );
        //FILL_DISPATCH_ITEM( clEnqueueTask );
        //FILL_DISPATCH_ITEM( clEnqueueUnmapMemObject );
        //FILL_DISPATCH_ITEM( clEnqueueWaitForEvents );
        //FILL_DISPATCH_ITEM( clEnqueueWriteBuffer );
        //FILL_DISPATCH_ITEM( clEnqueueWriteBufferRect );
        //FILL_DISPATCH_ITEM( clEnqueueWriteImage );
        FILL_DISPATCH_ITEM( clFinish );
        //FILL_DISPATCH_ITEM( clFlush );
        //FILL_DISPATCH_ITEM( clGetCommandQueueInfo );
        FILL_DISPATCH_ITEM( clGetContextInfo );
        FILL_DISPATCH_ITEM( clGetDeviceIDs );
        FILL_DISPATCH_ITEM( clGetDeviceInfo );
        //FILL_DISPATCH_ITEM( clGetEventInfo );
        //FILL_DISPATCH_ITEM( clGetEventProfilingInfo );
        //FILL_DISPATCH_ITEM( clGetExtensionFunctionAddress );
        //FILL_DISPATCH_ITEM( clGetGLObjectInfo );
        //FILL_DISPATCH_ITEM( clGetGLTextureInfo );
        //FILL_DISPATCH_ITEM( clGetImageInfo );
        //FILL_DISPATCH_ITEM( clGetKernelInfo );
        //FILL_DISPATCH_ITEM( clGetKernelWorkGroupInfo );
        //FILL_DISPATCH_ITEM( clGetMemObjectInfo );
        FILL_DISPATCH_ITEM( clGetPlatformIDs );
        FILL_DISPATCH_ITEM( clGetPlatformInfo );
        FILL_DISPATCH_ITEM( clGetProgramBuildInfo );
        //FILL_DISPATCH_ITEM( clGetProgramInfo );
        //FILL_DISPATCH_ITEM( clGetSamplerInfo );
        //FILL_DISPATCH_ITEM( clGetSupportedImageFormats );
        FILL_DISPATCH_ITEM( clReleaseCommandQueue );
        FILL_DISPATCH_ITEM( clReleaseContext );
        //FILL_DISPATCH_ITEM( clReleaseEvent );
        FILL_DISPATCH_ITEM( clReleaseKernel );
        //FILL_DISPATCH_ITEM( clReleaseMemObject );
        FILL_DISPATCH_ITEM( clReleaseProgram );
        //FILL_DISPATCH_ITEM( clReleaseSampler );
        FILL_DISPATCH_ITEM( clRetainCommandQueue );
        FILL_DISPATCH_ITEM( clRetainContext );
        //FILL_DISPATCH_ITEM( clRetainEvent );
        FILL_DISPATCH_ITEM( clRetainKernel );
        //FILL_DISPATCH_ITEM( clRetainMemObject );
        FILL_DISPATCH_ITEM( clRetainProgram );
        //FILL_DISPATCH_ITEM( clRetainSampler );
        //FILL_DISPATCH_ITEM( clSetCommandQueueProperty );
        //FILL_DISPATCH_ITEM( clSetEventCallback );
        //FILL_DISPATCH_ITEM( clSetKernelArg );
        //FILL_DISPATCH_ITEM( clSetMemObjectDestructorCallback );
        //FILL_DISPATCH_ITEM( clSetUserEventStatus );
        //FILL_DISPATCH_ITEM( clUnloadCompiler );
        //FILL_DISPATCH_ITEM( clWaitForEvents );
    }

#undef FILL_DISPATCH_ITEM

    template< typename DCL_TYPE_T >
    cl_object_set_t::iterator get_internal_object( typename DCL_TYPE_T::cl_type_t cl_ptr )
    {
        if( cl_ptr == NULL )
        {
            throw dcl::library_exception( "Invalid object", DCL_TYPE_T::invalid_error_value );
        }

        cl_object_set_t::iterator it = object_set_.find( reinterpret_cast< cl_object* >( cl_ptr ) );

        if( it == object_set_.end() )
        {
            throw dcl::library_exception( "Unknow object", DCL_TYPE_T::invalid_error_value );
        }

        uint32_t type = (*it)->dcl_type;
        uint32_t type_id = DCL_TYPE_T::type_id;

        if( type != type_id )
        {
            throw dcl::library_exception( "Invalid object type", DCL_TYPE_T::invalid_error_value );
        }

        return it;
    }

public:
    static icd_object_manager& get_instance()
    {
        if( instance_ptr_ == NULL )
        {
            instance_ptr_ = new icd_object_manager();
        }
        return *instance_ptr_;
    }

    template< typename DCL_TYPE_T >
    void remove_object( typename DCL_TYPE_T::cl_type_t cl_ptr )
    {
        cl_object_set_t::iterator it = get_internal_object< DCL_TYPE_T >( cl_ptr );

        DCL_TYPE_T* obj_ptr = reinterpret_cast< DCL_TYPE_T* >( (*it)->dcl_object );

        object_map_.erase( (*it)->dcl_object );
        object_set_.erase( it );

        delete obj_ptr;
    }

    template< typename DCL_TYPE_T >
    inline DCL_TYPE_T* get_object_ptr( typename DCL_TYPE_T::cl_type_t cl_ptr )
    {
        cl_object* cl_object_ptr = *get_internal_object< DCL_TYPE_T >( cl_ptr );

        return reinterpret_cast< DCL_TYPE_T* >( cl_object_ptr->dcl_object );
    }

    template< typename DCL_TYPE_T >
    typename DCL_TYPE_T::cl_type_t get_cl_id( DCL_TYPE_T* cl_object_ptr )
    {
        dcl_object_map_t::iterator it = object_map_.find( reinterpret_cast< void* >( cl_object_ptr ) );

        if( it != object_map_.end() )
        {
            return reinterpret_cast< typename DCL_TYPE_T::cl_type_t >( it->second );
        }
        else
        {
            cl_object* newObj = new cl_object();
            newObj->table = &dispatch_table_;
            newObj->dcl_type = DCL_TYPE_T::type_id;
            newObj->dcl_object = reinterpret_cast< void* >( cl_object_ptr );
            newObj->reference_count = 1;

            object_map_.insert( dcl_object_map_t::value_type( reinterpret_cast< void* >( cl_object_ptr ), newObj ) );
            object_set_.insert( newObj );

            typename DCL_TYPE_T::cl_type_t cl_ret = reinterpret_cast< typename DCL_TYPE_T::cl_type_t >( newObj );

            cl_object_ptr->set_icd_obj( cl_ret );

            return( cl_ret );
        }
    }

    template< typename DCL_TYPE_T >
    inline void retain( typename DCL_TYPE_T::cl_type_t cl_ptr )
    {
        cl_object* cl_object_ptr = *get_internal_object< DCL_TYPE_T >( cl_ptr );

        cl_object_ptr->reference_count++;
    }

    template< typename DCL_TYPE_T >
    inline void release( typename DCL_TYPE_T::cl_type_t cl_ptr )
    {
        cl_object* cl_object_ptr = *get_internal_object< DCL_TYPE_T >( cl_ptr );

        cl_object_ptr->reference_count--;

        if( cl_object_ptr->reference_count == 0 )
        {
            object_set_.erase( cl_object_ptr );
            object_map_.erase( cl_object_ptr->dcl_object );

            DCL_TYPE_T* dcl_object_ptr = reinterpret_cast<DCL_TYPE_T*>( cl_object_ptr->dcl_object );

            delete dcl_object_ptr;
            delete cl_object_ptr;
        }
    }
};
//-----------------------------------------------------------------------------
}} // namespace dcl::icd
//-----------------------------------------------------------------------------
#endif // _DCL_ICD_OBJECT_MANAGER_H_
