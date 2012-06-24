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
#include "distributedcl_internal.h"
#include "cl_utils.h"
#include "icd/icd_object_manager.h"
#include "composite/composite_memory.h"
#include "composite/composite_context.h"
#include "composite/composite_command_queue.h"
#include "composite/composite_event.h"
using dcl::icd::icd_object_manager;
using dcl::info::generic_event;
using dcl::info::generic_image;
using dcl::info::generic_memory_object;
using dcl::composite::composite_memory;
using dcl::composite::composite_context;
using dcl::composite::composite_command_queue;
using dcl::composite::composite_event;
using dcl::composite::composite_image;
//-----------------------------------------------------------------------------
static bool check_flags( cl_mem_flags flags, void* host_ptr, cl_int* errcode_ret )
{
    if( flags & (CL_MEM_USE_HOST_PTR|CL_MEM_COPY_HOST_PTR) )
    {
        if( host_ptr == NULL )
        {
            if( errcode_ret != NULL )
            {
                *errcode_ret = CL_INVALID_HOST_PTR;
            }
            return false;
        }
    }
    else
    {
        if( host_ptr != NULL )
        {
            if( errcode_ret != NULL )
            {
                *errcode_ret = CL_INVALID_HOST_PTR;
            }
            return false;
        }
    }

    if( ((flags & (CL_MEM_READ_WRITE|CL_MEM_WRITE_ONLY|CL_MEM_READ_ONLY)) != CL_MEM_READ_WRITE) &&
        ((flags & (CL_MEM_READ_WRITE|CL_MEM_WRITE_ONLY|CL_MEM_READ_ONLY)) != CL_MEM_WRITE_ONLY) &&
        ((flags & (CL_MEM_READ_WRITE|CL_MEM_WRITE_ONLY|CL_MEM_READ_ONLY)) != CL_MEM_READ_ONLY) &&
        ((flags & (CL_MEM_READ_WRITE|CL_MEM_WRITE_ONLY|CL_MEM_READ_ONLY)) != 0) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return false;
    }

    if( (flags & CL_MEM_USE_HOST_PTR) &&
        (flags & (CL_MEM_ALLOC_HOST_PTR|CL_MEM_COPY_HOST_PTR)) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return false;
    }

    return true;
}
//-----------------------------------------------------------------------------
static bool check_image_format( const cl_image_format* image_format )
{
    return true;
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_mem CL_API_CALL
clCreateBuffer( cl_context context, cl_mem_flags flags, size_t size,
                void* host_ptr, cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_0
{
    if( size == 0 )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_BUFFER_SIZE;
        }
        return NULL;
    }

    if( !check_flags( flags, host_ptr, errcode_ret ) )
    {
        return NULL;
    }

    try
    {
        std::cerr << "create buffer: " << host_ptr
                  << " - size: " << size
                  << std::endl;

        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_context* context_ptr = icd.get_object_ptr< composite_context >( context );
        composite_memory* buffer_ptr =
            reinterpret_cast< composite_memory* >( context_ptr->create_buffer( host_ptr, size, flags ) );

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }
        
        return icd.get_cl_id< composite_memory >( buffer_ptr );
    }
    catch( dcl::library_exception& ex )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = ex.get_error();
        }
        return NULL;
    }
    catch( ... )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    // Dummy
    if( errcode_ret != NULL )
    {
        *errcode_ret = CL_INVALID_VALUE;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_mem CL_API_CALL
//clCreateSubBuffer( cl_mem buffer, cl_mem_flags flags,
//                   cl_buffer_create_type buffer_create_type,
//                   const void* buffer_create_info,
//                   cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_mem CL_API_CALL
clCreateImage2D( cl_context context, cl_mem_flags flags,
                 const cl_image_format* image_format, size_t image_width,
                 size_t image_height, size_t image_row_pitch, void* host_ptr,
                 cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_0
{
    if( !check_flags( flags, host_ptr, errcode_ret ) )
    {
        return NULL;
    }

    if( !check_image_format( image_format ) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_IMAGE_FORMAT_DESCRIPTOR;
        }
        return NULL;
    }

    if( (image_width == 0) || (image_height == 0) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_IMAGE_SIZE;
        }
        return NULL;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_context* context_ptr = icd.get_object_ptr< composite_context >( context );
        generic_image* image_ptr =
            context_ptr->create_image( host_ptr, flags, image_format, image_width,
                                       image_height, image_row_pitch );

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }

        return icd.get_cl_id< composite_image >( reinterpret_cast< composite_image*>( image_ptr ) );
    }
    catch( dcl::library_exception& ex )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = ex.get_error();
            std::cerr << "Exception!!: " << ex.get_error() << std::endl;
        }
        return NULL;
    }
    catch( ... )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    if( errcode_ret != NULL )
    {
        *errcode_ret = CL_INVALID_VALUE;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_mem CL_API_CALL
//clCreateImage3D( cl_context context, cl_mem_flags flags,
//                 const cl_image_format* image_format, size_t image_width,
//                 size_t image_height, size_t image_depth, size_t image_row_pitch,
//                 size_t image_slice_pitch, void* host_ptr,
//                 cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_0
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clRetainMemObject( cl_mem memobj ) CL_API_SUFFIX__VERSION_1_0
{
    return retain_object< generic_memory_object >( memobj );
}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clReleaseMemObject( cl_mem memobj ) CL_API_SUFFIX__VERSION_1_0
{
    return release_object< generic_memory_object >( memobj );
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clGetSupportedImageFormats( cl_context context, cl_mem_flags flags,
//                            cl_mem_object_type image_type, cl_uint num_entries,
//                            cl_image_format* image_formats,
//                            cl_uint* num_image_formats ) CL_API_SUFFIX__VERSION_1_0
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clGetMemObjectInfo( cl_mem memobj, cl_mem_info param_name,
                    size_t param_value_size, void* param_value,
                    size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
{
    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_memory* memory_ptr = reinterpret_cast< composite_memory* >(
            get_info_check_parameters< generic_memory_object >( memobj, param_value_size,
                                                                param_value, param_value_size_ret ) );

        size_t param_size = 0;

        if( param_value != NULL )
        {
            switch( param_name )
            {
                case CL_MEM_OFFSET:
                    param_size = sizeof(size_t);

                    if( param_value_size < param_size )
                    {
                        return CL_INVALID_VALUE;
                    }

                    *(reinterpret_cast<size_t*>( param_value )) = 0;
                    break;

                case CL_MEM_ASSOCIATED_MEMOBJECT:
                    param_size = sizeof(cl_mem);

                    if( param_value_size < param_size )
                    {
                        return CL_INVALID_VALUE;
                    }

                    *(reinterpret_cast<cl_mem*>( param_value )) = NULL;
                    break;

                case CL_MEM_CONTEXT:
                {
                    param_size = sizeof(cl_context);

                    if( param_value_size < param_size )
                    {
                        return CL_INVALID_VALUE;
                    }

                    composite_context* context_ptr =
                        const_cast<composite_context*>( &(memory_ptr->get_context()) );

                    *(reinterpret_cast<cl_context*>( param_value )) =
                        icd.get_cl_id< composite_context >( context_ptr );
                    break;
                }

                default:
                    get_info< generic_memory_object >( memobj, param_name, param_value_size,
                                                       param_value, param_value_size_ret );

                    return CL_SUCCESS;
            }
        }

        if( param_value_size_ret != NULL )
        {
            *param_value_size_ret = param_size;
        }
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    return CL_SUCCESS;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clGetImageInfo( cl_mem image, cl_image_info param_name,
//                size_t param_value_size, void* param_value,
//                size_t* param_value_size_ret ) CL_API_SUFFIX__VERSION_1_0
//{
//}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clSetMemObjectDestructorCallback( cl_mem memobj,
//                                  void (CL_CALLBACK* pfn_notify)(cl_mem,void*),
//                                  void* user_data ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clEnqueueReadBuffer( cl_command_queue command_queue, cl_mem buffer,
                     cl_bool blocking_read, size_t offset, size_t cb,
                     void* ptr, cl_uint num_events_in_wait_list,
                     const cl_event* event_wait_list, cl_event* event ) CL_API_SUFFIX__VERSION_1_0
{
    if( ptr == NULL )
    {
        return CL_INVALID_VALUE;
    }

    if( ((event_wait_list == NULL) && (num_events_in_wait_list != 0)) ||
        ((event_wait_list != NULL) && (num_events_in_wait_list == 0)) )
    {
        return CL_INVALID_EVENT_WAIT_LIST;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_command_queue* queue_ptr = icd.get_object_ptr< composite_command_queue >( command_queue );
        composite_memory* buffer_ptr = icd.get_object_ptr< composite_memory >( buffer );

        dcl::events_t events;
        load_events( events, num_events_in_wait_list, event_wait_list );

        composite_event* event_ptr = NULL;

        buffer_ptr->read( queue_ptr, ptr, cb, offset, blocking_read, events,
                          (event != NULL) ? reinterpret_cast<generic_event**>( &event_ptr )
                                          : NULL );

        if( event != NULL )
        {
            *event = icd.get_cl_id< composite_event >( event_ptr );
        }

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clEnqueueReadBufferRect( cl_command_queue command_queue, cl_mem buffer,
//                         cl_bool blocking_read, const size_t* buffer_origin,
//                         const size_t* host_origin, const size_t* region,
//                         size_t buffer_row_pitch, size_t buffer_slice_pitch,
//                         size_t host_row_pitch, size_t host_slice_pitch,
//                         void* ptr, cl_uint num_events_in_wait_list,
//                         const cl_event* event_wait_list, cl_event* event ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWriteBuffer( cl_command_queue command_queue, cl_mem buffer,
                      cl_bool blocking_write, size_t offset, size_t cb,
                      const void* ptr, cl_uint num_events_in_wait_list,
                      const cl_event* event_wait_list, cl_event* event ) CL_API_SUFFIX__VERSION_1_0
{
    if( ptr == NULL )
    {
        return CL_INVALID_VALUE;
    }

    if( ((event_wait_list == NULL) && (num_events_in_wait_list != 0)) ||
        ((event_wait_list != NULL) && (num_events_in_wait_list == 0)) )
    {
        return CL_INVALID_EVENT_WAIT_LIST;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_command_queue* queue_ptr = icd.get_object_ptr< composite_command_queue >( command_queue );
        composite_memory* buffer_ptr = icd.get_object_ptr< composite_memory >( buffer );

        dcl::events_t events;
        load_events( events, num_events_in_wait_list, event_wait_list );

        composite_event* event_ptr = NULL;

        buffer_ptr->write( queue_ptr, ptr, cb, offset, blocking_write, events,
                           (event != NULL) ? reinterpret_cast<generic_event**>( &event_ptr )
                                           : NULL );

        if( event != NULL )
        {
            *event = icd.get_cl_id< composite_event >( event_ptr );
        }

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clEnqueueWriteBufferRect( cl_command_queue command_queue, cl_mem buffer,
//                          cl_bool blocking_write, const size_t* buffer_origin,
//                          const size_t* host_origin, const size_t* region,
//                          size_t buffer_row_pitch, size_t buffer_slice_pitch,
//                          size_t host_row_pitch, size_t host_slice_pitch,
//                          const void* ptr, cl_uint num_events_in_wait_list,
//                          const cl_event* event_wait_list, cl_event* event ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyBuffer( cl_command_queue command_queue, cl_mem src_buffer,
                     cl_mem dst_buffer, size_t src_offset, size_t dst_offset,
                     size_t cb, cl_uint num_events_in_wait_list,
                     const cl_event* event_wait_list, cl_event* event ) CL_API_SUFFIX__VERSION_1_0
{
    if( ((event_wait_list == NULL) && (num_events_in_wait_list != 0)) ||
        ((event_wait_list != NULL) && (num_events_in_wait_list == 0)) )
    {
        return CL_INVALID_EVENT_WAIT_LIST;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_command_queue* queue_ptr = icd.get_object_ptr< composite_command_queue >( command_queue );
        composite_memory* src_buffer_ptr = icd.get_object_ptr< composite_memory >( src_buffer );
        composite_memory* dst_buffer_ptr = icd.get_object_ptr< composite_memory >( dst_buffer );

        dcl::events_t events;
        load_events( events, num_events_in_wait_list, event_wait_list );

        composite_event* event_ptr = NULL;

        dst_buffer_ptr->copy( queue_ptr, src_buffer_ptr, cb, src_offset, dst_offset, events,
                              (event != NULL) ? reinterpret_cast<generic_event**>( &event_ptr )
                                              : NULL );

        if( event != NULL )
        {
            *event = icd.get_cl_id< composite_event >( event_ptr );
        }

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clEnqueueCopyBufferRect( cl_command_queue command_queue, cl_mem src_buffer,
//                         cl_mem dst_buffer, const size_t* src_origin,
//                         const size_t* dst_origin, const size_t* region,
//                         size_t src_row_pitch, size_t src_slice_pitch,
//                         size_t dst_row_pitch, size_t dst_slice_pitch,
//                         cl_uint num_events_in_wait_list, const cl_event* event_wait_list,
//                         cl_event* event ) CL_API_SUFFIX__VERSION_1_1
//{
//}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clEnqueueReadImage( cl_command_queue command_queue, cl_mem image,
//                    cl_bool blocking_read, const size_t origin[3],
//                    const size_t region[3], size_t row_pitch,
//                    size_t slice_pitch, void* ptr, cl_uint num_events_in_wait_list,
//                    const cl_event* event_wait_list, cl_event* event ) CL_API_SUFFIX__VERSION_1_0
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWriteImage( cl_command_queue command_queue, cl_mem image,
                     cl_bool blocking_write, const size_t origin[3],
                     const size_t region[3], size_t input_row_pitch,
                     size_t input_slice_pitch, const void* ptr,
                     cl_uint num_events_in_wait_list, const cl_event* event_wait_list,
                     cl_event* event ) CL_API_SUFFIX__VERSION_1_0
{
    if( ptr == NULL )
    {
        return CL_INVALID_VALUE;
    }

    if( ((event_wait_list == NULL) && (num_events_in_wait_list != 0)) ||
        ((event_wait_list != NULL) && (num_events_in_wait_list == 0)) )
    {
        return CL_INVALID_EVENT_WAIT_LIST;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_command_queue* queue_ptr = icd.get_object_ptr< composite_command_queue >( command_queue );
        composite_image* image_ptr = icd.get_object_ptr< composite_image >( image );

        dcl::events_t events;
        load_events( events, num_events_in_wait_list, event_wait_list );

        composite_event* event_ptr = NULL;

        image_ptr->write( queue_ptr, ptr, origin, region, input_row_pitch, input_slice_pitch,
                          (blocking_write == CL_TRUE)? true : false, events,
                          (event != NULL) ? reinterpret_cast<generic_event**>( &event_ptr )
                                          : NULL );

        if( event != NULL )
        {
            *event = icd.get_cl_id< composite_event >( event_ptr );
        }

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clEnqueueCopyImage( cl_command_queue command_queue, cl_mem src_image,
//                    cl_mem dst_image, const size_t* src_origin[3],
//                    const size_t* dst_origin[3], const size_t* region[3],
//                    cl_uint num_events_in_wait_list, const cl_event* event_wait_list,
//                    cl_event* event ) CL_API_SUFFIX__VERSION_1_0
//{
//}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clEnqueueCopyImageToBuffer( cl_command_queue command_queue, cl_mem src_image,
//                            cl_mem dst_buffer, const size_t* src_origin[3],
//                            const size_t* region[3], size_t dst_offset,
//                            cl_uint num_events_in_wait_list, const cl_event* event_wait_list,
//                            cl_event* event ) CL_API_SUFFIX__VERSION_1_0
//{
//}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY cl_int CL_API_CALL
//clEnqueueCopyBufferToImage( cl_command_queue command_queue, cl_mem src_buffer,
//                            cl_mem dst_image, size_t src_offset, const size_t* dst_origin[3],
//                            const size_t* region[3], cl_uint num_events_in_wait_list,
//                            const cl_event* event_wait_list, cl_event* event ) CL_API_SUFFIX__VERSION_1_0
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY void * CL_API_CALL
clEnqueueMapBuffer( cl_command_queue command_queue, cl_mem buffer,
                    cl_bool blocking_map, cl_map_flags map_flags, size_t offset,
                    size_t cb, cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list, cl_event* event,
                    cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_0
{
    if( ((event_wait_list == NULL) && (num_events_in_wait_list != 0)) ||
        ((event_wait_list != NULL) && (num_events_in_wait_list == 0)) )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_EVENT_WAIT_LIST;
        }
        return NULL;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_command_queue* queue_ptr =
            icd.get_object_ptr< composite_command_queue >( command_queue );

        composite_memory* buffer_ptr =
            icd.get_object_ptr< composite_memory >( buffer );

        std::cerr << "map buffer: " << (void*)buffer
                  << " - size: " << cb
                  << std::endl;

        dcl::events_t events;
        load_events( events, num_events_in_wait_list, event_wait_list );

        composite_event* event_ptr = NULL;

        void* ret_ptr = buffer_ptr->map( queue_ptr, map_flags, cb, offset, blocking_map, events,
                                         (event != NULL) ? reinterpret_cast<generic_event**>( &event_ptr )
                                                         : NULL );

        if( event != NULL )
        {
            *event = icd.get_cl_id< composite_event >( event_ptr );
        }

        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_SUCCESS;
        }

        return ret_ptr;
    }
    catch( dcl::library_exception& ex )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = ex.get_error();
            std::cerr << "Exception!!: " << ex.get_error() << std::endl;
        }
        return NULL;
    }
    catch( ... )
    {
        if( errcode_ret != NULL )
        {
            *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
    }

    // Dummy
    if( errcode_ret != NULL )
    {
        *errcode_ret = CL_INVALID_VALUE;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
//extern "C" CL_API_ENTRY void * CL_API_CALL
//clEnqueueMapImage( cl_command_queue command_queue, cl_mem image,
//                   cl_bool blocking_map, cl_map_flags map_flags, const size_t* origin[3],
//                   const size_t* region[3], size_t* image_row_pitch, size_t* image_slice_pitch,
//                   cl_uint num_events_in_wait_list, const cl_event* event_wait_list,
//                   cl_event* event, cl_int* errcode_ret ) CL_API_SUFFIX__VERSION_1_0
//{
//}
//-----------------------------------------------------------------------------
extern "C" CL_API_ENTRY cl_int CL_API_CALL
clEnqueueUnmapMemObject( cl_command_queue command_queue, cl_mem memobj,
                         void* mapped_ptr, cl_uint num_events_in_wait_list,
                         const cl_event* event_wait_list, cl_event* event ) CL_API_SUFFIX__VERSION_1_0
{
    if( ((event_wait_list == NULL) && (num_events_in_wait_list != 0)) ||
        ((event_wait_list != NULL) && (num_events_in_wait_list == 0)) )
    {
        return CL_INVALID_EVENT_WAIT_LIST;
    }

    try
    {
        icd_object_manager& icd = icd_object_manager::get_instance();

        composite_command_queue* queue_ptr =
            icd.get_object_ptr< composite_command_queue >( command_queue );

        composite_memory* buffer_ptr =
            icd.get_object_ptr< composite_memory >( memobj );

        std::cerr << "unmap buffer: " << (void*)memobj
                  << " - ptr: " << mapped_ptr
                  << std::endl;

        dcl::events_t events;
        load_events( events, num_events_in_wait_list, event_wait_list );

        composite_event* event_ptr = NULL;

        buffer_ptr->unmap( queue_ptr, mapped_ptr, events,
                           (event != NULL) ? reinterpret_cast<generic_event**>( &event_ptr )
                                           : NULL );

        if( event != NULL )
        {
            *event = icd.get_cl_id< composite_event >( event_ptr );
        }

        return CL_SUCCESS;
    }
    catch( dcl::library_exception& ex )
    {
        return ex.get_error();
    }
    catch( ... )
    {
        return CL_INVALID_VALUE;
    }

    // Dummy
    return CL_INVALID_VALUE;
}
//-----------------------------------------------------------------------------
