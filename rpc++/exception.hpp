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
#ifndef RPCXX_EXCEPTION_HPP
#define RPCXX_EXCEPTION_HPP

#include <stdint.h>

#include <stdexcept>

#define RPCXX_THROW(class, ...) \
        throw class(__FILE__, __LINE__, ## __VA_ARGS__)

namespace RPCXX
{
    template<typename ERROR>
    class ErrorType;

    class Exception
        : public std::runtime_error
    {
        const int*      error_type_;

    protected:
        Exception(const std::string& message, const int* error_type)
            : std::runtime_error(message), error_type_(error_type)
        { }

    public:
        template<typename OTHER_ERROR>
        bool is() const
        {
            return error_type_ == ErrorType<OTHER_ERROR>::Type();
        }
    };

    template<typename ERROR>
    class ErrorType
        : public Exception
    {
    protected:
        ErrorType(const std::string& message)
            : Exception(message, Type())
        { }

    public:
        static const int* Type()
        {
            static int value;

            return &value;
        }
    };

    template<typename ERROR>
    class ErrorCode
        : public ErrorType<ERROR>
    {
        int             error_code_;

    protected:
        ErrorCode(const std::string& message, int error_code)
            : ErrorType<ERROR>(message), error_code_(error_code)
        { }

    public:
        int error_code() const
        {
            return error_code_;
        }
    };

    template<typename ERROR>
    class Error
        : public ErrorType<ERROR>
    {
    public:
        Error(const std::string& message)
            : ErrorType<ERROR>(message)
        { }
    };
};

#endif /* RPCXX_EXCEPTION_HPP */
