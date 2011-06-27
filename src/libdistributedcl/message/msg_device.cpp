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
#include "msg_device.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
void dcl_message< msgGetDeviceIDs >::set_response( const base_message* response_ptr )
{
    const dcl_message< msgGetDeviceIDs >* msg_response_ptr = reinterpret_cast< const dcl_message< msgGetDeviceIDs >* >( response_ptr );
    const msgGetDeviceIDs_response* response = reinterpret_cast< const msgGetDeviceIDs_response* >( msg_response_ptr ->get_payload() );

    cpu_count_ = ntohs( response->cpu_count );
    gpu_count_ = ntohs( response->gpu_count );
    accelerator_count_ = ntohs( response->accelerator_count );
    other_count_ = ntohs( response->other_count );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetDeviceIDs >::create_response( uint8_t* payload_ptr )
{
    msgGetDeviceIDs_response* response = reinterpret_cast< msgGetDeviceIDs_response* >( payload_ptr );

    response->cpu_count = htons( static_cast< uint16_t >( cpu_count_ ) );
    response->gpu_count = htons( static_cast< uint16_t >( gpu_count_ ) );
    response->accelerator_count  = htons( static_cast< uint16_t >( accelerator_count_ ) );
    response->other_count = htons( static_cast< uint16_t >( other_count_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetDeviceInfo >::set_response( const base_message* response_ptr )
{
    cl_uint vector_sizes[] = { 0, 1, 2, 3, 4, 8, 16, 0 };
    const dcl_message< msgGetDeviceInfo >* msg_response_ptr = reinterpret_cast< const dcl_message< msgGetDeviceInfo >* >( response_ptr );
    const msgGetDeviceInfo_response* response = reinterpret_cast< const msgGetDeviceInfo_response* >( msg_response_ptr ->get_payload() );

    device_info_.vendor_id_ = ntohl( response->vendor_id_ );
    device_info_.max_compute_units_ = ntohl( response->max_compute_units_ );
    device_info_.max_work_item_dimensions_ = 3;
    device_info_.max_work_item_sizes_[0] = ntohl( response->max_compute_units_ );
    device_info_.max_work_item_sizes_[1] = ntohl( response->max_compute_units_ );
    device_info_.max_work_item_sizes_[2] = ntohl( response->max_compute_units_ );
    device_info_.max_work_group_size_ = ntohl( response->max_work_group_size_ );
    device_info_.preferred_vector_width_char_   = vector_sizes[ response->preferred_vector_width_char_   ];
    device_info_.preferred_vector_width_short_  = vector_sizes[ response->preferred_vector_width_short_  ];
    device_info_.preferred_vector_width_int_    = vector_sizes[ response->preferred_vector_width_int_    ];
    device_info_.preferred_vector_width_long_   = vector_sizes[ response->preferred_vector_width_long_   ];
    device_info_.preferred_vector_width_float_  = vector_sizes[ response->preferred_vector_width_float_  ];
    device_info_.preferred_vector_width_double_ = vector_sizes[ response->preferred_vector_width_double_ ];
    device_info_.preferred_vector_width_half_   = vector_sizes[ response->preferred_vector_width_half_   ];
    device_info_.native_vector_width_char_   = vector_sizes[ response->native_vector_width_char_   ];
    device_info_.native_vector_width_short_  = vector_sizes[ response->native_vector_width_short_  ];
    device_info_.native_vector_width_int_    = vector_sizes[ response->native_vector_width_int_    ];
    device_info_.native_vector_width_long_   = vector_sizes[ response->native_vector_width_long_   ];
    device_info_.native_vector_width_float_  = vector_sizes[ response->native_vector_width_float_  ];
    device_info_.native_vector_width_double_ = vector_sizes[ response->native_vector_width_double_ ];
    device_info_.native_vector_width_half_   = vector_sizes[ response->native_vector_width_half_   ];
    //device_info_.max_clock_frequency_;
    //device_info_.address_bits_;
    //device_info_.max_read_image_args_;
    //device_info_.max_write_image_args_;
    //device_info_.max_mem_alloc_size_;
    //cl_bool image_support_;
    //device_info_.image2d_max_width_;
    //device_info_.image2d_max_height_;
    //device_info_.image3d_max_width_;
    //device_info_.image3d_max_height_;
    //device_info_.image3d_max_depth_;
    //device_info_.max_parameter_size_;
    //device_info_.max_samplers_;
    //device_info_.mem_base_address_align_;
    //device_info_.min_data_type_align_size_;
    //cl_device_fp_config single_fp_config_;
    //cl_device_mem_cache_type global_mem_cache_type_;
    //device_info_.global_mem_cacheline_size_;
    //device_info_.global_mem_cache_size_;
    //device_info_.global_mem_size_;
    //device_info_.max_constant_buffer_size_;
    //device_info_.max_constant_args_;
    //cl_device_local_mem_type local_mem_type_;
    //device_info_.local_mem_size_;
    //cl_bool error_correction_support_;
    //cl_bool host_unified_memory_;
    //device_info_.profiling_timer_resolution_;
    //cl_bool endian_little_;
    //cl_bool avaiable_;
    //cl_bool compiler_avaiable_;
    //cl_device_exec_capabilities execution_capabilities_;
    //cl_command_queue_properties queue_properties_;
    //std::string name_;
    //std::string vendor_;
    //std::string driver_version_;
    //std::string profile_;
    //std::string version_;
    //std::string opencl_c_version_;
    //std::string extensions_;
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetDeviceInfo >::create_response( uint8_t* payload_ptr )
{
    msgGetDeviceInfo_response* response = reinterpret_cast< msgGetDeviceInfo_response* >( payload_ptr );

}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
