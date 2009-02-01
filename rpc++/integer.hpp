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
#ifndef RPCXX_INTEGER_HPP
#define RPCXX_INTEGER_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <stdint.h>

#include <netinet/in.h>

#ifndef htonll
# include <endian.h>
# if __BYTE_ORDER == __BIG_ENDIAN
#  define htonll(x)     (x)
#  define ntohll(x)     (x)
# else
#  include <byteswap.h>
#  define htonll(x)     bswap_64(x)
#  define ntohll(x)     bswap_64(x)
# endif
#endif

namespace RPCXX
{
    namespace marshal
    {
#if __BYTE_ORDER == __BIG_ENDIAN
        template<typename Local, typename Net>
        static
        typename boost::enable_if_c<boost::is_integral<Local>::value &&
                                    boost::is_integral<Net>::value, void>::type
        to_net(const Local& value, Net& net_value)
        {
            net_value = value;
        }

        template<typename Local, typename Net>
        static
        typename boost::enable_if_c<boost::is_integral<Local>::value &&
                                    boost::is_integral<Net>::value, void>::type
        from_net(Local& value, const Net& net_value)
        {
            value = net_value;
        }
#else
        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        to_net(const T& value, uint8_t& net_value)
        {
            net_value = value;
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        to_net(const T& value, int8_t& net_value)
        {
            net_value = value;
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        to_net(const T& value, uint16_t& net_value)
        {
            net_value = htons((uint16_t) value);
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        to_net(const T& value, int16_t& net_value)
        {
            net_value = htons((int16_t) value);
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        to_net(const T& value, uint32_t& net_value)
        {
            net_value = htonl((uint32_t) value);
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        to_net(const T& value, int32_t& net_value)
        {
            net_value = htonl((int32_t) value);
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        to_net(const T& value, uint64_t& net_value)
        {
            if (sizeof(value)>= sizeof(uint64_t))
            {
                net_value = htonll((uint64_t) value);
            }
            else
            {
                uint32_t* ptr = reinterpret_cast<uint32_t*>(&net_value);

                ptr[0] = 0;
                ptr[1] = htonl((uint32_t) value);
            }
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        to_net(const T& value, int64_t& net_value)
        {
            if (sizeof(value)>= sizeof(int64_t))
            {
                net_value = htonll((int64_t) value);
            }
            else
            {
                int32_t* ptr = reinterpret_cast<int32_t*>(&net_value);

                ptr[0] = 0;
                ptr[1] = htonl((int32_t) value);
            }
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        from_net(T& value, const uint8_t& net_value)
        {
            value = net_value;
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        from_net(T& value, const int8_t& net_value)
        {
            value = net_value;
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        from_net(T& value, const uint16_t& net_value)
        {
            value = ntohs(net_value);
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        from_net(T& value, const int16_t& net_value)
        {
            value = ntohs(net_value);
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        from_net(T& value, const uint32_t& net_value)
        {
            value = ntohl(net_value);
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        from_net(T& value, const int32_t& net_value)
        {
            value = ntohl(net_value);
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        from_net(T& value, const uint64_t& net_value)
        {
            if (sizeof(value)>= sizeof(uint64_t))
            {
                value = ntohll(net_value);
            }
            else
            {
                const uint32_t* ptr =
                    reinterpret_cast<const uint32_t*>(&net_value);

                value = ntohl(ptr[1]);
            }
        }

        template<typename T>
        static typename boost::enable_if<boost::is_integral<T>, void>::type
        from_net(T& value, const int64_t& net_value)
        {
            if (sizeof(value)>= sizeof(int64_t))
            {
                net_value = ntohll(net_value);
            }
            else
            {
                const int32_t* ptr =
                    reinterpret_cast<const int32_t*>(&net_value);

                value = ntohl(ptr[1]);
            }
        }
#endif // __BYTE_ORDER != __BIG_ENDIAN
    }; // namespace marshal
}; // namespace RPCXX

#endif /* RPCXX_INTEGER_HPP */
