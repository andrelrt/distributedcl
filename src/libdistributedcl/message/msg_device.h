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
#ifndef _DCL_DEVICE_MESSAGES_H_
#define _DCL_DEVICE_MESSAGES_H_

#include "message.h"
#include "info/device_info.h"
#include "remote/remote_device.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgGetDeviceIDs > : public base_message
{
public:
    dcl_message< msgGetDeviceIDs >() : 
        base_message( msgGetDeviceIDs, true, 0, sizeof( msgGetDeviceIDs_response ) ) {}

    virtual void parse_response( const base_message* response_ptr );

    inline std::size_t get_device_count()
    {
        return get_cpu_count() + get_gpu_count() + get_accelerator_count() + get_other_count();
    }

    inline std::size_t get_cpu_count()
    {
        return cpu_devices_.size();
    }

    inline std::size_t get_gpu_count()
    {
        return gpu_devices_.size();
    }

    inline std::size_t get_accelerator_count()
    {
        return accelerator_devices_.size();
    }

    inline std::size_t get_other_count()
    {
        return other_devices_.size();
    }

    inline void add_cpu_device( dcl::remote_id_t dev_id )
    {
        cpu_devices_.push_back( dev_id );
    }

    inline void add_gpu_device( dcl::remote_id_t dev_id )
    {
        gpu_devices_.push_back( dev_id );
    }

    inline void add_accelerator_device( dcl::remote_id_t dev_id )
    {
        accelerator_devices_.push_back( dev_id );
    }

    inline void add_other_device( dcl::remote_id_t dev_id )
    {
        other_devices_.push_back( dev_id );
    }

    inline void update_response_size()
    {
        set_response_size( sizeof( msgGetDeviceIDs_response ) - sizeof( dcl::remote_id_t ) +
                           sizeof( dcl::remote_id_t ) * get_device_count() );
    }

protected:
    virtual void create_response( uint8_t* payload_ptr );

private:
    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgGetDeviceIDs_response
    {
        uint16_t cpu_count;
        uint16_t gpu_count;
        uint16_t accelerator_count;
        uint16_t other_count;
        dcl::remote_id_t device_ids[1];
    };
    #pragma pack( pop )

    typedef std::vector< dcl::remote_id_t > remote_ids_t;
    remote_ids_t cpu_devices_;
    remote_ids_t gpu_devices_;
    remote_ids_t accelerator_devices_;
    remote_ids_t other_devices_;

};
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgGetDeviceInfo > : public base_message
{
public:
    dcl_message< msgGetDeviceInfo >() : 
        base_message( msgGetDeviceInfo, true, sizeof(dcl::remote_id_t) ), id_( 0xffff )
    {}

    dcl_message< msgGetDeviceInfo >( remote_id_t id ) : 
        base_message( msgGetDeviceInfo, true, sizeof(dcl::remote_id_t) ), id_( id )
    {}

    virtual void parse_response( const base_message* response_ptr );

    const dcl::remote_id_t get_remote_id() const
    {
        return id_;
    }

    const dcl::info::device_info& get_info() const
    {
        return device_info_;
    }

    void set_info( const dcl::info::device_info& info )
    {
        device_info_ = info;

        set_response_size( sizeof(msgGetDeviceInfo_response) - 1 + 
                           device_info_.name_.length() +  
                           device_info_.vendor_.length() +  
                           device_info_.driver_version_.length() +  
                           device_info_.version_.length() +  
                           device_info_.opencl_c_version_.length() +  
                           device_info_.extensions_.length() );
    }

protected:
    virtual void create_request( uint8_t* payload_ptr );
    virtual void create_response( uint8_t* payload_ptr );
    virtual void parse_request( const uint8_t* payload_ptr );

private:
    #pragma pack( push, 1 )
    // Better when aligned in 32 bits boundary
    struct msgGetDeviceInfo_response
    {
        uint32_t vendor_id_;                        // CL_DEVICE_VENDOR_ID
        uint32_t max_compute_units_;                // CL_DEVICE_MAX_COMPUTE_UNITS
        //uint32_t max_work_item_dimensions_;       // CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS DistributedCL supports only 3 dimensions
        uint32_t max_work_item_sizes_[3];           // CL_DEVICE_MAX_WORK_ITEM_SIZES
        uint32_t max_work_group_size_;              // CL_DEVICE_MAX_WORK_GROUP_SIZE

        // Vector widths are only 0, 1, 2, 3, 4, 8 or 16
        uint32_t preferred_vector_width_char_:3;    // CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR
        uint32_t preferred_vector_width_short_:3;   // CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT
        uint32_t preferred_vector_width_int_:3;     // CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT
        uint32_t preferred_vector_width_long_:3;    // CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG
        uint32_t preferred_vector_width_float_:3;   // CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT
        uint32_t preferred_vector_width_double_:3;  // CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE
        uint32_t preferred_vector_width_half_:3;    // CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF
        uint32_t native_vector_width_char_:3;       // CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR
        uint32_t native_vector_width_short_:3;      // CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT
        uint32_t native_vector_width_int_:3;        // CL_DEVICE_NATIVE_VECTOR_WIDTH_INT
        uint32_t image_support_:1;                  // CL_DEVICE_IMAGE_SUPPORT
        uint32_t error_correction_support_:1;       // CL_DEVICE_ERROR_CORRECTION_SUPPORT

        uint32_t native_vector_width_long_:3;       // CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG
        uint32_t native_vector_width_float_:3;      // CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT
        uint32_t native_vector_width_double_:3;     // CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE
        uint32_t native_vector_width_half_:3;       // CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF
        uint32_t endian_little_:1;                  // CL_DEVICE_ENDIAN_LITTLE
        uint32_t avaiable_:1;                       // CL_DEVICE_AVAILABLE
        uint32_t address_bits_:1;                   // CL_DEVICE_ADDRESS_BITS: 32 or 64 (0 or 1)
        uint32_t single_fp_config_:5;               // CL_DEVICE_SINGLE_FP_CONFIG
        uint32_t global_mem_cache_type_:2;          // CL_DEVICE_GLOBAL_MEM_CACHE_TYPE
        uint32_t local_mem_type_:1;                 // CL_DEVICE_LOCAL_MEM_TYPE: CL_LOCAL or CL_GLOBAL (0 or 1)
        uint32_t queue_properties_:1;               // CL_DEVICE_QUEUE_PROPERTIES: out of order execution support (true or false)
        uint32_t type_:2;                           // CL_DEVICE_TYPE: 0-CPU, 1-GPU, 2-ACCELERATOR
        uint32_t reserved_bits_:6;

        //uint32_t host_unified_memory_:1;          // CL_DEVICE_HOST_UNIFIED_MEMORY always false in DistributedCL
        //uint32_t compiler_avaiable_:1;            // CL_DEVICE_COMPILER_AVAILABLE always TRUE in FULL_PROFILE
        //uint32_t execution_capabilities_:1;       // CL_DEVICE_EXECUTION_CAPABILITIES always CL_EXEC_KERNEL only for DistributedCL

        uint32_t max_clock_frequency_;              // CL_DEVICE_MAX_CLOCK_FREQUENCY
        uint32_t max_read_image_args_;              // CL_DEVICE_MAX_READ_IMAGE_ARGS
        uint32_t max_write_image_args_;             // CL_DEVICE_MAX_WRITE_IMAGE_ARGS
        uint64_t max_mem_alloc_size_;               // CL_DEVICE_MAX_MEM_ALLOC_SIZE
        uint32_t image2d_max_width_;                // CL_DEVICE_IMAGE2D_MAX_WIDTH
        uint32_t image2d_max_height_;               // CL_DEVICE_IMAGE2D_MAX_HEIGHT
        uint32_t image3d_max_width_;                // CL_DEVICE_IMAGE3D_MAX_WIDTH
        uint32_t image3d_max_height_;               // CL_DEVICE_IMAGE3D_MAX_HEIGHT
        uint32_t image3d_max_depth_;                // CL_DEVICE_IMAGE3D_MAX_DEPTH
        uint32_t max_parameter_size_;               // CL_DEVICE_MAX_PARAMETER_SIZE
        uint32_t max_samplers_;                     // CL_DEVICE_MAX_SAMPLERS
        uint32_t mem_base_address_align_;           // CL_DEVICE_MEM_BASE_ADDR_ALIGN
        uint32_t min_data_type_align_size_;         // CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE
        uint32_t global_mem_cacheline_size_;        // CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE
        uint64_t global_mem_cache_size_;            // CL_DEVICE_GLOBAL_MEM_CACHE_SIZE
        uint64_t global_mem_size_;                  // CL_DEVICE_GLOBAL_MEM_SIZE
        uint64_t max_constant_buffer_size_;         // CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
        uint32_t max_constant_args_;                // CL_DEVICE_MAX_CONSTANT_ARGS
        uint64_t local_mem_size_;                   // CL_DEVICE_LOCAL_MEM_SIZE
        uint32_t profiling_timer_resolution_;       // CL_DEVICE_PROFILING_TIMER_RESOLUTION

        uint32_t name_len_;                         // CL_DEVICE_NAME
        uint32_t vendor_len_;                       // CL_DEVICE_VENDOR
        uint32_t driver_version_len_;               // CL_DRIVER_VERSION
        //uint32_t profile_len_;                    // CL_DEVICE_PROFILE DistributedCL supports only FULL_PROFILE devices
        uint32_t version_len_;                      // CL_DEVICE_VERSION
        uint32_t opencl_c_version_len_;             // CL_DEVICE_OPENCL_C_VERSION
        uint32_t extensions_len_;                   // CL_DEVICE_EXTENSIONS

        uint8_t string_buffer_[1];                  // Dummy var to strings address
    };
    #pragma pack( pop )

    dcl::remote_id_t id_;
    dcl::info::device_info device_info_;
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_DEVICE_MESSAGES_H_
