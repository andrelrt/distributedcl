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
#include "server_sampler.h"
#include "server_platform.h"
#include "message/msg_sampler.h"
#include "composite/composite_context.h"
#include "composite/composite_sampler.h"
using dcl::composite::composite_context;
using dcl::composite::composite_sampler;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
void msgCreateSampler_command::execute()
{
    server_platform& server = session_context_ptr_->get_server_platform();

    remote_id_t context_id = message_->get_context_id();

    composite_context* context_ptr = server.get_context_manager().get( context_id );

    composite_sampler* sampler_ptr = reinterpret_cast<composite_sampler*>(
        context_ptr->create_sampler( message_->get_normalized_coords(),
                                     message_->get_addressing_mode(),
                                     message_->get_filter_mode() ) );

    remote_id_t id = server.get_sampler_manager().add( sampler_ptr );

    message_->set_remote_id( id );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
