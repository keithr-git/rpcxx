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
#ifndef RPCXX_BUFFER_HPP
#define RPCXX_BUFFER_HPP

#include <algorithm>
#include <stdexcept>

#include <rpc++/assert.hpp>
#include <rpc++/marshal.hpp>

namespace RPCXX
{
    class Buffer
    {
    public:
        typedef unsigned int            size_type;

    private:
        char*           data_;
        size_type       size_;
        char*           head_;
        char*           tail_;
        bool            allocated_;

        void resize(size_type new_size);

    public:
        Buffer();
        Buffer(char* data, size_type size, size_type length = 0);
        Buffer(const char* str);

        ~Buffer()
        {
            release();
        }

        void release()
        {
            if (allocated_)
            {
                delete [] data_;
            }
        }

        void reset()
        {
            head_ = data_;
            tail_ = data_;
        }

        void reassign(char* data, size_type size, size_type length = 0,
                bool allocated = false);

        size_type size() const
        {
            return size_;
        }

        size_type length() const
        {
            return tail_ - data_;
        }

        void reserve(size_type new_size)
        {
            if (new_size > size_)
            {
                resize(new_size);
            }
        }

        operator char*()
        {
            return head_;
        }

        operator const char*() const
        {
            return head_;
        }

        void move_head(size_type bytes)
        {
            RPCXX_ASSERT((head_ + bytes) <= tail_,
                    std::range_error("Buffer out of data"));
            head_ += bytes;
        }

        void move_tail(size_type bytes)
        {
            RPCXX_ASSERT((length() + bytes) <= size_,
                    std::range_error("Buffer overflow"));
            tail_ += bytes;
        }

        char* head(size_type size);

        const char* head(size_type size) const
        {
            return const_cast<Buffer*>(this)->head(size);
        }

        template<typename T>
        T& head()
        {
            return *(reinterpret_cast<T*>(head(sizeof(T))));
        }

        template<typename T>
        const T& head() const
        {
            return *(reinterpret_cast<const T*>(head(sizeof(T))));
        }

        char* tail(size_type size);

        const char* tail(size_type size) const
        {
            return const_cast<Buffer*>(this)->tail(size);
        }

        template<typename T>
        T& tail()
        {
            return *(reinterpret_cast<T*>(tail(sizeof(T))));
        }

        template<typename T>
        const T& tail() const
        {
            return *(reinterpret_cast<const T*>(tail(sizeof(T))));
        }

        void insert(const char* buf, size_type size);

        void extract(char* buf, size_type size);
    };

    namespace marshal
    {
        template<>
        uint32_t net_size<Buffer>(const Buffer& buffer)
        {
            return buffer.size() + sizeof(Buffer::size_type);
        }
    };

    template<typename T>
    Buffer& operator<<(Buffer& buffer, const T& value)
    {
        buffer.tail<T>() = value;
    }

    template<typename T>
    Buffer& operator>>(Buffer& buffer, T& value)
    {
        value = buffer.head<T>();
    }

    struct NetBuffer : public Buffer
    {
        NetBuffer()
            : Buffer()
        { }

        NetBuffer(char* data, unsigned int size, unsigned int length = 0)
            : Buffer(data, size, length)
        { }
    };

    template<typename T>
    NetBuffer& operator<<(NetBuffer& buffer, const T& value)
    {
        marshal::to_net(value,
                buffer.tail<typename marshal::net_type<T>::type>());
        return buffer;
    }

    template<typename T>
    NetBuffer& operator>>(NetBuffer& buffer, T& value)
    {
        marshal::from_net(value,
                buffer.head<const typename marshal::net_type<T>::type>());
        return buffer;
    }

    static inline NetBuffer& operator<<(NetBuffer& buffer, const Buffer& value)
    {
        buffer << value.length();
        buffer.insert((const char*) value, value.length());
    }

    static inline NetBuffer& operator>>(NetBuffer& buffer, Buffer& value)
    {
        Buffer::size_type size;

        buffer >> size;
        value.reassign(buffer.head(size), size);
    }

    struct RequestBuffer : public NetBuffer
    {
        RequestBuffer()
            : NetBuffer()
        { }

        RequestBuffer(char* data, unsigned int size, unsigned int length = 0)
            : NetBuffer(data, size, length)
        { }
    };

    template<typename T>
    static RequestBuffer& operator<<(RequestBuffer& buffer, const T& value)
    {
        static_cast<NetBuffer&>(buffer) << value;
        return buffer;
    }

    template<typename T>
    static RequestBuffer& operator>>(RequestBuffer& buffer, T& value)
    {
        static_cast<NetBuffer&>(buffer) >> value;
        return buffer;
    }

    struct ReplyBuffer : public NetBuffer
    {
        ReplyBuffer()
            : NetBuffer()
        { }

        ReplyBuffer(char* data, unsigned int size, unsigned int length = 0)
            : NetBuffer(data, size, length)
        { }
    };

    template<typename T>
    static ReplyBuffer& operator<<(ReplyBuffer& buffer, const T& value)
    {
        static_cast<NetBuffer&>(buffer) << value;
        return buffer;
    }

    template<typename T>
    static ReplyBuffer& operator>>(ReplyBuffer& buffer, T& value)
    {
        static_cast<NetBuffer&>(buffer) >> value;
        return buffer;
    }
};

#endif /* RPCXX_BUFFER_HPP */
