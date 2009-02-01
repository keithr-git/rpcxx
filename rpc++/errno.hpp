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
#ifndef RPCXX_ERRNO_HPP
#define RPCXX_ERRNO_HPP

#include <rpc++/exception.hpp>

namespace RPCXX
{
    struct SystemError {};

    template<>
    class Error<SystemError>
        : public ErrorCode<SystemError>
    {
    public:
        Error(const std::string& message);
        Error(const std::string& message, int error_code);
        Error(int error_code);
    };
};

#endif /* RPCXX_ERRNO_HPP */
