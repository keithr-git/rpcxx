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

#include <sys/stat.h>

#include <rpc++/client.hpp>

#include "test1.hpp"

int main()
{
    RPCXX::Client c("localhost", 4242);
    Test1 t(c);
    int i = 42;
    int j = 0;
    int r = 0;
    struct stat info;

    std::printf("r: %d, i: %d, j: %d\n", r, i, j);
    r = t.f1(i, j);
    std::printf("r: %d, i: %d, j: %d\n", r, i, j);
    r = t.f1(3, j);
    std::printf("r: %d, i: %d, j: %d\n", r, i, j);
    t.f2(5, j);
    std::printf("r: %d, i: %d, j: %d\n", r, i, j);
    r = t.f3(j);
    std::printf("r: %d, i: %d, j: %d\n", r, i, j);
    t.stat(".", info);
    std::printf(".: %lu/%lu\n", (long) info.st_uid, (long) info.st_gid);

    return 0;
}
