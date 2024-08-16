#line 1 "include/boost/asio/traits/query_free.hpp"
//
// traits/query_free.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_TRAITS_QUERY_FREE_HPP
#define BOOST_ASIO_TRAITS_QUERY_FREE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/asio/detail/type_traits.hpp>

#if defined(BOOST_ASIO_HAS_DECLTYPE) \
  && defined(BOOST_ASIO_HAS_NOEXCEPT) \
  && defined(BOOST_ASIO_HAS_WORKING_EXPRESSION_SFINAE)
# define BOOST_ASIO_HAS_DEDUCED_QUERY_FREE_TRAIT 1
#endif // defined(BOOST_ASIO_HAS_DECLTYPE)
       //   && defined(BOOST_ASIO_HAS_NOEXCEPT)
       //   && defined(BOOST_ASIO_HAS_WORKING_EXPRESSION_SFINAE)

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {
namespace traits {

template <typename T, typename Property, typename = void>
struct query_free_default;

template <typename T, typename Property, typename = void>
struct query_free;

} // namespace traits
namespace detail {

struct no_query_free
{
  BOOST_ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  BOOST_ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#if defined(BOOST_ASIO_HAS_DEDUCED_QUERY_FREE_TRAIT)

template <typename T, typename Property, typename = void>
struct query_free_trait : no_query_free
{
};

template <typename T, typename Property>
struct query_free_trait<T, Property,
  typename void_type<
    decltype(query(declval<T>(), declval<Property>()))
  >::type>
{
  BOOST_ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    query(declval<T>(), declval<Property>()));

  BOOST_ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    query(declval<T>(), declval<Property>())));
};

#else // defined(BOOST_ASIO_HAS_DEDUCED_QUERY_FREE_TRAIT)

template <typename T, typename Property, typename = void>
struct query_free_trait :
  conditional<
    is_same<T, typename decay<T>::type>::value
      && is_same<Property, typename decay<Property>::type>::value,
    no_query_free,
    traits::query_free<
      typename decay<T>::type,
      typename decay<Property>::type>
  >::type
{
};

#endif // defined(BOOST_ASIO_HAS_DEDUCED_QUERY_FREE_TRAIT)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename>
struct query_free_default :
  detail::query_free_trait<T, Property>
{
};

template <typename T, typename Property, typename>
struct query_free :
  query_free_default<T, Property>
{
};

} // namespace traits
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_TRAITS_QUERY_FREE_HPP
