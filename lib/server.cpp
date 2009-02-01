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
#include <errno.h>
#include <string.h>

#include <cstdio>
#include <stdexcept>
#include <string>

#include <rpc++/rpc++.hpp>
#include <rpc++/errno.hpp>
#include <rpc++/exception.hpp>
#include <rpc++/server.hpp>

using std::fprintf;
using std::string;

namespace RPCXX
{
    Server::Server(int fd)
        : fd_(fd)
    { }

    void Server::runLoop()
    {
        char headerData[std::max(sizeof(Request), sizeof(Reply))];
        NetBuffer header(headerData, sizeof(headerData));
        char requestData[4096];
        RequestBuffer requestBuffer(requestData, sizeof(requestData));
        char replyData[4096];
        ReplyBuffer replyBuffer(replyData, sizeof(replyData));

        while (true)
        {
            struct iovec buffers[2];
            Request request;
            Reply reply;
            int length;

            buffers[0].iov_base = header.tail(sizeof(Reply));
            buffers[0].iov_len = sizeof(Reply);
            buffers[1].iov_base = requestBuffer.tail(0);
            buffers[1].iov_len = requestBuffer.size();

            if ((length = readv(fd_, buffers, 2)) <= 0)
            {
                if (length == 0)
                {
                    // EOF
                    break;
                }
                else
                {
                    throw std::runtime_error(string("Read error: ") +
                            strerror(errno));
                }
            }
            // fprintf(stderr, "read %d bytes of data\n", length); /* DeBuG */

            header >> request;
            // fprintf(stderr, "request: id %lu function %lu dataLength %lu\n", request.id, request.function, request.dataLength); /* DeBuG */

            if (request.dataLength > requestBuffer.size())
            {
                Buffer::size_type offset = requestBuffer.size();

                requestBuffer.reserve(request.dataLength);
                if (read(fd_, ((char*) requestBuffer) + offset,
                        request.dataLength - offset) == -1)
                {
                    throw(std::runtime_error(string("Read failed: ") +
                                strerror(errno)));
                }
            }

            requestBuffer.move_tail(request.dataLength);

            reply.id = request.id;

            try
            {
                // fprintf(stderr, "Calling function %d\n", request.function); /* DeBuG */
                functions_[request.function]->receive(requestBuffer, replyBuffer);
                reply.result = 0;
                reply.dataLength = replyBuffer.length();
            }
            catch (Exception& e)
            {
                if (e.is<SystemError>())
                {
                }
            }
            catch (std::runtime_error& e)
            {
                reply.result = 1;
                reply.dataLength = strlen(e.what());
                replyBuffer.insert(e.what(), strlen(e.what()));
            }

            // fprintf(stderr, "reply: id %lu result %lu dataLength %lu\n", reply.iid, reply.result, reply.dataLength); /* DeBuG */
            header.reset();
            header << reply;
            buffers[0].iov_base = header.head(sizeof(Request));
            buffers[0].iov_len = sizeof(Request);
            buffers[1].iov_base = (void*) replyBuffer.head(replyBuffer.length());
            buffers[1].iov_len = replyBuffer.length();

            if (writev(fd_, buffers, 2) == -1)
            {
                throw std::runtime_error(string("Write error: ") +
                        strerror(errno));
            }

            header.reset();
            requestBuffer.reset();
            replyBuffer.reset();
        }
    }

    void Server::addFunction(int id, function_base* function)
    {
        // fprintf(stderr, "addFunction(%d, %p)\n", id, function);
        if (functions_.size() <= id)
        {
            functions_.resize(id + 1, 0);
        }

        functions_[id] = function;
    }
};
