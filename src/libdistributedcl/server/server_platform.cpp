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
#include "server_platform.h"
#include "message/message.h"
#include "message/msg_device.h"
#include "composite/opencl_composite.h"
using dcl::network::message::dcl_message;
using dcl::network::message::msgGetDeviceIDs;
using dcl::composite::opencl_composite;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace server {
//-----------------------------------------------------------------------------
void GetDeviceIDs_command::execute()
{
    message_.set_cpu_count( 0 );
    message_.set_gpu_count( 0 );
    message_.set_accelerator_count( 1 );
    message_.set_other_count( 0 );
}
//-----------------------------------------------------------------------------
void GetDeviceInfo_command::execute()
{
    dcl::info::device_info info;

    info.type_ = CL_DEVICE_TYPE_ACCELERATOR;
    info.vendor_id_ = 0xe1babaca;
    info.max_compute_units_ = 2;
    info.max_work_item_dimensions_ = 1024;
    info.max_work_item_sizes_[0] = 1025;
    info.max_work_item_sizes_[1] = 1026;
    info.max_work_item_sizes_[2] = 1027;
    info.max_work_group_size_ = 1028;
    info.preferred_vector_width_char_ = 16;
    info.preferred_vector_width_short_ = 8;
    info.preferred_vector_width_int_ = 4;
    info.preferred_vector_width_long_ = 2;
    info.preferred_vector_width_float_ = 4;
    info.preferred_vector_width_double_ = 1;
    info.preferred_vector_width_half_ = 0;
    info.native_vector_width_char_ = 16;
    info.native_vector_width_short_ = 8;
    info.native_vector_width_int_ = 4;
    info.native_vector_width_long_ = 2;
    info.native_vector_width_float_ = 4;
    info.native_vector_width_double_ = 1;
    info.native_vector_width_half_ = 0;
    info.max_clock_frequency_ = 1029;
    info.address_bits_ = 64;
    info.max_read_image_args_ = 1030;
    info.max_write_image_args_ = 1031;
    info.max_mem_alloc_size_ = 1032;
    info.image_support_ = true;
    info.image2d_max_width_ = 1033;
    info.image2d_max_height_ = 1034;
    info.image3d_max_width_ = 1035;
    info.image3d_max_height_ = 1036;
    info.image3d_max_depth_ = 1037;
    info.max_parameter_size_ = 1038;
    info.max_samplers_ = 1039;
    info.mem_base_address_align_ = 1040;
    info.min_data_type_align_size_ = 1041;
    info.single_fp_config_ = 0x7f;
    info.global_mem_cache_type_ = CL_READ_WRITE_CACHE;
    info.global_mem_cacheline_size_ = 1043;
    info.global_mem_cache_size_ = 1044;
    info.global_mem_size_ = 1045;
    info.max_constant_buffer_size_ = 1046;
    info.max_constant_args_ = 1047;
    info.local_mem_type_ = CL_LOCAL;
    info.local_mem_size_ = 1042;
    info.error_correction_support_ = false;
    info.host_unified_memory_ = false;
    info.profiling_timer_resolution_ = 1048;
    info.endian_little_ = true;
    info.avaiable_ = true;
    info.compiler_avaiable_ = true;
    info.execution_capabilities_ = CL_EXEC_KERNEL;
    info.queue_properties_ = CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
    info.name_.assign( "DistributedCL Sample" );
    info.vendor_.assign( "DistributedCL Project" );
    info.driver_version_.assign( "OpenCL 1.0 DistributedCL Sample 0.1" );
    info.profile_.assign( "FULL PROFILE" );
    info.version_.assign( "OpenCL 1.0 DistributedCL Sample 0.1" );
    info.opencl_c_version_.assign( "OpenCL C 1.0 DistributedCL Sample 0.1" );
    info.extensions_.assign( "cl_khr_icd" );

    message_.set_info( info );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::server
//-----------------------------------------------------------------------------
