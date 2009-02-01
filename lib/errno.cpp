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

#include <rpc++/errno.hpp>

namespace RPCXX
{
    Error<SystemError>::Error(const std::string& message)
        : ErrorCode<SystemError>(message + ": " + strerror(errno), errno)
    { }

    Error<SystemError>::Error(const std::string& message, int system_error)
        : ErrorCode<SystemError>(message + ": " + strerror(system_error), errno)
    { }

    Error<SystemError>::Error(int system_error)
        : ErrorCode<SystemError>(strerror(system_error), errno)
    { }
};
