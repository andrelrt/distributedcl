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
#ifndef _DCL_NETWORK_SERVER_H_
#define _DCL_NETWORK_SERVER_H_

#include "distributedcl_internal.h"
#include "server_session.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace server {
//-----------------------------------------------------------------------------
template< template< class > class COMM >
class server
{
public:
    typedef COMM< server > communication_t;
    typedef typename COMM< server >::config_info_t config_info_t;

    server( const config_info_t& config ) : communication_( config )
    {
        communication_.startup( this );
    }

    ~server()
    {
        communication_.shutdown();
    }

    inline void start_accept_thread()
    {
        communication_.start_accept_thread();
    }

    inline void on_accept( typename communication_t::connection_t& connection,
                           typename communication_t::endpoint_t& client )
    {
        typename server_session< COMM >::config_info_t config( connection, client );

        server_session< COMM >* new_session = new server_session< COMM >( config );

        new_session->startup();
    }

    inline void wait()
    {
        communication_.wait();
    }

private:
    communication_t communication_;
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::server
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_SERVER_H_
