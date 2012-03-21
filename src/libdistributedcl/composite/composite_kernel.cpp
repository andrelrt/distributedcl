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
#include "composite_kernel.h"
#include "composite_command_queue.h"
#include "composite_memory.h"
using dcl::info::ndrange;
using dcl::info::generic_kernel;
using dcl::info::generic_context;
using dcl::info::generic_command_queue;
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
void composite_kernel::execute( const generic_command_queue* queue_ptr, 
                                const ndrange& offset, const ndrange& global, 
                                const ndrange& local )
{
    const generic_context* ctx = queue_ptr->get_context();
    generic_kernel* kernel_ptr = find( ctx );

    kernel_ptr->execute( queue_ptr, offset, global, local );
}
//-----------------------------------------------------------------------------
void composite_kernel::set_argument( uint32_t arg_index, const generic_memory* memory_ptr )
{
    const composite_memory* mem_ptr = 
        reinterpret_cast<const composite_memory*>( memory_ptr );

    for( iterator it = begin(); it != end(); it++ )
    {
        it->second->set_argument( arg_index, mem_ptr->find( it->first ) );
    }
}
//-----------------------------------------------------------------------------
void composite_kernel::set_argument( uint32_t arg_index, size_t arg_size, const void* arg_value )
{
    for( iterator it = begin(); it != end(); it++ )
    {
        it->second->set_argument( arg_index, arg_size, arg_value );
    }
}
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
