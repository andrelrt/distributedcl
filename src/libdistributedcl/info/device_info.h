/*
 * Copyright (c) 2009-2011 André Tupinambá (andrelrt@gmail.com)
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
#ifndef _DCL_INFO_DEVICE_H_
#define _DCL_INFO_DEVICE_H_

#include <string>
#include "distributedcl_internal.h"
#include "library_exception.h"
#include "dcl_objects.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace info {
//-----------------------------------------------------------------------------
struct device_info
{
public:
    cl_device_type type_;                               // CL_DEVICE_TYPE
    cl_uint vendor_id_;                                 // CL_DEVICE_VENDOR_ID
    cl_uint max_compute_units_;                         // CL_DEVICE_MAX_COMPUTE_UNITS
    cl_uint max_work_item_dimensions_;                  // CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
    size_t max_work_item_sizes_[3];                     // CL_DEVICE_MAX_WORK_ITEM_SIZES
    size_t max_work_group_size_;                        // CL_DEVICE_MAX_WORK_GROUP_SIZE
    cl_uint preferred_vector_width_char_;               // CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR
    cl_uint preferred_vector_width_short_;              // CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT
    cl_uint preferred_vector_width_int_;                // CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT
    cl_uint preferred_vector_width_long_;               // CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG
    cl_uint preferred_vector_width_float_;              // CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT
    cl_uint preferred_vector_width_double_;             // CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE
    cl_uint preferred_vector_width_half_;               // CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF
    cl_uint native_vector_width_char_;                  // CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR
    cl_uint native_vector_width_short_;                 // CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT
    cl_uint native_vector_width_int_;                   // CL_DEVICE_NATIVE_VECTOR_WIDTH_INT
    cl_uint native_vector_width_long_;                  // CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG
    cl_uint native_vector_width_float_;                 // CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT
    cl_uint native_vector_width_double_;                // CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE
    cl_uint native_vector_width_half_;                  // CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF
    cl_uint max_clock_frequency_;                       // CL_DEVICE_MAX_CLOCK_FREQUENCY
    cl_uint address_bits_;                              // CL_DEVICE_ADDRESS_BITS
    cl_uint max_read_image_args_;                       // CL_DEVICE_MAX_READ_IMAGE_ARGS
    cl_uint max_write_image_args_;                      // CL_DEVICE_MAX_WRITE_IMAGE_ARGS
    cl_ulong max_mem_alloc_size_;                       // CL_DEVICE_MAX_MEM_ALLOC_SIZE
    cl_bool image_support_;                             // CL_DEVICE_IMAGE_SUPPORT
    size_t image2d_max_width_;                          // CL_DEVICE_IMAGE2D_MAX_WIDTH
    size_t image2d_max_height_;                         // CL_DEVICE_IMAGE2D_MAX_HEIGHT
    size_t image3d_max_width_;                          // CL_DEVICE_IMAGE3D_MAX_WIDTH
    size_t image3d_max_height_;                         // CL_DEVICE_IMAGE3D_MAX_HEIGHT
    size_t image3d_max_depth_;                          // CL_DEVICE_IMAGE3D_MAX_DEPTH
    size_t max_parameter_size_;                         // CL_DEVICE_MAX_PARAMETER_SIZE
    cl_uint max_samplers_;                              // CL_DEVICE_MAX_SAMPLERS
    cl_uint mem_base_address_align_;                    // CL_DEVICE_MEM_BASE_ADDR_ALIGN
    cl_uint min_data_type_align_size_;                  // CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE
    cl_device_fp_config single_fp_config_;              // CL_DEVICE_SINGLE_FP_CONFIG
    cl_device_mem_cache_type global_mem_cache_type_;    // CL_DEVICE_GLOBAL_MEM_CACHE_TYPE
    cl_uint global_mem_cacheline_size_;                 // CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE
    cl_ulong global_mem_cache_size_;                    // CL_DEVICE_GLOBAL_MEM_CACHE_SIZE
    cl_ulong global_mem_size_;                          // CL_DEVICE_GLOBAL_MEM_SIZE
    cl_ulong max_constant_buffer_size_;                 // CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
    cl_uint max_constant_args_;                         // CL_DEVICE_MAX_CONSTANT_ARGS
    cl_device_local_mem_type local_mem_type_;           // CL_DEVICE_LOCAL_MEM_TYPE
    cl_ulong local_mem_size_;                           // CL_DEVICE_LOCAL_MEM_SIZE
    cl_bool error_correction_support_;                  // CL_DEVICE_ERROR_CORRECTION_SUPPORT
    cl_bool host_unified_memory_;                       // CL_DEVICE_HOST_UNIFIED_MEMORY
    size_t profiling_timer_resolution_;                 // CL_DEVICE_PROFILING_TIMER_RESOLUTION
    cl_bool endian_little_;                             // CL_DEVICE_ENDIAN_LITTLE
    cl_bool avaiable_;                                  // CL_DEVICE_AVAILABLE
    cl_bool compiler_avaiable_;                         // CL_DEVICE_COMPILER_AVAILABLE
    cl_device_exec_capabilities execution_capabilities_;// CL_DEVICE_EXECUTION_CAPABILITIES
    cl_command_queue_properties queue_properties_;      // CL_DEVICE_QUEUE_PROPERTIES
    std::string name_;                                  // CL_DEVICE_NAME
    std::string vendor_;                                // CL_DEVICE_VENDOR
    std::string driver_version_;                        // CL_DRIVER_VERSION
    std::string profile_;                               // CL_DEVICE_PROFILE
    std::string version_;                               // CL_DEVICE_VERSION
    std::string opencl_c_version_;                      // CL_DEVICE_OPENCL_C_VERSION
    std::string extensions_;                            // CL_DEVICE_EXTENSIONS

    device_info() : max_work_item_dimensions_( 3 ){}

    inline bool operator< ( const device_info& other ) const
    {
        return name_ < other.name_;
    }

    inline size_t get_info_size( cl_device_info info ) const
    {
        switch( info )
        {
            case CL_DEVICE_TYPE                         : return sizeof( cl_device_type );
            case CL_DEVICE_VENDOR_ID                    : return sizeof( cl_uint );
            case CL_DEVICE_MAX_COMPUTE_UNITS            : return sizeof( cl_uint );
            case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS     : return sizeof( cl_uint );
            case CL_DEVICE_MAX_WORK_GROUP_SIZE          : return sizeof( size_t );
            case CL_DEVICE_MAX_WORK_ITEM_SIZES          : return sizeof( size_t ) * 3;
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR  : return sizeof( cl_uint );
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT : return sizeof( cl_uint );
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT   : return sizeof( cl_uint );
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG  : return sizeof( cl_uint );
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT : return sizeof( cl_uint );
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE: return sizeof( cl_uint );
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF  : return sizeof( cl_uint );
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR     : return sizeof( cl_uint );
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT    : return sizeof( cl_uint );
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_INT      : return sizeof( cl_uint );
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG     : return sizeof( cl_uint );
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT    : return sizeof( cl_uint );
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE   : return sizeof( cl_uint );
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF     : return sizeof( cl_uint );
            case CL_DEVICE_MAX_CLOCK_FREQUENCY          : return sizeof( cl_uint );
            case CL_DEVICE_ADDRESS_BITS                 : return sizeof( cl_uint );
            case CL_DEVICE_MAX_READ_IMAGE_ARGS          : return sizeof( cl_uint );
            case CL_DEVICE_MAX_WRITE_IMAGE_ARGS         : return sizeof( cl_uint );
            case CL_DEVICE_MAX_MEM_ALLOC_SIZE           : return sizeof( cl_ulong );
            case CL_DEVICE_IMAGE_SUPPORT                : return sizeof( cl_bool );
            case CL_DEVICE_IMAGE2D_MAX_WIDTH            : return sizeof( size_t );
            case CL_DEVICE_IMAGE2D_MAX_HEIGHT           : return sizeof( size_t );
            case CL_DEVICE_IMAGE3D_MAX_WIDTH            : return sizeof( size_t );
            case CL_DEVICE_IMAGE3D_MAX_HEIGHT           : return sizeof( size_t );
            case CL_DEVICE_IMAGE3D_MAX_DEPTH            : return sizeof( size_t );
            case CL_DEVICE_MAX_PARAMETER_SIZE           : return sizeof( size_t );
            case CL_DEVICE_MAX_SAMPLERS                 : return sizeof( cl_uint );
            case CL_DEVICE_MEM_BASE_ADDR_ALIGN          : return sizeof( cl_uint );
            case CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE     : return sizeof( cl_uint );
            case CL_DEVICE_SINGLE_FP_CONFIG             : return sizeof( cl_device_fp_config );
            case CL_DEVICE_GLOBAL_MEM_CACHE_TYPE        : return sizeof( cl_device_mem_cache_type );
            case CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE    : return sizeof( cl_uint );
            case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE        : return sizeof( cl_ulong );
            case CL_DEVICE_GLOBAL_MEM_SIZE              : return sizeof( cl_ulong );
            case CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE     : return sizeof( cl_ulong );
            case CL_DEVICE_MAX_CONSTANT_ARGS            : return sizeof( cl_uint );
            case CL_DEVICE_LOCAL_MEM_TYPE               : return sizeof( cl_device_local_mem_type );
            case CL_DEVICE_LOCAL_MEM_SIZE               : return sizeof( cl_ulong );
            case CL_DEVICE_ERROR_CORRECTION_SUPPORT     : return sizeof( cl_bool );
            case CL_DEVICE_HOST_UNIFIED_MEMORY          : return sizeof( cl_bool );
            case CL_DEVICE_PROFILING_TIMER_RESOLUTION   : return sizeof( size_t );
            case CL_DEVICE_ENDIAN_LITTLE                : return sizeof( cl_bool );
            case CL_DEVICE_AVAILABLE                    : return sizeof( cl_bool );
            case CL_DEVICE_COMPILER_AVAILABLE           : return sizeof( cl_bool );
            case CL_DEVICE_EXECUTION_CAPABILITIES       : return sizeof( cl_device_exec_capabilities );
            case CL_DEVICE_QUEUE_PROPERTIES             : return sizeof( cl_command_queue_properties );
            case CL_DEVICE_NAME                         : return name_.length() + 1;
            case CL_DEVICE_VENDOR                       : return vendor_.length() + 1;
            case CL_DRIVER_VERSION                      : return driver_version_.length() + 1;
            case CL_DEVICE_PROFILE                      : return profile_.length() + 1;
            case CL_DEVICE_VERSION                      : return version_.length() + 1;
            case CL_DEVICE_OPENCL_C_VERSION             : return opencl_c_version_.length() + 1;
            case CL_DEVICE_EXTENSIONS                   : return extensions_.length() + 1;

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }

    inline const void* get_info_pointer( cl_device_info info ) const
    {
        switch( info )
        {
            case CL_DEVICE_TYPE                         : return &type_;
            case CL_DEVICE_VENDOR_ID                    : return &vendor_id_;
            case CL_DEVICE_MAX_COMPUTE_UNITS            : return &max_compute_units_;
            case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS     : return &max_work_item_dimensions_;
            case CL_DEVICE_MAX_WORK_ITEM_SIZES          : return max_work_item_sizes_;
            case CL_DEVICE_MAX_WORK_GROUP_SIZE          : return &max_work_group_size_;
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR  : return &preferred_vector_width_char_;
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT : return &preferred_vector_width_short_;
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT   : return &preferred_vector_width_int_;
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG  : return &preferred_vector_width_long_;
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT : return &preferred_vector_width_float_;
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE: return &preferred_vector_width_double_;
            case CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF  : return &preferred_vector_width_half_;
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR     : return &native_vector_width_char_;
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT    : return &native_vector_width_short_;
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_INT      : return &native_vector_width_int_;
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG     : return &native_vector_width_long_;
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT    : return &native_vector_width_float_;
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE   : return &native_vector_width_double_;
            case CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF     : return &native_vector_width_half_;
            case CL_DEVICE_MAX_CLOCK_FREQUENCY          : return &max_clock_frequency_;
            case CL_DEVICE_ADDRESS_BITS                 : return &address_bits_;
            case CL_DEVICE_MAX_READ_IMAGE_ARGS          : return &max_read_image_args_;
            case CL_DEVICE_MAX_WRITE_IMAGE_ARGS         : return &max_write_image_args_;
            case CL_DEVICE_MAX_MEM_ALLOC_SIZE           : return &max_mem_alloc_size_;
            case CL_DEVICE_IMAGE_SUPPORT                : return &image_support_;
            case CL_DEVICE_IMAGE2D_MAX_WIDTH            : return &image2d_max_width_;
            case CL_DEVICE_IMAGE2D_MAX_HEIGHT           : return &image2d_max_height_;
            case CL_DEVICE_IMAGE3D_MAX_WIDTH            : return &image3d_max_width_;
            case CL_DEVICE_IMAGE3D_MAX_HEIGHT           : return &image3d_max_height_;
            case CL_DEVICE_IMAGE3D_MAX_DEPTH            : return &image3d_max_depth_;
            case CL_DEVICE_MAX_PARAMETER_SIZE           : return &max_parameter_size_;
            case CL_DEVICE_MAX_SAMPLERS                 : return &max_samplers_;
            case CL_DEVICE_MEM_BASE_ADDR_ALIGN          : return &mem_base_address_align_;
            case CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE     : return &min_data_type_align_size_;
            case CL_DEVICE_SINGLE_FP_CONFIG             : return &single_fp_config_;
            case CL_DEVICE_GLOBAL_MEM_CACHE_TYPE        : return &global_mem_cache_type_;
            case CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE    : return &global_mem_cacheline_size_;
            case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE        : return &global_mem_cache_size_;
            case CL_DEVICE_GLOBAL_MEM_SIZE              : return &global_mem_size_;
            case CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE     : return &max_constant_buffer_size_;
            case CL_DEVICE_MAX_CONSTANT_ARGS            : return &max_constant_args_;
            case CL_DEVICE_LOCAL_MEM_TYPE               : return &local_mem_type_;
            case CL_DEVICE_LOCAL_MEM_SIZE               : return &local_mem_size_;
            case CL_DEVICE_ERROR_CORRECTION_SUPPORT     : return &error_correction_support_;
            case CL_DEVICE_HOST_UNIFIED_MEMORY          : return &host_unified_memory_;
            case CL_DEVICE_PROFILING_TIMER_RESOLUTION   : return &profiling_timer_resolution_;
            case CL_DEVICE_ENDIAN_LITTLE                : return &endian_little_;
            case CL_DEVICE_AVAILABLE                    : return &avaiable_;
            case CL_DEVICE_COMPILER_AVAILABLE           : return &compiler_avaiable_;
            case CL_DEVICE_EXECUTION_CAPABILITIES       : return &execution_capabilities_;
            case CL_DEVICE_QUEUE_PROPERTIES             : return &queue_properties_;
            case CL_DEVICE_NAME                         : return name_.c_str();
            case CL_DEVICE_VENDOR                       : return vendor_.c_str();
            case CL_DRIVER_VERSION                      : return driver_version_.c_str();
            case CL_DEVICE_PROFILE                      : return profile_.c_str();
            case CL_DEVICE_VERSION                      : return version_.c_str();
            case CL_DEVICE_OPENCL_C_VERSION             : return opencl_c_version_.c_str();
            case CL_DEVICE_EXTENSIONS                   : return extensions_.c_str();

            default:
                throw library_exception( CL_INVALID_VALUE );
        }
    }
};
//-----------------------------------------------------------------------------
class generic_device :
    public cl_object< cl_device_id, cl_device_info, CL_INVALID_DEVICE >,
    public dcl_object< device_info >
{
public:
    generic_device( const generic_platform* platform_ptr, cl_device_type type = 0 ) : 
        info_loaded_( false ), platform_ptr_( platform_ptr )
    {
        local_info_.type_ = type;
    }

    ~generic_device(){}

    inline void load_info()
    {
        if( !info_loaded_ )
        {
            info_loaded_ = load_device_info();
        }
    }

    inline cl_device_type get_type() const
    {
        return local_info_.type_;
    }

    inline const generic_platform* get_platform() const
    {
        return platform_ptr_;
    }

protected:
    virtual bool load_device_info() = 0;

    inline bool get_info_loaded() const
    {
        return info_loaded_;
    }

private:
    bool info_loaded_;
    const generic_platform* platform_ptr_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::info
//-----------------------------------------------------------------------------
#endif // _DCL_INFO_DEVICE_H_
