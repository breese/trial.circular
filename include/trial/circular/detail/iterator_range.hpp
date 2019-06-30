#ifndef TRIAL_CIRCULAR_DETAIL_ITERATOR_RANGE_HPP
#define TRIAL_CIRCULAR_DETAIL_ITERATOR_RANGE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

namespace trial
{
namespace circular
{
namespace detail
{

//! @brief Iterator range

template <typename Iterator>
class iterator_range
{
public:
    constexpr iterator_range(Iterator first, Iterator last) noexcept
        : member{ std::move(first), std::move(last) }
    {
    }

    constexpr Iterator begin() const noexcept
    {
        return member.first;
    }

    constexpr Iterator end() const noexcept
    {
        return member.last;
    }

private:
    struct
    {
        Iterator first;
        Iterator last;
    } member;
};

template <typename Iterator>
iterator_range<Iterator> make_iterator_range(Iterator first, Iterator last)
{
    return iterator_range<Iterator>(std::move(first), std::move(last));
}

} // namespace detail
} // namespace circular
} // namespace trial

#endif // TRIAL_CIRCULAR_DETAIL_ITERATOR_RANGE_HPP
