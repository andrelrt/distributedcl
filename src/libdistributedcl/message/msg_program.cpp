/*
 * Copyright (c) 2009-2012 Andr� Tupinamb� (andrelrt@gmail.com)
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
#include "msg_program.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::create_request( uint8_t* payload_ptr )
{
    msgCreateProgramWithSource_request* request_ptr = 
        reinterpret_cast< msgCreateProgramWithSource_request* >( payload_ptr );

    request_ptr->context_id_ = host_to_network( context_id_ );
    request_ptr->source_code_len_ = host_to_network( static_cast<uint32_t>( source_code_.length() ) );
    
    memcpy( request_ptr->source_code_, source_code_.data(), source_code_.length() );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::parse_request( const uint8_t* payload_ptr )
{
    const msgCreateProgramWithSource_request* request_ptr = 
        reinterpret_cast< const msgCreateProgramWithSource_request* >( payload_ptr );

    context_id_ = network_to_host( request_ptr->context_id_ );

    source_code_.assign( reinterpret_cast<const char*>( request_ptr->source_code_ ), 
                         network_to_host( request_ptr->source_code_len_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::create_response( uint8_t* payload_ptr )
{
    dcl::remote_id_t* response_ptr = reinterpret_cast< dcl::remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateProgramWithSource >::parse_response( const base_message* message_ptr )
{
    const dcl_message< msgCreateProgramWithSource >* msg_response_ptr = 
        reinterpret_cast< const dcl_message< msgCreateProgramWithSource >* >( message_ptr );

    const dcl::remote_id_t* response_ptr = 
        reinterpret_cast< const dcl::remote_id_t* >( msg_response_ptr->get_payload() );

    id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
void dcl_message< msgBuildProgram >::create_request( uint8_t* payload_ptr )
{
    msgBuildProgram_request* request_ptr = 
        reinterpret_cast< msgBuildProgram_request* >( payload_ptr );

    request_ptr->program_id_ = host_to_network( program_id_ );
    request_ptr->build_options_len_ = host_to_network( static_cast<uint32_t>( build_options_.length() ) );
    
    memcpy( request_ptr->build_options_, build_options_.data(), build_options_.length() );
}
//-----------------------------------------------------------------------------
void dcl_message< msgBuildProgram >::parse_request( const uint8_t* payload_ptr )
{
    const msgBuildProgram_request* request_ptr = 
        reinterpret_cast< const msgBuildProgram_request* >( payload_ptr );

    program_id_ = network_to_host( request_ptr->program_id_ );

    build_options_.assign( reinterpret_cast<const char*>( request_ptr->build_options_ ), 
                           network_to_host( request_ptr->build_options_len_ ) );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------