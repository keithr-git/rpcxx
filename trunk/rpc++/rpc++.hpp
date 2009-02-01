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
#ifndef RPCXX_RPCXX_HPP
#define RPCXX_RPCXX_HPP

#include <stdint.h>

#include <rpc++/buffer.hpp>
#include <rpc++/marshal.hpp>

namespace RPCXX
{
    typedef uint32_t            size_type;

    struct Request
    {
        uint32_t                id;
        uint32_t                function;
        uint32_t                dataLength;
    };

    struct Reply
    {
        uint32_t                id;
        int32_t                 result;
        uint32_t                dataLength;
    };

    static NetBuffer& operator>>(NetBuffer& buffer, Request& request)
    {
        const Request& net_request = buffer.head<Request>();

        marshal::from_net(request.id, net_request.id);
        marshal::from_net(request.function, net_request.function);
        marshal::from_net(request.dataLength, net_request.dataLength);

        return buffer;
    }

    static NetBuffer& operator<<(NetBuffer& buffer, const Request& request)
    {
        Request& net_request = buffer.tail<Request>();

        marshal::to_net(request.id, net_request.id);
        marshal::to_net(request.function, net_request.function);
        marshal::to_net(request.dataLength, net_request.dataLength);

        return buffer;
    }

    static NetBuffer& operator>>(NetBuffer& buffer, Reply& reply)
    {
        const Reply& net_reply = buffer.head<Reply>();

        marshal::from_net(reply.id, net_reply.id);
        marshal::from_net(reply.result, net_reply.result);
        marshal::from_net(reply.dataLength, net_reply.dataLength);

        return buffer;
    }

    static NetBuffer& operator<<(NetBuffer& buffer, const Reply& reply)
    {
        Reply& net_reply = buffer.tail<Reply>();

        marshal::to_net(reply.id, net_reply.id);
        marshal::to_net(reply.result, net_reply.result);
        marshal::to_net(reply.dataLength, net_reply.dataLength);

        return buffer;
    }
};

#endif /* RPCXX_RPCXX_HPP */
