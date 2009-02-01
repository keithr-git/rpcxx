//
//    Copyright 2008, 2009 Keith Reynolds.
//
//    This file is part of RPC++.
//
//    RPC++ is free software: you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation,
//    either version 2.1 of the License, or (at your option) any
//    later version.
//
//    RPC++ is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty
//    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//    See the GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General
//    Public License along with RPC++.  If not, see
//    <http://www.gnu.org/licenses/>.
//
#include <sys/socket.h>

#include <rpc++/errno.hpp>
#include <rpc++/socket.hpp>

#include <netdb.h>
#include <string.h>

#include <string>

using std::string;

namespace RPCXX
{
    Socket::Socket(int domain, int type, int protocol)
        : fd_(socket(domain, type, protocol))
    {
        if (fd_ == -1)
        {
            throw Error<SystemError>("Could not create socket");
        }
    }

    int Socket::close_read()
    {
        return shutdown(*this, SHUT_RD);
    }

    int Socket::close_write()
    {
        return shutdown(*this, SHUT_WR);
    }

    TCPClient::TCPClient(const char * server, int port)
        : TCPSocket()
    {
        struct addrinfo hints;
        struct addrinfo *addrs;
        char port_string[21];
        int result;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_NUMERICSERV;
        snprintf(port_string, sizeof(port_string), "%d", port);

        if ((result = getaddrinfo(server, port_string, &hints, &addrs)) != 0)
        {
            throw Error<LookupFailed>(string("Lookup failed for ") + server,
                    result);
        }

        if (connect(*this, addrs[0].ai_addr, addrs[0].ai_addrlen) == -1)
        {
            throw Error<SystemError>(string("Could not connect to ") + server +
                    " port " + port_string);
        }
    }

    Error<Socket::LookupFailed>::Error(const std::string& message,
            int error_code)
        : ErrorCode<Socket::LookupFailed>(message + ": " +
                gai_strerror(error_code), error_code)
    { }

    Error<Socket::LookupFailed>::Error(int error_code)
        : ErrorCode<Socket::LookupFailed>(gai_strerror(error_code),
                error_code)
    { }
};
