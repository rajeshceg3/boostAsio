#line 1 "include/boost/asio/traits/submit_free.hpp"
//
// traits/submit_free.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_TRAITS_SUBMIT_FREE_HPP
#define BOOST_ASIO_TRAITS_SUBMIT_FREE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/asio/detail/type_traits.hpp>

#if defined(BOOST_ASIO_HAS_DECLTYPE) \
  && defined(BOOST_ASIO_HAS_NOEXCEPT) \
  && defined(BOOST_ASIO_HAS_WORKING_EXPRESSION_SFINAE)
# define BOOST_ASIO_HAS_DEDUCED_SUBMIT_FREE_TRAIT 1
#endif // defined(BOOST_ASIO_HAS_DECLTYPE)
       //   && defined(BOOST_ASIO_HAS_NOEXCEPT)
       //   && defined(BOOST_ASIO_HAS_WORKING_EXPRESSION_SFINAE)

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {
namespace traits {

template <typename S, typename R, typename = void>
struct submit_free_default;

template <typename S, typename R, typename = void>
struct submit_free;

} // namespace traits
namespace detail {

struct no_submit_free
{
  BOOST_ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  BOOST_ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#if defined(BOOST_ASIO_HAS_DEDUCED_SUBMIT_FREE_TRAIT)

template <typename S, typename R, typename = void>
struct submit_free_trait : no_submit_free
{
};

template <typename S, typename R>
struct submit_free_trait<S, R,
  typename void_type<
    decltype(submit(declval<S>(), declval<R>()))
  >::type>
{
  BOOST_ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    submit(declval<S>(), declval<R>()));

  BOOST_ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    submit(declval<S>(), declval<R>())));
};

#else // defined(BOOST_ASIO_HAS_DEDUCED_SUBMIT_FREE_TRAIT)

template <typename S, typename R, typename = void>
struct submit_free_trait :
  conditional<
    is_same<S, typename remove_reference<S>::type>::value
      && is_same<R, typename decay<R>::type>::value,
    typename conditional<
      is_same<S, typename add_const<S>::type>::value,
      no_submit_free,
      traits::submit_free<typename add_const<S>::type, R>
    >::type,
    traits::submit_free<
      typename remove_reference<S>::type,
      typename decay<R>::type>
  >::type
{
};

#endif // defined(BOOST_ASIO_HAS_DEDUCED_SUBMIT_FREE_TRAIT)

} // namespace detail
namespace traits {

template <typename S, typename R, typename>
struct submit_free_default :
  detail::submit_free_trait<S, R>
{
};

template <typename S, typename R, typename>
struct submit_free :
  submit_free_default<S, R>
{
};

} // namespace traits
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_TRAITS_SUBMIT_FREE_HPP
