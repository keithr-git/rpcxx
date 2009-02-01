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
#ifndef RPCXX_SERVER_HPP
#define RPCXX_SERVER_HPP

#include <rpc++/argument.hpp>
#include <rpc++/rpc++.hpp>
#include <rpc++/macros.hpp>
#include <rpc++/marshal.hpp>

#include <cstdio>
#include <vector>

namespace RPCXX
{
    class function_base;

    class Server
    {
        int                             fd_;
        std::vector<function_base*>     functions_;

    public:
        Server(int fd);

        void addFunction(int id, function_base* function);
        void runLoop();
    };

    typedef Server Connection;

    class Protocol
    {
        Server&         server_;

    protected:
        Protocol(Server& server)
            : server_(server)
        { }

    public:
        Server& server()
        {
            return server_;
        }
    };

    class function_base
    {
        Protocol&               protocol_;

    protected:
        function_base(Protocol* protocol, int function_number)
            : protocol_(*protocol)
        {
            protocol_.server().addFunction(function_number, this);
        }

        Protocol& protocol() const
        {
            return protocol_;
        }

    public:
        virtual void receive(RequestBuffer& requestBuffer,
                ReplyBuffer& replyBuffer) = 0;
    };
};

#define RPCXX_FOR_EACH_ARG(macro, ...) \
    BOOST_PP_SEQ_FOR_EACH_I(macro, 0, RPCXX_VARARGS_TO_SEQ(__VA_ARGS__))

#define RPCXX_OP_ARG(r, unused, i, type) \
    BOOST_PP_COMMA_IF(BOOST_PP_AND(BOOST_PP_NOT(BOOST_PP_IS_EMPTY(type)), \
                BOOST_PP_NOT_EQUAL(i, 0))) \
    BOOST_PP_EXPR_IIF(BOOST_PP_NOT(BOOST_PP_IS_EMPTY(type)), \
            type::argument_type)

#define RPCXX_RETURN_TYPE_void
#define RPCXX_RETURN_TYPE(Ret)   RPCXX_RETURN_TYPE_ ## Ret
#define RPCXX_IF_RETURN(Ret, code) \
    BOOST_PP_EXPR_IF(BOOST_PP_NOT(BOOST_PP_IS_EMPTY(RPCXX_RETURN_TYPE(Ret))), \
            code)

#define RPCXX_ARG_DECL(r, unused, i, type) \
    type::data_type value ## i; \
    type arg ## i(value ## i);

#define RPCXX_ARG_RECEIVE(r, unused, i, type)    requestBuffer >> arg ## i;
#define RPCXX_ARG_SEND(r, unused, i, type)       replyBuffer << arg ## i;

#define RPCXX_ARG_PASS(r, unused, i, type) \
    BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) \
    arg ## i

#define RPCXX_ARG_RESERVE(r, unused, i, type) \
    BOOST_PP_EXPR_IF(BOOST_PP_NOT_EQUAL(i, 0), +) \
    arg ## i.out_size()

#define RPCXX_NUM_ARGS(...) \
    BOOST_PP_SEQ_SIZE(RPCXX_VARARGS_TO_SEQ(__VA_ARGS__))

#define RPCXX_METHOD(NUM, Ret, Name, ...) \
    class Name : public RPCXX::function_base \
    { \
    public: \
        Name(RPCXX::Protocol* protocol) \
            : RPCXX::function_base(protocol, NUM) \
        { } \
        Ret operator()(RPCXX_FOR_EACH_ARG(RPCXX_OP_ARG, __VA_ARGS__)); \
        void receive(RPCXX::RequestBuffer& requestBuffer, \
                RPCXX::ReplyBuffer& replyBuffer) \
        { \
            RPCXX_IF_RETURN(Ret, RPCXX::out<Ret>::data_type return_value;) \
            RPCXX_IF_RETURN(Ret, RPCXX::out<Ret> ret(return_value);) \
            RPCXX_FOR_EACH_ARG(RPCXX_ARG_DECL, __VA_ARGS__) \
            RPCXX_FOR_EACH_ARG(RPCXX_ARG_RECEIVE, __VA_ARGS__) \
            RPCXX_IF_RETURN(Ret, ret =) \
            operator()(RPCXX_FOR_EACH_ARG(RPCXX_ARG_PASS, __VA_ARGS__)); \
            replyBuffer.reserve(RPCXX_IF_RETURN(Ret, ret.out_size() +) \
                    RPCXX_FOR_EACH_ARG(RPCXX_ARG_RESERVE, __VA_ARGS__)); \
            RPCXX_IF_RETURN(Ret, replyBuffer << ret;) \
            RPCXX_FOR_EACH_ARG(RPCXX_ARG_SEND, __VA_ARGS__) \
        } \
    }; \
    Name Name

#endif /* RPCXX_SERVER_HPP */
