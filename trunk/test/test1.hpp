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
#ifndef TEST1_HPP
#define TEST1_HPP

#include <sys/stat.h>

#include <rpc++/marshal.hpp>
#include <rpc++/buffer.hpp>
#include <rpc++/types.hpp>

class Test1 : public RPCXX::Protocol
{
public:
    RPCXX_METHOD(1, int32_t, f1, RPCXX::in<int32_t>, RPCXX::out<int32_t>);

    RPCXX_METHOD(2, void, f2, RPCXX::in<int32_t>, RPCXX::in_out<int32_t>);

    RPCXX_METHOD(3, int32_t, f3, RPCXX::in<int32_t>);

    RPCXX_METHOD(4, void, stat, RPCXX::in<const char*>,
            RPCXX::out<struct ::stat>);

    Test1(RPCXX::Connection& c)
        : RPCXX::Protocol(c), f1(this), f2(this), f3(this), stat(this)
    { }
};

#endif /* TEST1_HPP */
