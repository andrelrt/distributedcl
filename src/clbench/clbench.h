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
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif
#ifndef _DCL_CLBENCH_H_
#define _DCL_CLBENCH_H_

#include <vector>
#include <map>
#include <stdint.h>
#include <string>
#include <CL/cl.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/detail/atomic.hpp>

#define MULTS_PER_ITERATION     10
//-----------------------------------------------------------------------------
namespace dcl {
namespace benchmark {
//-----------------------------------------------------------------------------
typedef boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> t_scoped_lock;
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
        t_scoped_lock lock( mutex_ );

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
struct double_check{ static bool is_double(){ return false; } };
//-----------------------------------------------------------------------------
template<>
struct double_check<double>{ static bool is_double(){ return true; } };
//-----------------------------------------------------------------------------
class result
{
public:
    result() : last_( 0 ), count_(0){}

    void add_result( boost::timer::cpu_times times )
    {
        ++count_;
        ++count_per_second_[ static_cast<uint32_t>(times.wall/1000000000LL) ];
        results_.push_back( static_cast<uint64_t>( times.wall ) - last_ );
        last_ = static_cast<uint64_t>( times.wall );
    }

    uint32_t get_count() const
    {
        return count_;
    }

    size_t get_seconds_count() const
    {
        return count_per_second_.size();
    }

    uint32_t get_second_count( uint32_t second )
    {
        return count_per_second_[ second ];
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
    uint64_t last_;
    std::vector<uint64_t> results_;
    volatile uint32_t count_;
    std::map<uint32_t, uint32_t> count_per_second_;
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
    clbench( const std::vector<uint32_t>& sizes, uint32_t timeout, bool divide ) :
        divide_( divide ), timeout_(timeout), sizes_( sizes )
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
    bool divide_;
    uint32_t timeout_;
    std::vector<uint32_t> sizes_;
    std::map<uint32_t, std::vector<uint32_t> > counts_;

    volatile uint32_t threads_running_;
    boost::scoped_ptr<cl::Context> context_;
    boost::scoped_ptr<cl::Program> program_;
    std::vector<cl::Device> devices_;

    typedef std::map<uint32_t, std::vector<result> > t_results;
    t_results results_;

    boost::interprocess::interprocess_mutex threads_mutex_;
    boost::interprocess::interprocess_condition start_threads_condition_;

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
        if( devs.empty() ) 
        {
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

        std::cerr << devices_[0].getInfo<CL_DEVICE_NAME>() << std::endl;

        if( devices_.empty() )
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

        return true;
    }

    bool load_data( uint32_t size )
    {
        return true;
    }

    typedef std::map< uint32_t, std::pair< uint64_t, uint64_t > > t_size_data;
    t_size_data size_average_;

    bool execute_kernels( uint32_t size )
    {
        boost::thread_group threads;

        std::cerr << "Size " << size;

        results_[ size ].resize( devices_.size() );

        boost::timer::cpu_timer global_timer;
        
        uint32_t vector_size = size;

        if( divide_ )
        {
            vector_size /= static_cast<uint32_t>( devices_.size() );

            if( (vector_size == 0) ||
                ((size % static_cast<uint32_t>( devices_.size() )) != 0) )
            {
                ++vector_size;
            }
        }

        threads_running_ = 0;

        for( uint32_t i = 0; i < devices_.size(); ++i )
        {
            results_[ size ][ i ];
            threads.create_thread( boost::bind( &dcl::benchmark::clbench<T>::bench, this, size, vector_size, i ) );
        }

        while( threads_running_ != devices_.size() )
            boost::this_thread::sleep( boost::posix_time::milliseconds(1) );

        {
            t_scoped_lock lock( threads_mutex_ );
            start_threads_condition_.notify_all();
        }

        global_timer.start();

        //uint32_t last_count = 0;
        //while( threads_running_ != 0 )
        //{
        //    boost::this_thread::sleep( boost::posix_time::seconds(1) );
        //    uint32_t total_count = 0;

        //    for( uint32_t i = 0; i < devices_.size(); ++i )
        //    {
        //        total_count += results_[ size ][ i ].get_count();
        //    }

        //    counts_[ size ].push_back( total_count - last_count );
        //    last_count = total_count;
        //}

        threads.join_all();
        global_timer.stop();

        size_average_[ size ].first = static_cast<uint64_t>( global_timer.elapsed().wall );
        size_average_[ size ].second = 0;

        std::cerr << std::setiosflags(std::ios::fixed) << std::setprecision(2) 
                  << " " << size_average_[ size ].first / 1000000000. << "s" << std::endl;

        return true;
    }

    void bench( uint32_t full_size, uint32_t size, uint32_t index )
    {
        cl_int err;
        boost::scoped_ptr<cl::Buffer> vectorA, vectorB, result_vector[4];
        boost::scoped_ptr<cl::Kernel> kernel[4];

        vectorA.reset( new cl::Buffer( *context_, CL_MEM_READ_ONLY,
                                        sizeof(t_value_type) * size,
                                        NULL, &err) );
        if (err != CL_SUCCESS)
            return;

        vectorB.reset( new cl::Buffer( *context_, CL_MEM_READ_ONLY,
                                        sizeof(t_value_type) * size,
                                        NULL, &err) );
        if (err != CL_SUCCESS)
            return;

        for( int i = 0; i < 4; ++i )
        {
            result_vector[i].reset( new cl::Buffer( *context_, CL_MEM_WRITE_ONLY,
                                                     sizeof(t_value_type) * size,
                                                     NULL, &err) );

            kernel[i].reset( new cl::Kernel( *program_, "bench", &err) );
        }

        if (err != CL_SUCCESS)
            return;

        data_generator<T>::setup_vectors( size );

        boost::scoped_ptr<cl::CommandQueue> queue(
            new cl::CommandQueue(*context_, devices_[index], 0, &err) );

        err = queue->enqueueWriteBuffer( *vectorA, CL_FALSE, 0,
                                         sizeof(t_value_type) * size,
                                         data_generator<T>::get_vectorA_data( size ),
                                         NULL, NULL );
        if (err != CL_SUCCESS)
            return;

        err = queue->enqueueWriteBuffer( *vectorB, CL_FALSE, 0,
                                         sizeof(t_value_type) * size,
                                         data_generator<T>::get_vectorB_data( size ),
                                         NULL, NULL );
        if (err != CL_SUCCESS)
            return;

        t_value_type* buffer = static_cast<t_value_type*>(data_generator<T>::get_result_buffer( size ));
        std::vector<iteration_data> data;

        uint64_t max_elapsed = timeout_ * 1000000000LL; // 1 sec
        uint32_t i = 0;

        // warm up
        kernel[0]->setArg( 0, *vectorA );
        kernel[0]->setArg( 1, *vectorB );
        kernel[0]->setArg( 2, *result_vector[0] );
        kernel[0]->setArg( 3, size );

        queue->enqueueNDRangeKernel( *kernel[0], cl::NullRange,   // offset
                                     cl::NDRange( size ),       // global
                                     cl::NullRange, 0,          // local
                                     NULL );

        queue->enqueueReadBuffer( *result_vector[0], CL_FALSE, 0,
                                  sizeof(t_value_type) * size,
                                  buffer, NULL, NULL );

        queue->finish();

        {
            t_scoped_lock lock( threads_mutex_ );
            ++threads_running_;
            start_threads_condition_.wait( lock );
        }
        //std::cerr << index;

        boost::timer::cpu_timer global_timer;
        global_timer.start();

        uint64_t second = 0;
        for(;;)
        {
            if( (divide_ && (i > 30)) ||
                (!divide_ && (max_elapsed < static_cast<uint64_t>( global_timer.elapsed().wall ))) )
            {
                break;
            }

            iteration_data this_data;
            this_data.number = i;
            this_data.timer.start();

            // Start outside
            kernel[0]->setArg( 0, *vectorA );
            kernel[0]->setArg( 1, *vectorB );
            kernel[0]->setArg( 2, *result_vector[0] );
            kernel[0]->setArg( 3, size );

            queue->enqueueNDRangeKernel( *kernel[0], cl::NullRange,   // offset
                                         cl::NDRange( size ),       // global
                                         cl::NullRange, 0,          // local
                                         NULL );

            queue->flush();

            for( uint32_t j = 0; j < MULTS_PER_ITERATION; ++j )
            {
                // read 0 --------------------
                queue->enqueueReadBuffer( *result_vector[0], CL_FALSE, 0,
                                          sizeof(t_value_type) * size,
                                          buffer, NULL, NULL );

                results_[ full_size ][ index ].add_result( global_timer.elapsed() );

                // 1 -------------------------
                kernel[1]->setArg( 0, *vectorA );
                kernel[1]->setArg( 1, *vectorB );
                kernel[1]->setArg( 2, *(result_vector[1]) );
                kernel[1]->setArg( 3, size );

                queue->enqueueNDRangeKernel( *kernel[1], cl::NullRange,   // offset
                                             cl::NDRange( size ),       // global
                                             cl::NullRange, 0,          // local
                                             NULL );
                queue->flush();

                // read 1 --------------------
                queue->enqueueReadBuffer( *result_vector[1], CL_FALSE, 0,
                                          sizeof(t_value_type) * size,
                                          buffer, NULL, NULL );

                results_[ full_size ][ index ].add_result( global_timer.elapsed() );

                // 2 -------------------------
                kernel[2]->setArg( 0, *vectorA );
                kernel[2]->setArg( 1, *vectorB );
                kernel[2]->setArg( 2, *(result_vector[2]) );
                kernel[2]->setArg( 3, size );

                queue->enqueueNDRangeKernel( *kernel[2], cl::NullRange,   // offset
                                             cl::NDRange( size ),       // global
                                             cl::NullRange, 0,          // local
                                             NULL );
                queue->flush();

                // read 2 --------------------
                queue->enqueueReadBuffer( *result_vector[2], CL_FALSE, 0,
                                          sizeof(t_value_type) * size,
                                          buffer, NULL, NULL );

                results_[ full_size ][ index ].add_result( global_timer.elapsed() );

                // 3 -------------------------
                kernel[3]->setArg( 0, *vectorA );
                kernel[3]->setArg( 1, *vectorB );
                kernel[3]->setArg( 2, *(result_vector[3]) );
                kernel[3]->setArg( 3, size );

                queue->enqueueNDRangeKernel( *kernel[3], cl::NullRange,   // offset
                                             cl::NDRange( size ),       // global
                                             cl::NullRange, 0,          // local
                                             NULL );
                queue->flush();

                // read 3 --------------------
                queue->enqueueReadBuffer( *result_vector[3], CL_FALSE, 0,
                                          sizeof(t_value_type) * size,
                                          buffer, NULL, NULL );

                results_[ full_size ][ index ].add_result( global_timer.elapsed() );

                // 0 -------------------------
                kernel[0]->setArg( 0, *vectorA );
                kernel[0]->setArg( 1, *vectorB );
                kernel[0]->setArg( 2, *(result_vector[0]) );
                kernel[0]->setArg( 3, size );

                queue->enqueueNDRangeKernel( *kernel[0], cl::NullRange,   // offset
                                             cl::NDRange( size ),       // global
                                             cl::NullRange, 0,          // local
                                             NULL );
                queue->flush();

            }

            // read 0 --------------------
            queue->enqueueReadBuffer( *result_vector[0], CL_FALSE, 0,
                                      sizeof(t_value_type) * size,
                                      buffer, NULL, NULL );
            queue->finish();

            results_[ full_size ][ index ].add_result( global_timer.elapsed() );

            this_data.timer.stop();
            if( !(i & 0xf) )
                std::cerr << ".";
            
            data.push_back( this_data );
            
            ++i;
        }

        //std::cerr << index;
        boost::interprocess::ipcdetail::atomic_dec32( &threads_running_ );
    }


    void print_results()
    {
        typedef std::map<uint32_t, t_size_data > t_all_times;
        t_all_times all_times;

        for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
        {
            uint32_t size = sizes_[ sizeIndex ];
            for( uint32_t index = 0; index < devices_.size(); ++index )
            {
                for( uint32_t i = 0; i < results_[ size ][ index ].get_seconds_count(); ++i )
                {
                    all_times[ i + 1 ][ size ].first += results_[ size ][ index ].get_second_count( i );
                    all_times[ i + 1 ][ size ].second = 1000000000LL;
                    size_average_[ size ].second += results_[ size ][ index ].get_second_count( i );
                }
            }


//            for( uint32_t index = 0; index < devices_.size(); ++index )
//            {
//                uint32_t second = 1;
//                uint64_t total_time = 0;
//                uint64_t total_ops = 0;
//                ++size_average_[ size ].second;
//
//                for( uint32_t i = 0; i < results_[ size ][ index ].get_result_count(); ++i )
//                {
//                    total_time += results_[ size ][ index ][ i ];
//                    size_average_[ size ].second += MULTS_PER_ITERATION * 4;
//
//                    total_ops += MULTS_PER_ITERATION * 4;
//
//                    if( total_time >= 1000000000LL ) // 1 sec
//                    {
//                        all_times[ second ][ size ].first += total_ops * 1000000000LL / total_time;
//                        all_times[ second ][ size ].second += 1000000000LL;
//
//                        ++second;
//                        total_time = 0;
//                        total_ops = 0;
//                    }
//                }
//
////                if( total_time != 0 )
////                    std::cout << second << "s: " << static_cast<double>(count)*1000000000./static_cast<double>(total_time) << "mult/s" << std::endl;
//
////                std::cout << std::endl;
//            }
        }


        std::cout << "------------------------------" << std::endl 
                  << "Operations per second" << std::endl << std::endl;

        // CSV of the totals
        std::cout << ",\"Seconds\"";
        for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
        {
            uint32_t size = sizes_[ sizeIndex ];
            std::cout << ",\"" << size << "\"";
        }
        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(8) << std::endl;

        std::cout << "\"Total time (s)\",";
        for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
        {
            std::cout << ",\"" << static_cast<double>(size_average_[ sizes_[ sizeIndex ] ].first)/1000000000. << "\"";
        }
        std::cout << std::endl;

        std::cout << "\"Operation count\",";
        for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
        {
            uint32_t size = sizes_[ sizeIndex ];
            std::cout << ",\"" << size_average_[ sizes_[ sizeIndex ] ].second << "\"";
        }
        std::cout << std::endl;

        std::cout << "\"Global average (op/s)\",";
        for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
        {
            uint32_t size = sizes_[ sizeIndex ];
            std::cout << ",\"=" << size_average_[ sizes_[ sizeIndex ] ].second
                      << "/" << static_cast<double>(size_average_[ sizes_[ sizeIndex ] ].first)/1000000000. << "\"";
        }
        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0) << std::endl;

        for( t_all_times::iterator it = all_times.begin(); it != all_times.end(); ++it )
        {
            std::cout << ",\"" << it->first << "\"";
            for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
            {
                uint32_t size = sizes_[ sizeIndex ];
                //double mul_per_sec = 0;

                if( it->second[ size ].second != 0 )
                {
                    std::cout << ",\"" << it->second[ size ].first << "\"";
                    //mul_per_sec = static_cast<double>(devices_.size()) *
                    //              static_cast<double>(it->second[ size ].first) *
                    //              1000000000. /
                    //              static_cast<double>(it->second[ size ].second);
                }

                //if( mul_per_sec > 0 )
                //{
                //    std::cout << ",\"" << mul_per_sec << "\"";
                //}
                else
                {
                    std::cout << ",";
                }

            }
            std::cout << std::endl;
        }

        std::cout << std::endl << std::endl;

        //std::cout << "------------------------------" << std::endl 
        //          << "Miliseconds per operation" << std::endl 
        //          << std::setiosflags(std::ios::fixed) << std::setprecision(4) << std::endl;

        //for( uint32_t sizeIndex = 0; sizeIndex < sizes_.size(); ++sizeIndex )
        //{
        //    uint32_t size = sizes_[ sizeIndex ];

        //    for( uint32_t index = 0; index < devices_.size(); ++index )
        //    {
        //        std::cout << "\"" << devices_[ index ].getInfo<CL_DEVICE_NAME>() << "\",\"" << size * sizeof(t_value_type) << " (" << size << ")\"";

        //        size_t result_count = results_[ size ][ index ].get_result_count();
        //        for( size_t i = 0; i < result_count ; ++i )
        //        {
        //            std::cout << ",\"" << static_cast<double>(results_[ size ][ index ][ i ])/1000000.0 << "\"";
        //        }
        //        std::cout << std::endl;
        //    }
        //}

        //std::cout << std::endl << std::endl;
    }

    void cleanup(){}
};
//-----------------------------------------------------------------------------
}} // namespace dcl::benchmark
//-----------------------------------------------------------------------------
#endif // _DCL_CLBENCH_H_
