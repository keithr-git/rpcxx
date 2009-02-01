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
#include <sys/stat.h>

#include <stdint.h>

class Test2 : public RPCXX::Protocol
{
public:
    RPCXX_METHOD(1, int, f1, RPCXX::in<int>, RPCXX::out<int>);

    // RPCXX_METHOD(2, void, f2, RPCXX::in<int>, RPCXX::in_out<int>);

    Test2(RPCXX::Connection& c)
        : RPCXX::Protocol(c), f1(this) // , f2(this)
    { }
};
