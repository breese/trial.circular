#ifndef TRIAL_CIRCULAR_EXAMPLE_IMPULSE_HPP
#define TRIAL_CIRCULAR_EXAMPLE_IMPULSE_HPP

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
#include <numeric>
#include <array>
#include <trial/circular/array.hpp>

namespace trial
{
namespace circular
{
namespace example
{

// Finite Impulse Response filter

template <typename T, std::size_t N>
class impulse
{
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");

public:
    using value_type = T;

    //! @brief Creates a finite impulse filter with N entries.

    template <typename... Args>
    impulse(Args&&... args)
        : coefficients{{std::forward<Args>(args)...}}
    {
    }

    //! @brief Appends data point.

    void push(value_type input)
    {
        window.push_front(input);
    }

    //! @brief Returns the filtered value.

    value_type value() const
    {
        return std::inner_product(window.begin(),
                                  window.end(),
                                  coefficients.begin(),
                                  value_type{});
    }

private:
    circular::array<value_type, N> window;
    std::array<value_type, N> coefficients;
};

} // namespace example
} // namespace circular
} // namespace trial

#endif // TRIAL_CIRCULAR_EXAMPLE_IMPULSE_HPP
