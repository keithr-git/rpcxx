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

#include "marshal.hpp"

using std::printf;

typedef unsigned long int foo_t;

void send(const uint8_t value, uint64_t& net_value)
{
    RPCXX::marshal::to_net(value, net_value);
}

void send(const int8_t value, int64_t& net_value)
{
    RPCXX::marshal::to_net(value, net_value);
}

void send(const uint16_t value, uint64_t& net_value)
{
    RPCXX::marshal::to_net(value, net_value);
}

void send(const foo_t& value, uint64_t& net_value)
{
    RPCXX::marshal::to_net(value, net_value);
}

/*
void receive(foo_t& value, const uint64_t& net_value)
{
    RPCXX::marshal::from_net(value, net_value);
}
 */

void send(const int8_t& value, int32_t& net_value)
{
    RPCXX::marshal::to_net(value, net_value);
}

void send(const uint8_t& value, uint32_t& net_value)
{
    RPCXX::marshal::to_net(value, net_value);
}

void send(const int16_t& value, int32_t& net_value)
{
    RPCXX::marshal::to_net(value, net_value);
}

uint32_t send(const uint64_t& value)
{
    return htonll(value);
}

int
main()
{
    int16_t value = 0x8000;
    int8_t cvalue = 0x80;
    int32_t net_value;

    send(value, net_value);
    printf("%08x %08x\n", net_value, htonl((int32_t) value));
    printf("%08x %08x\n", (int32_t) value, ((int32_t) value) / (1 << 15));
    printf("%d %d\n", (int32_t) value, ((int32_t) value) / (1 << 15));
    send(cvalue, net_value);
    printf("%08x %08x\n", net_value, htonl((int32_t) cvalue));
}
