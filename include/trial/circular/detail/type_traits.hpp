#ifndef TRIAL_CIRCULAR_DETAIL_TYPE_TRAITS_HPP
#define TRIAL_CIRCULAR_DETAIL_TYPE_TRAITS_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <type_traits>

namespace trial
{
namespace circular
{
namespace detail
{

#if __cplusplus >= 201703L

template <typename T>
using is_nothrow_swappable = std::is_nothrow_swappable<T>;

#else

template <typename T, typename = void>
struct is_nothrow_swappable
    : public std::false_type
{
};

// See P0185R1

template <typename T>
struct is_nothrow_swappable<T,
                            std::enable_if<std::is_nothrow_move_constructible<T>::value &&
                                           noexcept(std::swap(std::declval<T&>(), std::declval<T&>()))>>
    : public std::true_type
{
};

#endif

} // namespace detail
} // namespace circular
} // namespace trial

#endif // TRIAL_CIRCULAR_DETAIL_TYPE_TRAITS_HPP
