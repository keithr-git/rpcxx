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
#ifndef CPRC_SOCKET_HPP
#define CPRC_SOCKET_HPP

#include <netinet/in.h>

#include <rpc++/errno.hpp>
#include <rpc++/exception.hpp>

namespace RPCXX
{
    class Socket
    {
        int         fd_;

    public:
        //
        // Socket errors.
        //
        struct InvalidSocket {};
        struct LookupFailed {};

        Socket(int fd = -1)
            : fd_(fd)
        { }

        Socket(int domain, int type, int protocol);

        ~Socket()
        {
            if (fd_ != -1)
            {
                close(fd_);
            }
        }

        operator int()
        {
            if (fd_ == -1)
            {
                throw Error<InvalidSocket>("Socket not open");
            }

            return fd_;
        }

        int close_read();

        int close_write();
    };

    class TCPSocket
        : public Socket
    {
    public:
        TCPSocket(int domain = PF_INET)
            : Socket(domain, SOCK_STREAM, IPPROTO_TCP)
        { }
    };

    class TCPClient
        : public TCPSocket
    {
    public:
        TCPClient(const char * server, int port);
    };

    template<>
    class Error<Socket::LookupFailed>
        : public ErrorCode<Socket::LookupFailed>
    {
    public:
        Error(const std::string& message, int error_code);
        Error(int error_code);
    };
};

#endif /* CPRC_SOCKET_HPP */
