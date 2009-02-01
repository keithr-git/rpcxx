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
#ifndef RPCXX_ARGUMENT_HPP
#define RPCXX_ARGUMENT_HPP

#include <stdint.h>
#include <string.h>

#include <rpc++/buffer.hpp>
#include <rpc++/marshal.hpp>

namespace RPCXX
{
    //
    // Input arguments are generally passed into implementation
    // functions by const reference.  For scalar POD types, that's
    // cumbersome and unnecessary.  This replaces that with
    // pass-by-value semantics for POD types.
    //
    template<typename T>
    struct in_argument
    {
        typedef const T&        type;
    };

    template<>
    struct in_argument<int8_t>
    {
        typedef int8_t          type;
    };

    template<>
    struct in_argument<uint8_t>
    {
        typedef uint8_t         type;
    };

    template<>
    struct in_argument<int16_t>
    {
        typedef int16_t         type;
    };

    template<>
    struct in_argument<uint16_t>
    {
        typedef uint16_t        type;
    };

    template<>
    struct in_argument<int32_t>
    {
        typedef int32_t         type;
    };

    template<>
    struct in_argument<uint32_t>
    {
        typedef uint32_t        type;
    };

    template<>
    struct in_argument<int64_t>
    {
        typedef int64_t         type;
    };

    template<>
    struct in_argument<uint64_t>
    {
        typedef uint64_t        type;
    };

    template<typename T>
    class argument
    {
        T&                      value_;

    protected:
        argument(T& value)
            : value_(value)
        { }

    public:
        typedef T               data_type;
        typedef T&              argument_type;

        enum { reserve_size = sizeof(typename marshal::net_type<T>::type) };

        static unsigned int net_size(const T& value)
        {
            return marshal::net_size(value);
        }

        operator T&()
        {
            return value_;
        }

        operator const T&() const
        {
            return value_;
        }

        argument& operator=(const T& value)
        {
            value_ = value;
        }
    };

    template<>
    class argument<void>
    {
    public:
        typedef char            data_type;

        enum { reserve_size = 0 };
    };

    template<typename T>
    class in : public argument<const T>
    {
    public:
        typedef T               data_type;

        typedef typename in_argument<T>::type
                                argument_type;

        in(const T& value)
            : argument<const T>(value)
        { }

        unsigned int in_size() const
        {
            return marshal::net_size(*this);
        }

        unsigned int out_size() const
        {
            return 0;
        }

        unsigned int reserve() const
        {
            return 0;
        }
    };

    template<typename T, int Reserve = argument<T>::reserve_size>
    class out : public argument<T>
    {
    public:
        out(T& value)
            : argument<T>(value)
        { }

        unsigned int in_size() const
        {
            return 0;
        }

        unsigned int out_size() const
        {
            return marshal::net_size(*this);
        }

        unsigned int reserve() const
        {
            return Reserve;
        }

        out& operator=(const T& value)
        {
            static_cast<T&>(*this) = value;
        }
    };

    template<typename T, int Reserve = sizeof(T)>
    class in_out : public argument<T>
    {
    public:
        in_out(T& value)
            : argument<T>(value)
        { }

        unsigned int in_size() const
        {
            return marshal::net_size(*this);
        }

        unsigned int out_size() const
        {
            return marshal::net_size(*this);
        }

        unsigned int reserve() const
        {
            return Reserve;
        }

        in_out& operator=(const T& value)
        {
            static_cast<T&>(*this) = value;
        }
    };

    template<typename T>
    static RequestBuffer& operator>>(RequestBuffer& buffer, in<T>& value)
    {
        //
        // This const_cast could theoretically overwrite the
        // client's value for an in-only parameter.  But it
        // is only ever used on the server side, to write the
        // copy of the input parameter from the client's
        // request.
        //
        buffer >> const_cast<T&>(static_cast<const T&>(value));
        return buffer;
    }

    template<typename T>
    static RequestBuffer& operator>>(RequestBuffer& buffer, out<T>& value)
    {
        return buffer;
    }

    template<typename T>
    static RequestBuffer& operator>>(RequestBuffer& buffer, in_out<T>& value)
    {
        buffer >> static_cast<T&>(value);
        return buffer;
    }

    template<typename T>
    static RequestBuffer& operator<<(RequestBuffer& buffer, const in<T>& value)
    {
        buffer << static_cast<const T&>(value);
        return buffer;
    }

    template<typename T>
    static RequestBuffer& operator<<(RequestBuffer& buffer,
            const out<T>& value)
    {
        return buffer;
    }

    template<typename T>
    static RequestBuffer& operator<<(RequestBuffer& buffer,
            const in_out<T>& value)
    {
        buffer << static_cast<T>(value);
        return buffer;
    }

    template<typename T>
    static ReplyBuffer& operator>>(ReplyBuffer& buffer, in<T>& value)
    {
        return buffer;
    }

    template<typename T>
    static ReplyBuffer& operator>>(ReplyBuffer& buffer, out<T>& value)
    {
        buffer >> static_cast<T&>(value);
        return buffer;
    }

    template<typename T>
    static ReplyBuffer& operator>>(ReplyBuffer& buffer, in_out<T>& value)
    {
        buffer >> static_cast<T&>(value);
        return buffer;
    }

    template<typename T>
    static ReplyBuffer& operator<<(ReplyBuffer& buffer, const in<T>& value)
    {
        return buffer;
    }

    template<typename T>
    static ReplyBuffer& operator<<(ReplyBuffer& buffer, const out<T>& value)
    {
        buffer << static_cast<const T&>(value);
        return buffer;
    }

    template<typename T>
    static ReplyBuffer& operator<<(ReplyBuffer& buffer, const in_out<T>& value)
    {
        buffer << static_cast<const T&>(value);
        return buffer;
    }

    template<int Reserve>
    class out<void, Reserve>
    {
    public:
        typedef char            data_type;

        out()
        { }

        out(char&)
        { }

        unsigned int in_size() const
        {
            return 0;
        }

        unsigned int out_size() const
        {
            return 0;
        }

        unsigned int reserve() const
        {
            return 0;
        }

        static out null_arg;
    };

    template<>
    ReplyBuffer& operator>>(ReplyBuffer& buffer, out<void>& value)
    {
        return buffer;
    }

    template<>
    ReplyBuffer& operator<<(ReplyBuffer& buffer, const out<void>& value)
    {
        return buffer;
    }

    //
    // Specialization for const char* input arguments - it sends the
    // length+1 and the string, including the trailing NUL byte.
    // (Including the NUL byte allows the server to use the string
    // inside its receive buffer, without having to copy it elsewhere
    // just to terminate it).
    //
    template<>
    class in<const char*> : public argument<const char*>
    {
        mutable uint32_t                size_;

    public:
        typedef const char*             argument_type;

        in(const char* const& value)
            : argument<const char*>(const_cast<const char*&>(value)),
              size_(-1)
        { }

        unsigned int size() const
        {
            if (size_ == -1)
            {
                size_ = strlen((const char*)(*this)) + 1;
            }

            return size_;
        }

        unsigned int in_size() const
        {
            return size() + sizeof(uint32_t);
        }

        unsigned int out_size() const
        {
            return 0;
        }

        unsigned int reserve() const
        {
            return 0;
        }

        //
        // I haven't found a way to make this private and friend the
        // operator>> function, like I did for the generic in<> template.
        //
        in& operator=(const char* value)
        {
            const_cast<const char*&>(static_cast<const char*&>(*this)) =
                value;
        }
    };

    template<>
    RequestBuffer& operator<<(RequestBuffer& buffer,
            const in<const char*>& value)
    {
        buffer << value.size();
        buffer.insert(value, value.size());

        return buffer;
    }

    template<>
    RequestBuffer& operator>>(RequestBuffer& buffer,
            in<const char*>& value)
    {
        Buffer::size_type size;

        buffer >> size;
        value = buffer.head(size);
        return buffer;
    }
};

#endif /* RPCXX_ARGUMENT_HPP */
