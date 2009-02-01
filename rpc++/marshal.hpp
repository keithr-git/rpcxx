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
#ifndef RPCXX_MARSHAL_HPP
#define RPCXX_MARSHAL_HPP

#include <rpc++/integer.hpp>

namespace RPCXX
{
    namespace marshal
    {
        template<typename T>
        struct net_type
        {
            typedef T          type;
        };

        template<typename T>
        static uint32_t net_size(const T& value)
        {
            return sizeof(typename net_type<T>::type);
        }
    }; // namespace marshal
}; // namespace RPCXX

#endif /* RPCXX_MARSHAL_HPP */
