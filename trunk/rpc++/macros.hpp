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
#ifndef RPCXX_MACROS_HPP
#define RPCXX_MACROS_HPP

#include <boost/preprocessor.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>

#ifndef RPCXX_MAX_ARGS
#define RPCXX_MAX_ARGS   8
#endif

//
// A set of macros to convert the arguments to a variadic
// macro into a BOOST_PP_SET, which can then be iterated
// over.  The trick is to pass the argument list as a
// single argument wrapped in parentheses, since the boost
// macros only expect one argument, and then strip off
// the parentheses when passing it to FIRST_ARG() and
// OTHER_ARGS().  That's why the calls are of the form:
//
// RPC++_VARARGS_FIRST_ARG args
//
// rather than:
//
// RPC++_VARARGS_FIRST_ARG(args)
//
#define RPCXX_VARARGS_FIRST_ARG(a, ...)       a
#define RPCXX_VARARGS_OTHER_ARGS(a, ...)      (__VA_ARGS__)

#define RPCXX_VARARGS_TO_SEQ_PRED(r, args) \
    BOOST_PP_NOT(BOOST_PP_IS_EMPTY(RPCXX_VARARGS_FIRST_ARG args))

#define RPCXX_VARARGS_TO_SEQ_OP(r, args) \
    RPCXX_VARARGS_OTHER_ARGS args

#define RPCXX_VARARGS_TO_SEQ_MACRO(r, args) \
    (RPCXX_VARARGS_FIRST_ARG args)

#define RPCXX_VARARGS_TO_SEQ(...) \
    BOOST_PP_FOR((__VA_ARGS__,), RPCXX_VARARGS_TO_SEQ_PRED, \
            RPCXX_VARARGS_TO_SEQ_OP, RPCXX_VARARGS_TO_SEQ_MACRO)

//
// Macros for use in generating the RPC++ template classes.
//
#define RPCXX_TEMPLATE_PARAM(z, n, default) \
        typename T ## n default
#define RPCXX_SIZE(z, n, method) \
        BOOST_PP_IF(n, +, BOOST_PP_EMPTY()) arg ## n.method() 
#define RPCXX_STATEMENT(z, n, statement) \
        statement arg ## n;

#endif /* RPCXX_MACROS_HPP */
