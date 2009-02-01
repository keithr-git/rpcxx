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
#include <boost/preprocessor.hpp>

namespace RPCXX
{
#define RPCXX_MAX_ARGS   4

#define RPCXX_TEMPLATE_PARAM(z, n, unused) \
    typename T ## n = out<void>
#define RPCXX_SIZE(z, n, method) \
        BOOST_PP_IF(n, +, BOOST_PP_EMPTY()) arg ## n.method() 
#define RPCXX_STATEMENT(z, n, statement) \
        statement arg ## n;

#define RPCXX_operator(z, n, has_return) \
        BOOST_PP_IF(has_return, Ret, void) \
        operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, T, arg)) \
        { \
            BOOST_PP_IF(has_return, Ret return_value; out<Ret> ret;, \
                    BOOST_PP_EMPTY()) \
            BOOST_PP_IF(n, \
                char requestData[BOOST_PP_REPEAT(n, RPCXX_SIZE, \
                    in_size)]; \
                RequestBuffer requestBuffer(requestData, \
                    sizeof(requestData)), \
                RequestBuffer requestBuffer); \
            BOOST_PP_IF(BOOST_PP_OR(n, has_return), \
                char replyData[BOOST_PP_IF(has_return, \
                            ret.reserve() BOOST_PP_IF(n, +, BOOST_PP_EMPTY()), \
                            BOOST_PP_EMPTY()) \
                        BOOST_PP_REPEAT(n, RPCXX_SIZE, reserve)]; \
                    ReplyBuffer replyBuffer(replyData, sizeof(replyData)), \
                    ReplyBuffer replyBuffer); \
            BOOST_PP_REPEAT(n, RPCXX_STATEMENT, requestBuffer <<) \
            protocol().client().send(function_number(), requestBuffer, \
                    replyBuffer); \
            BOOST_PP_IF(has_return, replyBuffer >> ret;, BOOST_PP_EMPTY()) \
            BOOST_PP_REPEAT(n, RPCXX_STATEMENT, replyBuffer >>) \
            BOOST_PP_IF(has_return, return return_value;, BOOST_PP_EMPTY()) \
        }

    template< typename Ret,
        BOOST_PP_ENUM(RPCXX_MAX_ARGS, RPCXX_TEMPLATE_PARAM, 0) >
    class function
        : public function_base
    {
    protected:
        function(Protocol* protocol, int function_number)
            : function_base(protocol, function_number)
        { }

    public:
        }
        BOOST_PP_REPEAT(RPCXX_MAX_ARGS, RPCXX_operator, 1)
    };

    template< BOOST_PP_ENUM(RPCXX_MAX_ARGS, RPCXX_TEMPLATE_PARAM, 0) >
    class function<void, BOOST_PP_ENUM_PARAMS(RPCXX_MAX_ARGS, T)>
        : public function_base
    {
    protected:
        function(Protocol* protocol, int function_number)
            : function_base(protocol, function_number)
        { }

    public:
        BOOST_PP_REPEAT(RPCXX_MAX_ARGS, RPCXX_operator, 0)
    };
};
