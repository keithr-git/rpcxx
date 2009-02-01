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

#include <cstdio> /* DeBuG */
#include <string>
#include <errno.h>

#include <rpc++/errno.hpp>
#include <rpc++/exception.hpp>
#include <rpc++/rpc++.hpp>
#include <rpc++/server.hpp>
#include <rpc++/types.hpp>

#include "test1.hpp"

using std::fprintf; /* DeBuG */
using std::string;

int32_t Test1::f1::operator()(int i, int& j)
{
    fprintf(stderr, "f1(%d)\n", i); /* DeBuG */
    j = i * 2;
    return i + j;
}

void Test1::f2::operator()(int arg1, int& arg2)
{
    fprintf(stderr, "f2(%d, %d)\n", (int) arg1, (int) arg2); /* DeBuG */
    arg2 = arg1 * arg2;
}

int32_t Test1::f3::operator()(int arg1)
{
    fprintf(stderr, "f3(%d)\n", (int) arg1); /* DeBuG */
    return arg1 * 4;
}

void Test1::stat::operator()(const char* name, struct ::stat& info)
{
    fprintf(stderr, "stat(%s)\n", name); /* DeBuG */
    if (::stat(name, &info) == -1)
    {
        throw RPCXX::Error<RPCXX::SystemError>(string("Cannot stat ") + name);
    }
}

int
main()
{
    RPCXX::Server s(0);
    Test1 t(s);

    s.runLoop();
    return 0;
}
