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

//-----------------------------------------------------------------------------
// is_final
//-----------------------------------------------------------------------------

#if __cpp_lib_is_final >= 201402L

using std::is_final;

#else

template <typename T>
struct is_final : std::false_type {};

#endif

//-----------------------------------------------------------------------------
// is_nothrow_swappable
//-----------------------------------------------------------------------------

#if __cpp_lib_is_swappable >= 201603L

template <typename T>
using is_nothrow_swappable = std::is_nothrow_swappable<T>;

#else

// See P0185R1

using std::swap;

struct is_nothrow_swappable_tester
{
    template <typename T>
    static auto test(int) -> std::integral_constant<bool, noexcept(swap(std::declval<T&>(), std::declval<T&>()))>;

    template <typename T>
    static std::false_type test(...);
};

template <typename T>
struct is_nothrow_swappable
    : decltype(is_nothrow_swappable_tester::test<T>(0))
{
};

#endif

} // namespace detail
} // namespace circular
} // namespace trial

#endif // TRIAL_CIRCULAR_DETAIL_TYPE_TRAITS_HPP
