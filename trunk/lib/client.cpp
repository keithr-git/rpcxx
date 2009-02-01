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
#include <sys/uio.h>

#include <errno.h>
#include <string.h>

#include <cstdio> /* DeBuG */
#include <stdexcept>
#include <string>

#include <rpc++/rpc++.hpp>
#include <rpc++/client.hpp>
#include <rpc++/socket.hpp>

using std::printf;
using std::string;

namespace RPCXX
{
    Client::Client(const char * server, int port)
        : socket_(server, port)
    {
    }

    void Client::send(int function, const RequestBuffer& requestBuffer,
            ReplyBuffer& replyBuffer)
    {
        static uint32_t sequence;
        Request request = { sequence++, function, requestBuffer.length() };
        Reply reply;
        iovec buffers[2] = {
            { &request, sizeof(request), },
            { (void*) (const char*) requestBuffer, requestBuffer.length(), },
        };
        char headerData[std::max(sizeof(Request), sizeof(Reply))];
        NetBuffer header(headerData, sizeof(headerData));
        unsigned int readLength;

        header << request;
        buffers[0].iov_base = header.head(sizeof(Request));
        buffers[0].iov_len = sizeof(Request);
        buffers[1].iov_base = (void*) requestBuffer.head(requestBuffer.length());
        buffers[1].iov_len = requestBuffer.length();

        if (writev(socket_, buffers, 2) == -1)
        {
            throw Error<SystemError>("Write failed");
        }

        header.reset();
        buffers[0].iov_base = header.tail(sizeof(Reply));
        buffers[0].iov_len = sizeof(Reply);
        buffers[1].iov_base = replyBuffer.tail(0);
        buffers[1].iov_len = replyBuffer.size();

        if (readv(socket_, buffers, 2) == -1)
        {
            throw Error<SystemError>("Read failed");
        }

        header >> reply;
        if (reply.dataLength > replyBuffer.size())
        {
            Buffer::size_type offset = replyBuffer.size();

            replyBuffer.reserve(reply.dataLength);
            if (read(socket_, ((char*) replyBuffer) + offset,
                    reply.dataLength - offset) == -1)
            {
                throw Error<SystemError>("Read failed");
            }
        }

        replyBuffer.move_tail(reply.dataLength);
        if (reply.result != 0)
        {
            throw(std::runtime_error(string(replyBuffer,
                            replyBuffer.length())));
        }
    }
};
