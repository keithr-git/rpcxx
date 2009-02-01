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
#include <algorithm>

#include <string.h>

#include <rpc++/buffer.hpp>

namespace RPCXX
{
    Buffer::Buffer()
        : data_(0), size_(0), head_(0), tail_(0), allocated_(false)
    { }

    Buffer::Buffer(char* data, size_type size, size_type length)
        : data_(data), size_(size), head_(data_), tail_(data_),
          allocated_(false)
    { }

    Buffer::Buffer(const char* str)
        : data_((char*) str), size_(strlen(str) + 1), head_(data_),
          tail_(data_), allocated_(false)
    { }

    void Buffer::resize(size_type new_size)
    {
        char* new_data = new char[new_size];
        size_type offset = head_ - data_;
        size_type length = std::min((size_type)(tail_ - data_), new_size);

        release();
        data_ = new_data;
        size_ = new_size;
        head_ = data_ + offset;
        tail_ = data_ + length;
        allocated_ = true;
        memcpy(new_data, data_, length);
    }

    void Buffer::reassign(char* data, size_type size, size_type length,
            bool allocated)
    {
        release();
        data_ = data;
        size_ = size;
        head_ = data_;
        allocated_ = allocated;

        if (length)
        {
            tail_ = data_ + length;
        }
        else
        {
            tail_ = data_ + size_;
        }
    }

    char* Buffer::head(size_type size)
    {
        char* addr = head_;

        move_head(size);
        return addr;
    }

    char* Buffer::tail(size_type size)
    {
        char *addr = tail_;

        move_tail(size);
        return addr;
    }

    void Buffer::insert(const char* buf, size_type size)
    {
        char* dest = tail(size);

        memcpy(dest, buf, size);
    }

    void Buffer::extract(char* buf, size_type size)
    {
        char* src = head(size);

        memcpy(buf, src, size);
    }
};
