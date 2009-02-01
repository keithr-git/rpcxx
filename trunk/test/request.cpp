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
#include <cstdio>

#include "rpc++.hpp"

using std::printf;

int
main()
{
    RPCXX::Request request = { 1, 2, 3 };
    RPCXX::Request net_request;

    RPCXX::marshal::to_net(request, net_request);
    printf("%08x %08x\n", request.function, net_request.function);
    RPCXX::marshal::from_net(request, net_request);
    printf("%08x %08x\n", request.function, net_request.function);
    return 0;
}
