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
#ifndef _DCL_CLBENCH_H_
#define _DCL_CLBENCH_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <stdint.h>
#include <string>
#include <CL/cl.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
//-----------------------------------------------------------------------------
namespace dcl {
namespace benchmark {
//-----------------------------------------------------------------------------
template< typename T >
class source_generator{};
//-----------------------------------------------------------------------------
template<>
class source_generator< cl_double >
{
protected:
    source_generator(){}
    const std::string& get_source() const { return source_; }

private:
    static std::string source_;
};
//-----------------------------------------------------------------------------
template<>
class source_generator< cl_float >
{
protected:
    source_generator(){}
    const std::string& get_source() const { return source_; }

private:
    static std::string source_;
};
//-----------------------------------------------------------------------------
template<>
class source_generator< cl_int >
{
protected:
    source_generator(){}
    const std::string& get_source() const { return source_; }

private:
    static std::string source_;
};
//-----------------------------------------------------------------------------
template< typename T >
class data_generator
{
protected:
    data_generator()
    {
        srand( 5154 );
    }

    void* get_matrixA_data( uint32_t size )
    {
        if( matrixA_.size() != size*size )
            setup_matrices( size );

        return matrixA_.data();
    }

    void* get_matrixB_data( uint32_t size )
    {
        if( matrixB_.size() != size*size )
            setup_matrices( size );

        return matrixB_.data();
    }

    void* get_result_buffer( uint32_t size )
    {
        if( resultMatrix_.size() != size*size )
            setup_matrices( size );

        return resultMatrix_.data();
    }

    void setup_matrices( uint32_t size )
    {
        boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock( mutex_ );

        matrixA_.resize( size * size );
        matrixB_.resize( size * size );
        resultMatrix_.resize( size * size, 0 );

        for( uint32_t y = 0; y < size; ++y )
        {
            for( uint32_t x = 0; x < size; ++x )
            {
                //matrixA_[y*size+x] = static_cast<T>( rand() );
                //matrixB_[y*size+x] = static_cast<T>( rand() );
                matrixA_[y*size+x] = static_cast<T>( x==y? 1 : 0 );
                matrixB_[y*size+x] = static_cast<T>( x==y? 1 : 0 );
            }
        }
    }

private:
    std::vector<T> matrixA_;
    std::vector<T> matrixB_;
    std::vector<T> resultMatrix_;
    boost::interprocess::interprocess_mutex mutex_;
};
//-----------------------------------------------------------------------------
template< typename T >
struct double_check{ bool is_double(){ return false; } };
//-----------------------------------------------------------------------------
template<>
struct double_check<double>{ bool is_double(){ return true; } };
//-----------------------------------------------------------------------------
template< typename T >
class clbench :
    public source_generator<T>,
    public data_generator<T>,
    public double_check<T>
{
public:
    clbench( uint32_t begin, uint32_t end, uint32_t iterations ) :
        begin_(begin), end_(end), iterations_(iterations){}

    void run()
    {
        if( !setup_cl() ) return;
        if( !load_kernel() ) return;

        for( uint32_t i = begin_; i <= end_; ++i )
        {
            if( !load_data( i ) ) return;
            if( !execute_kernels( i ) ) return;
        }

        cleanup();
    }

private:
    uint32_t begin_, end_, iterations_;
    boost::scoped_ptr<cl::Context> context_;
    boost::scoped_ptr<cl::Program> program_;
    boost::scoped_ptr<cl::Kernel> kernel_;
    boost::scoped_ptr<cl::Buffer> matrixA_, matrixB_, resultMatrix_;
    std::vector<cl::Device> devices_;

    typedef T t_value_type;

    bool setup_cl()
    {
        cl_int err;

        // Platform info
        std::vector<cl::Platform> platforms;
        err = cl::Platform::get(&platforms);

        if(err != CL_SUCCESS)
        {
            std::cerr << "Platform::get() failed (" << err << ")" << std::endl;
            return false;
        }

        std::vector<cl::Platform>::iterator i = platforms.begin();
        cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(*i)(), 0 };

        context_.reset( new cl::Context(CL_DEVICE_TYPE_GPU, cps, NULL, NULL, &err) );
        if (err != CL_SUCCESS)
        {
            std::cerr << "Context::Context() failed (" << err << ")\n";
            return false;
        }

        std::vector<cl::Device> devs;
        devs = context_->getInfo<CL_CONTEXT_DEVICES>();
        if (devs.size() == 0) {
            std::cerr << "No device in context\n";
            return false;
        }

        for( uint32_t i = 0; i < devs.size(); ++i )
        {
            if( !double_check<T>::is_double() || 
                devs[i].getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE>() != 0 )
            {
                devices_.push_back( devs[i] );
            }
        }

        if( devices_.size() == 0 )
        {
            std::cerr << "No device available\n";
            return false;
        }

        return true;
    }

    bool load_kernel()
    {
        cl_int err;

        cl::Program::Sources sources(
                    1, std::make_pair( source_generator<T>::get_source().data(),
                                       source_generator<T>::get_source().size()));

        program_.reset( new cl::Program( *context_, sources ) );
        err = program_->build( devices_ );
        if (err != CL_SUCCESS) {
            std::cerr << "Program::build() failed (" << err << ")\n";
            return false;
        }

        kernel_.reset( new cl::Kernel( *program_, "bench", &err) );
        if (err != CL_SUCCESS) {
            std::cerr << "Kernel::Kernel() failed (" << err << ")\n";
            return false;
        }

        return true;
    }

    bool load_data( uint32_t size )
    {
        cl_int err;

        matrixA_.reset( new cl::Buffer( *context_, CL_MEM_READ_ONLY,
                                        sizeof(t_value_type) * size * size,
                                        NULL, &err) );
        if (err != CL_SUCCESS)
            return false;

        matrixB_.reset( new cl::Buffer( *context_, CL_MEM_READ_ONLY,
                                        sizeof(t_value_type) * size * size,
                                        NULL, &err) );
        if (err != CL_SUCCESS)
            return false;

        resultMatrix_.reset( new cl::Buffer( *context_, CL_MEM_WRITE_ONLY,
                                             sizeof(t_value_type) * size * size,
                                             NULL, &err) );
        if (err != CL_SUCCESS)
            return false;

        data_generator<T>::setup_matrices( size );

        return true;
    }

    bool execute_kernels( uint32_t size )
    {
        boost::thread_group threads;

        for( uint32_t i = 0; i < devices_.size(); ++i )
            threads.create_thread( boost::bind( &dcl::benchmark::clbench<T>::bench, this, size, i ) );

        threads.join_all();

        return true;
    }

    void bench( uint32_t size, uint32_t index )
    {
        cl_int err;

        boost::scoped_ptr<cl::CommandQueue> queue(
            new cl::CommandQueue(*context_, devices_[index], 0, &err) );

        err = queue->enqueueWriteBuffer( *matrixA_, CL_FALSE, 0,
                                         sizeof(t_value_type) * size * size,
                                         data_generator<T>::get_matrixA_data( size ),
                                         NULL, NULL );
        if (err != CL_SUCCESS)
            return;

        err = queue->enqueueWriteBuffer( *matrixB_, CL_FALSE, 0,
                                         sizeof(t_value_type) * size * size,
                                         data_generator<T>::get_matrixB_data( size ),
                                         NULL, NULL );
        if (err != CL_SUCCESS)
            return;

        queue->flush();

        boost::timer::cpu_timer t;
        for( uint32_t i = 0; i < iterations_; ++ i )
        {
            t.start();

            kernel_->setArg( 0, *matrixA_ );
            kernel_->setArg( 1, *matrixB_ );
            kernel_->setArg( 2, *resultMatrix_ );
            kernel_->setArg( 3, size );

            cl::Event execEvent;
            err = queue->enqueueNDRangeKernel( *kernel_, cl::NullRange,   // offset
                                               cl::NDRange( size, size ), // global
                                               cl::NullRange,             // local
                                               0, &execEvent );

            std::vector<cl::Event> waitEvents;
            waitEvents.push_back( execEvent );

            t_value_type* buffer = static_cast<t_value_type*>(data_generator<T>::get_result_buffer( size ));

            cl::Event readEvent;
            err = queue->enqueueReadBuffer( *resultMatrix_, CL_FALSE, 0,
                                            sizeof(t_value_type) * size * size,
                                            buffer, &waitEvents, &readEvent );
            queue->finish();

            readEvent.wait();

            t.stop();
            std::cout << t.format();

            for( uint32_t y = 0; y < size; ++y )
            {
                for( uint32_t x = 0; x < size; ++x )
                {
                    if( buffer[y*size+x] != ((x==y)?1:0) )
                    {
                        std::cerr << "Matriz resultado errado" << std::endl;
                        exit(1);
                    }
                }
            }
        }
    }

    void cleanup(){}
};
//-----------------------------------------------------------------------------
}} // namespace dcl::benchmark
//-----------------------------------------------------------------------------
#endif // _DCL_CLBENCH_H_
