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
#ifndef _DCL_DEVICE_MESSAGES_H_
#define _DCL_DEVICE_MESSAGES_H_

#include "message.h"
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
        base_message( msgGetDeviceIDs, true ), 
        cpu_count_( 0 ), gpu_count_( 0 ), accelerator_count_( 0 ), other_count_( 0 ) {}

    virtual void set_response( const base_message* response_ptr );

    inline std::size_t get_device_count()
    {
        return cpu_count_ + gpu_count_ + accelerator_count_ + other_count_;
    }

    inline std::size_t get_cpu_count()
    {
        return cpu_count_;
    }

    inline std::size_t get_gpu_count()
    {
        return gpu_count_;
    }

    inline std::size_t get_accelerator_count()
    {
        return accelerator_count_;
    }

    inline std::size_t get_other_count()
    {
        return other_count_;
    }

    inline void set_cpu_count( std::size_t cpu_count )
    {
        cpu_count_ = cpu_count;
    }

    inline void set_gpu_count( std::size_t gpu_count )
    {
        gpu_count_ = gpu_count;
    }

    inline void set_accelerator_count( std::size_t accelerator_count )
    {
        accelerator_count_ = accelerator_count;
    }

    inline void set_other_count( std::size_t other_count )
    {
        other_count_ = other_count;
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
    };
    #pragma pack( pop )

    std::size_t cpu_count_;
    std::size_t gpu_count_;
    std::size_t accelerator_count_;
    std::size_t other_count_;
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_DEVICE_MESSAGES_H_
