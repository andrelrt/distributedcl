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
#include <boost/scoped_array.hpp>
#include "device.h"
#include "platform.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
device::device( const platform* platform_ptr, cl_device_id id )
    : generic_device( platform_ptr ), opencl_object( platform_ptr->get_opencl(), id ) 
{
    load_info();
}
//-----------------------------------------------------------------------------
bool device::load_device_info()
{
    load_info_data( CL_DEVICE_TYPE                          );
    load_info_data( CL_DEVICE_VENDOR_ID                     );
    load_info_data( CL_DEVICE_MAX_COMPUTE_UNITS             );
    load_info_data( CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS      );
    load_info_data( CL_DEVICE_MAX_WORK_ITEM_SIZES           );
    load_info_data( CL_DEVICE_MAX_WORK_GROUP_SIZE           );
    load_info_data( CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR   );
    load_info_data( CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT  );
    load_info_data( CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT    );
    load_info_data( CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG   );
    load_info_data( CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT  );
    load_info_data( CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE );
    load_info_data( CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF   );
    load_info_data( CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR      );
    load_info_data( CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT     );
    load_info_data( CL_DEVICE_NATIVE_VECTOR_WIDTH_INT       );
    load_info_data( CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG      );
    load_info_data( CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT     );
    load_info_data( CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE    );
    load_info_data( CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF      );
    load_info_data( CL_DEVICE_MAX_CLOCK_FREQUENCY           );
    load_info_data( CL_DEVICE_ADDRESS_BITS                  );
    load_info_data( CL_DEVICE_MAX_READ_IMAGE_ARGS           );
    load_info_data( CL_DEVICE_MAX_WRITE_IMAGE_ARGS          );
    load_info_data( CL_DEVICE_MAX_MEM_ALLOC_SIZE            );
    load_info_data( CL_DEVICE_IMAGE_SUPPORT                 );
    load_info_data( CL_DEVICE_IMAGE2D_MAX_WIDTH             );
    load_info_data( CL_DEVICE_IMAGE2D_MAX_HEIGHT            );
    load_info_data( CL_DEVICE_IMAGE3D_MAX_WIDTH             );
    load_info_data( CL_DEVICE_IMAGE3D_MAX_HEIGHT            );
    load_info_data( CL_DEVICE_IMAGE3D_MAX_DEPTH             );
    load_info_data( CL_DEVICE_MAX_PARAMETER_SIZE            );
    load_info_data( CL_DEVICE_MAX_SAMPLERS                  );
    load_info_data( CL_DEVICE_MEM_BASE_ADDR_ALIGN           );
    load_info_data( CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE      );
    load_info_data( CL_DEVICE_SINGLE_FP_CONFIG              );
    load_info_data( CL_DEVICE_GLOBAL_MEM_CACHE_TYPE         );
    load_info_data( CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE     );
    load_info_data( CL_DEVICE_GLOBAL_MEM_CACHE_SIZE         );
    load_info_data( CL_DEVICE_GLOBAL_MEM_SIZE               );
    load_info_data( CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE      );
    load_info_data( CL_DEVICE_MAX_CONSTANT_ARGS             );
    load_info_data( CL_DEVICE_LOCAL_MEM_TYPE                );
    load_info_data( CL_DEVICE_LOCAL_MEM_SIZE                );
    load_info_data( CL_DEVICE_ERROR_CORRECTION_SUPPORT      );
    load_info_data( CL_DEVICE_HOST_UNIFIED_MEMORY           );
    load_info_data( CL_DEVICE_PROFILING_TIMER_RESOLUTION    );
    load_info_data( CL_DEVICE_ENDIAN_LITTLE                 );
    load_info_data( CL_DEVICE_AVAILABLE                     );
    load_info_data( CL_DEVICE_COMPILER_AVAILABLE            );
    load_info_data( CL_DEVICE_EXECUTION_CAPABILITIES        );
    load_info_data( CL_DEVICE_QUEUE_PROPERTIES              );
             
    load_info_string( CL_DEVICE_NAME, local_info_.name_ );
    load_info_string( CL_DEVICE_VENDOR, local_info_.vendor_ );
    load_info_string( CL_DRIVER_VERSION, local_info_.driver_version_ );
    load_info_string( CL_DEVICE_PROFILE, local_info_.profile_ );
    load_info_string( CL_DEVICE_VERSION, local_info_.version_ );
    load_info_string( CL_DEVICE_OPENCL_C_VERSION, local_info_.opencl_c_version_ );
    load_info_string( CL_DEVICE_EXTENSIONS, local_info_.extensions_ );

    return true;
}
//-----------------------------------------------------------------------------
void device::load_info_data( cl_device_info info )
{
    cl_int cl_error = opencl_.clGetDeviceInfo( id_, info, local_info_.get_info_size( info ), 
                                               const_cast<void*>( local_info_.get_info_pointer( info ) ), NULL );

    if( cl_error != CL_SUCCESS )
        throw dcl::library_exception( cl_error );
}
//-----------------------------------------------------------------------------
void device::load_info_string( cl_device_info info, std::string& str )
{
    size_t value_size;
    cl_int cl_error = opencl_.clGetDeviceInfo( id_, info, 0, NULL, &value_size );

    if( cl_error == CL_SUCCESS )
    {
        boost::scoped_array<char> param_value( new char[ value_size +1 ] );

        cl_error = opencl_.clGetDeviceInfo( id_, info, value_size, param_value.get(), &value_size );

        if( cl_error == CL_SUCCESS )
        {
            param_value[ value_size ] = 0;
            str.assign( param_value.get() );
        }
    }

    if( cl_error != CL_SUCCESS )
        throw dcl::library_exception( cl_error );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
