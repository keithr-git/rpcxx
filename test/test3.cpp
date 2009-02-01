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

#include "server.hpp"
#include "test2.hpp"

int Test2::f1::operator()(RPCXX::in<int> arg1, RPCXX::out<int> arg2)
{
    arg2 = arg1 * 2;
    return arg1 + 5;
}

/*
void Test2::f2::operator()(RPCXX::in<int> arg1, RPCXX::in_out<int> arg2)
{
    arg2 = arg1 + 42;
}
 */

int main()
{
    RPCXX::Server s(0);
    Test2 t(s);

    s.runLoop();
    return 0;
}
