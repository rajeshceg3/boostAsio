#line 1 "include/boost/asio/append.hpp"
//
// append.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_APPEND_HPP
#define BOOST_ASIO_APPEND_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>

#if (defined(BOOST_ASIO_HAS_STD_TUPLE) \
    && defined(BOOST_ASIO_HAS_VARIADIC_TEMPLATES)) \
  || defined(GENERATING_DOCUMENTATION)

#include <tuple>
#include <boost/asio/detail/type_traits.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {

/// Completion token type used to specify that the completion handler
/// arguments should be passed additional values after the results of the
/// operation.
template <typename CompletionToken, typename... Values>
class append_t
{
public:
  /// Constructor.
  template <typename T, typename... V>
  BOOST_ASIO_CONSTEXPR explicit append_t(
      BOOST_ASIO_MOVE_ARG(T) completion_token,
      BOOST_ASIO_MOVE_ARG(V)... values)
    : token_(BOOST_ASIO_MOVE_CAST(T)(completion_token)),
      values_(BOOST_ASIO_MOVE_CAST(V)(values)...)
  {
  }

//private:
  CompletionToken token_;
  std::tuple<Values...> values_;
};

/// Completion token type used to specify that the completion handler
/// arguments should be passed additional values after the results of the
/// operation.
template <typename CompletionToken, typename... Values>
BOOST_ASIO_NODISCARD inline BOOST_ASIO_CONSTEXPR append_t<
  typename decay<CompletionToken>::type, typename decay<Values>::type...>
append(BOOST_ASIO_MOVE_ARG(CompletionToken) completion_token,
    BOOST_ASIO_MOVE_ARG(Values)... values)
{
  return append_t<
    typename decay<CompletionToken>::type, typename decay<Values>::type...>(
      BOOST_ASIO_MOVE_CAST(CompletionToken)(completion_token),
      BOOST_ASIO_MOVE_CAST(Values)(values)...);
}

} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#include <boost/asio/impl/append.hpp>

#endif // (defined(BOOST_ASIO_HAS_STD_TUPLE)
       //     && defined(BOOST_ASIO_HAS_VARIADIC_TEMPLATES))
       //   || defined(GENERATING_DOCUMENTATION)

#endif // BOOST_ASIO_APPEND_HPP
