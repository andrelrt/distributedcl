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
#include "opencl_single.h"
#include "platform.h"
using dcl::info::generic_platform;
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
const platforms_t& opencl_single::get_platforms()
{
    if( platforms_.empty() )
    {
        cl_uint num_platforms = 0;
        cl_int ret = opencl_.clGetPlatformIDs( 0, NULL, &num_platforms );

        if( ret != CL_SUCCESS )
            throw dcl::library_exception( ret );

        cl_uint num_entries = num_platforms;
        boost::scoped_array< cl_platform_id > platformIds( new cl_platform_id[ num_platforms ] );

        ret = opencl_.clGetPlatformIDs( num_entries, platformIds.get(), &num_platforms );

        if( ret != CL_SUCCESS )
        {
            throw dcl::library_exception( ret );
        }

        for( cl_uint i = 0; i < num_entries; i++ )
        {
            platforms_.push_back( reinterpret_cast< generic_platform* >( new platform( opencl_, platformIds[ i ] ) ) );
        }
    }

    return platforms_;
}
//-----------------------------------------------------------------------------
//const devices_t& opencl_single::get_devices()
//{
//    if( devices_.empty() )
//    {
//        if( platforms_.empty() )
//            get_platforms();
//
//        platforms_t::iterator it;
//
//        for( it = platforms_.begin(); it != platforms_.end(); ++it )
//        {
//            cl_uint num_devices = 0;
//            cl_int ret = opencl_.clGetDeviceIDs( it->second->get_id(), CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices );
//
//            if( ret != CL_SUCCESS )
//            {
//                devices_.clear();
//                throw library_exception( ret );
//            }
//
//            cl_uint num_entries = num_devices;
//            boost::scoped_array<cl_device_id> deviceIds( new cl_device_id[ num_devices ] );
//
//            ret = opencl_.clGetDeviceIDs( it->second->get_id(), CL_DEVICE_TYPE_ALL,
//                                          num_entries, deviceIds.get(), &num_devices );
//            if( ret != CL_SUCCESS )
//            {
//                devices_.clear();
//                throw new library_exception( ret );
//            }
//
//            for( cl_uint i = 0; i < num_entries; i++ )
//            {
//                device* device_ptr = new device( opencl_, *(it->second), deviceIds[ i ] );
//
//                devices_.push_back( device_ptr );
//                it->second->add_device( device_ptr );
//            }
//        }
//    }
//
//    return( devices_ );
//}
//-----------------------------------------------------------------------------
//void opencl_single::get_devices( devices_t& devices, const platform& platform_ref,
//                                 cl_device_type device_type )
//{
//	if( devices_.empty() )
//		get_devices();
//
//	devices.clear();
//
//	devices_iterator_t it;
//
//	for( it = devices_.begin(); it != devices_.end(); ++it )
//	{
//		if( (device_type & (*it)->get_type()) &&
//            ((platform_ref.get_id() == NULL) ||
//			 (platform_ref.get_id() == (*it)->get_platform_id())) )
//		{
//			devices.push_back( *it );
//		}
//	}
//}
//-----------------------------------------------------------------------------
//void opencl_single::get_devices( devices_t& devices, cl_device_type device_type )
//{
//	get_devices( devices, platform(), device_type );
//}
//-----------------------------------------------------------------------------
//context* opencl_single::create_context( const devices_t& devices, const platform& platform_ref )
//{
//	return new context( opencl_, devices, platform_ref );
//}
//-----------------------------------------------------------------------------
//context* opencl_single::create_context( cl_device_type device_type, const platform& platform_ref )
//{
//	return new context( opencl_, device_type, platform_ref );
//}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
