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
    }

    void* get_vectorA_data( uint32_t size )
    {
        if( vectorA_.size() != size )
            setup_vectors( size );

        return vectorA_.data();
    }

    void* get_vectorB_data( uint32_t size )
    {
        if( vectorB_.size() != size )
            setup_vectors( size );

        return vectorB_.data();
    }

    void* get_result_buffer( uint32_t size )
    {
        if( result_vector_.size() != size )
            setup_vectors( size );

        return result_vector_.data();
    }

    void setup_vectors( uint32_t size )
    {
        boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock( mutex_ );

        vectorA_.resize( size, static_cast<T>( 1 ) );
        vectorB_.resize( size, static_cast<T>( 1 ) );
        result_vector_.resize( size, 0 );
    }

private:
    std::vector<T> vectorA_;
    std::vector<T> vectorB_;
    std::vector<T> result_vector_;
    boost::interprocess::interprocess_mutex mutex_;
};
//-----------------------------------------------------------------------------
template< typename T >
struct double_check{ bool is_double(){ return false; } };
//-----------------------------------------------------------------------------
template<>
struct double_check<double>{ bool is_double(){ return true; } };
//-----------------------------------------------------------------------------
class result
{
public:
    result(){}
    void add_result( boost::timer::cpu_times times )
    {
        results_.push_back( static_cast<uint64_t>( times.wall ) );
    }

    size_t get_result_count() const
    {
        return results_.size();
    }

    uint64_t operator[]( size_t index ) const
    {
        return results_[ index ];
    }

private:
    std::vector<uint64_t> results_;
};
//-----------------------------------------------------------------------------
struct iteration_data
{
    uint32_t number;
    cl::Event read_event;
    std::vector<cl::Event> execute_event;
    boost::timer::cpu_timer timer;

    iteration_data() : execute_event(1){}
};
//-----------------------------------------------------------------------------
template< typename T >
class clbench :
    public source_generator<T>,
    public data_generator<T>,
    public double_check<T>
{
public:
    clbench( const std::vector<uint32_t>& sizes, uint32_t iterations ) :
        iterations_(iterations), sizes_( sizes )
    {
    }

    void run()
    {
        if( !setup_cl() ) return;
        if( !load_kernel() ) return;

        for( uint32_t i = 0; i < sizes_.size(); ++i )
        {
            results_[ sizes_[ i ] ];
            if( !load_data( sizes_[ i ] ) ) return;
            if( !execute_kernels( sizes_[ i ] ) ) return;
        }

        print_results();

        cleanup();
    }

private:
    uint32_t iterations_;
    std::vector<uint32_t> sizes_;
    boost::scoped_ptr<cl::Context> context_;
    boost::scoped_ptr<cl::Program> program_;
    boost::scoped_ptr<cl::Kernel> kernel_;
    boost::scoped_ptr<cl::Buffer> vectorA_, vectorB_, result_vector_;
    std::vector<cl::Device> devices_;
    std::map<uint32_t, std::vector<result> > results_;

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

        vectorA_.reset( new cl::Buffer( *context_, CL_MEM_READ_ONLY,
                                        sizeof(t_value_type) * size,
                                        NULL, &err) );
        if (err != CL_SUCCESS)
            return false;

        vectorB_.reset( new cl::Buffer( *context_, CL_MEM_READ_ONLY,
                                        sizeof(t_value_type) * size,
                                        NULL, &err) );
        if (err != CL_SUCCESS)
            return false;

        result_vector_.reset( new cl::Buffer( *context_, CL_MEM_WRITE_ONLY,
                                             sizeof(t_value_type) * size,
                                             NULL, &err) );
        if (err != CL_SUCCESS)
            return false;

        data_generator<T>::setup_vectors( size );

        return true;
    }

    bool execute_kernels( uint32_t size )
    {
        boost::thread_group threads;

        results_[ size ].resize( devices_.size() );

        for( uint32_t i = 0; i < devices_.size(); ++i )
            threads.create_thread( boost::bind( &dcl::benchmark::clbench<T>::bench, this, size, i ) );

        threads.join_all();

        return true;
    }

    void bench( uint32_t size, uint32_t index )
    {
        cl_int err;

        std::cerr << "Size " << size;
        boost::scoped_ptr<cl::CommandQueue> queue(
            new cl::CommandQueue(*context_, devices_[index], 0, &err) );

        err = queue->enqueueWriteBuffer( *vectorA_, CL_FALSE, 0,
                                         sizeof(t_value_type) * size,
                                         data_generator<T>::get_vectorA_data( size ),
                                         NULL, NULL );
        if (err != CL_SUCCESS)
            return;

        err = queue->enqueueWriteBuffer( *vectorB_, CL_FALSE, 0,
                                         sizeof(t_value_type) * size,
                                         data_generator<T>::get_vectorB_data( size ),
                                         NULL, NULL );
        if (err != CL_SUCCESS)
            return;

        queue->flush();

        t_value_type* buffer = static_cast<t_value_type*>(data_generator<T>::get_result_buffer( size ));
        std::vector<iteration_data> data(iterations_);

        for( uint32_t i = 0; i < iterations_; ++i )
        {
            data[ i ].number = i;
            data[ i ].timer.start();

            kernel_->setArg( 0, *vectorA_ );
            kernel_->setArg( 1, *vectorB_ );
            kernel_->setArg( 2, *result_vector_ );
            kernel_->setArg( 3, size );

            queue->enqueueNDRangeKernel( *kernel_, cl::NullRange,   // offset
                                         cl::NDRange( size ),       // global
                                         cl::NullRange, 0,          // local
                                         NULL );

            for( uint32_t j = 0; j < 10; ++j )
            {
                queue->enqueueReadBuffer( *result_vector_, CL_FALSE, 0,
                                          sizeof(t_value_type) * size,
                                          buffer, NULL, &data[ i ].read_event );

                kernel_->setArg( 0, *vectorA_ );
                kernel_->setArg( 1, *vectorB_ );
                kernel_->setArg( 2, *result_vector_ );
                kernel_->setArg( 3, size );

                queue->enqueueNDRangeKernel( *kernel_, cl::NullRange,   // offset
                                             cl::NDRange( size ),       // global
                                             cl::NullRange, 0,          // local
                                             NULL );

                data[ i ].read_event.wait();
            }

            queue->enqueueReadBuffer( *result_vector_, CL_FALSE, 0,
                                      sizeof(t_value_type) * size,
                                      buffer, NULL, &data[ i ].read_event );

            data[ i ].read_event.wait();

            data[ i ].timer.stop();
            std::cerr << ".";
            results_[ size ][ index ].add_result( data[ i ].timer.elapsed() );
        }
        std::cerr << std::endl;
    }

    void print_results()
    {
        typedef std::map< uint32_t, std::pair< uint64_t, uint64_t> > t_size_data;
        typedef std::map<uint32_t, t_size_data > t_all_times;
        t_all_times all_times;

        for( uint32_t index = 0; index < devices_.size(); ++index )
        {
            std::cout << "------------------------------" << std::endl 
                      << "Device " << devices_[ index ].getInfo<CL_DEVICE_NAME>() << std::endl << std::endl;

            for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
            {
                uint32_t size = sizes_[ sizeIndex ];

                std::cout << "Size " << size * sizeof(t_value_type) <<"(" << size << ")" << std::endl; 

                uint32_t second = 1;
                uint32_t count = 1;
                uint64_t total_time = 0;
                for( uint32_t i = 0; i < results_[ size ][ index ].get_result_count(); ++i )
                {
                    total_time += results_[ size ][ index ][ i ];
                    all_times[ second ][ size ].second += results_[ size ][ index ][ i ];

                    if( total_time >= 1000000000LL ) // 1 sec
                    {
                        std::cout << second << "s: " << static_cast<double>(count)*1000000000./static_cast<double>(total_time) << "mult/s" << std::endl;

                        count = 0;
                        total_time -= 1000000000LL;
                        ++second;
                        all_times[ second ][ size ].second += total_time;
                    }

                    ++count;
                    ++(all_times[ second ][ size ].first);
                }

                if( total_time != 0 )
                    std::cout << second << "s: " << static_cast<double>(count)*1000000000./static_cast<double>(total_time) << "mult/s" << std::endl;

                std::cout << std::endl;
            }
        }


        std::cout << "------------------------------" << std::endl 
                  << "Totals" << std::endl << std::endl;

        // CSV of the totals
        std::cout << "\"Seconds\"";
        for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
        {
            uint32_t size = sizes_[ sizeIndex ];
            std::cout << ",\"" << size * sizeof(t_value_type) << " (" << size << ")\"";
        }

        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(8) << std::endl;

        for( t_all_times::iterator it = all_times.begin(); it != all_times.end(); ++it )
        {
            std::cout << "\"" << it->first << "\"";
            for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
            {
                uint32_t size = sizes_[ sizeIndex ];

                if( it->second[ size ].second != 0 )
                {
                    double mul_per_sec = static_cast<double>(devices_.size()) *
                                         static_cast<double>(it->second[ size ].first) *
                                         1000000000. /
                                         static_cast<double>(it->second[ size ].second);

                    std::cout << ",\"" << mul_per_sec << "\"";
                }
                else
                {
                    std::cout << ",";
                }

            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    void cleanup(){}
};
//-----------------------------------------------------------------------------
}} // namespace dcl::benchmark
//-----------------------------------------------------------------------------
#endif // _DCL_CLBENCH_H_