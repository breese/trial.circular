#ifndef TRIAL_DETAIL_FUNCTIONAL_HPP
#define TRIAL_DETAIL_FUNCTIONAL_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <limits>

namespace trial
{
namespace detail
{

template <typename T>
struct close_to
{
    constexpr explicit close_to(T relative_tolerance = std::numeric_limits<T>::epsilon(),
                                T absolute_tolerance = std::numeric_limits<T>::min()) noexcept
        : relative(relative_tolerance),
          absolute(absolute_tolerance)
    {}

    constexpr bool operator() (const T& lhs, const T& rhs) const noexcept
    {
        return std::abs(lhs - rhs) < std::max(relative * std::max(std::abs(lhs), std::abs(rhs)), absolute);
    }

private:
    const T relative;
    const T absolute;
};

} // namespace detail
} // namespace trial

#endif // TRIAL_DETAIL_FUNCTIONAL_HPP
