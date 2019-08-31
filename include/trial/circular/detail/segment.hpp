#ifndef TRIAL_CIRCULAR_DETAIL_SEGMENT_HPP
#define TRIAL_CIRCULAR_DETAIL_SEGMENT_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <trial/circular/detail/config.hpp>

namespace trial
{
namespace circular
{
namespace detail
{

template <typename T>
class segment
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;

    constexpr segment()
        : member{ nullptr, size_type(0) }
    {
    }

    constexpr segment(pointer data, size_type size)
        : member{ data, size }
    {
    }

    constexpr segment(pointer first, pointer last)
        : member{ first, size_type(last - first) }
    {
    }

    TRIAL_CXX14_CONSTEXPR
    pointer data() noexcept
    {
        return member.data;
    }

    constexpr const_pointer data() const noexcept
    {
        return member.data;
    }

    constexpr size_type size() const noexcept
    {
        return member.size;
    }

    TRIAL_CXX14_CONSTEXPR
    iterator begin() noexcept
    {
        return member.data;
    }

    constexpr const_iterator begin() const noexcept
    {
        return member.data;
    }

    constexpr const_iterator cbegin() const noexcept
    {
        return member.data;
    }

    TRIAL_CXX14_CONSTEXPR
    iterator end() noexcept
    {
        return member.data + member.size;
    }

    constexpr const_iterator end() const noexcept
    {
        return member.data + member.size;
    }

    constexpr const_iterator cend() const noexcept
    {
        return member.data + member.size;
    }

private:
    struct
    {
        pointer data;
        size_type size;
    } member;
};

} // namespace detail
} // namespace circular
} // namespace trial

#endif // TRIAL_CIRCULAR_DETAIL_SEGMENT_HPP
