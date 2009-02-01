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
#ifndef RPCXX_CLIENT_HPP
#define RPCXX_CLIENT_HPP

#include <boost/preprocessor.hpp>

#include <rpc++/argument.hpp>
#include <rpc++/buffer.hpp>
#include <rpc++/macros.hpp>
#include <rpc++/marshal.hpp>
#include <rpc++/socket.hpp>

namespace RPCXX
{
    class Client
    {
        TCPClient       socket_;

    public:
        Client(const char * server, int port);

        void send(int id, const RequestBuffer& requestBuffer,
                ReplyBuffer& replyBuffer);
    };

    typedef Client Connection;

    class Protocol
    {
        Client&         client_;

    protected:
        Protocol(Client& client)
            : client_(client)
        { }

    public:
        Client& client()
        {
            return client_;
        }
    };

    class function_base
    {
        Protocol&               protocol_;
        int                     function_number_;

    public:
        function_base(Protocol* protocol, int function_number)
            : protocol_(*protocol), function_number_(function_number)
        { }

        Protocol& protocol() const
        {
            return protocol_;
        }

        int function_number() const
        {
            return function_number_;
        }
    };

#define RPCXX_OPERATOR(z, n, has_return) \
        BOOST_PP_IF(has_return, Ret, void) \
        operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, T, arg)) \
        { \
            BOOST_PP_EXPR_IF(has_return, Ret return_value;) \
            BOOST_PP_EXPR_IF(has_return, out<Ret> ret(return_value);) \
            BOOST_PP_IF(n, \
                char requestData[BOOST_PP_REPEAT(n, RPCXX_SIZE, in_size)]; \
                RequestBuffer requestBuffer(requestData, \
                    sizeof(requestData)), \
                RequestBuffer requestBuffer); \
            BOOST_PP_IF(BOOST_PP_OR(n, has_return), \
                char replyData[BOOST_PP_EXPR_IF(has_return, \
                            ret.reserve() BOOST_PP_EXPR_IF(n, +)) \
                        BOOST_PP_REPEAT(n, RPCXX_SIZE, reserve)]; \
                    ReplyBuffer replyBuffer(replyData, sizeof(replyData)), \
                    ReplyBuffer replyBuffer); \
            BOOST_PP_REPEAT(n, RPCXX_STATEMENT, requestBuffer <<) \
            protocol().client().send(function_number(), requestBuffer, \
                    replyBuffer); \
            BOOST_PP_EXPR_IF(has_return, replyBuffer >> ret;) \
            BOOST_PP_REPEAT(n, RPCXX_STATEMENT, replyBuffer >>) \
            BOOST_PP_EXPR_IF(has_return, return return_value;) \
        }

    template< typename Ret,
        BOOST_PP_ENUM(RPCXX_MAX_ARGS, RPCXX_TEMPLATE_PARAM, = out<void>) >
    class function;

    template<
        BOOST_PP_ENUM(RPCXX_MAX_ARGS, RPCXX_TEMPLATE_PARAM, BOOST_PP_EMPTY()) >
    class function<void, BOOST_PP_ENUM_PARAMS(RPCXX_MAX_ARGS, T)>
        : public function_base
    {
    protected:
        function(Protocol* protocol, int function_number)
            : function_base(protocol, function_number)
        { }

    public:
        BOOST_PP_REPEAT(BOOST_PP_ADD(RPCXX_MAX_ARGS, 1), RPCXX_OPERATOR, 0)
    };

    template< typename Ret,
        BOOST_PP_ENUM(RPCXX_MAX_ARGS, RPCXX_TEMPLATE_PARAM, BOOST_PP_EMPTY()) >
    class function
        : public function_base
    {
    protected:
        function(Protocol* protocol, int function_number)
            : function_base(protocol, function_number)
        { }

    public:
        BOOST_PP_REPEAT(BOOST_PP_ADD(RPCXX_MAX_ARGS, 1), RPCXX_OPERATOR, 1)
    };
};

#define RPCXX_METHOD(NUM, Ret, Name, ...) \
    class Name : public RPCXX::function<Ret, ## __VA_ARGS__> \
    { \
    public: \
        Name(RPCXX::Protocol* protocol) \
            : RPCXX::function<Ret, ## __VA_ARGS__>(protocol, NUM) \
        { } \
    }; \
    Name Name

#endif /* RPCXX_CLIENT_HPP */
