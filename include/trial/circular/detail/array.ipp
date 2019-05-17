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

template <typename T, std::size_t N>
array<T, N>::array() noexcept
    : span(storage::begin(), storage::end())
{
}

template <typename T, std::size_t N>
array<T, N>::array(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value)
    : span(storage::begin(), storage::end())
{
    span::operator=(std::move(input));
}

template <typename T, std::size_t N>
template <typename InputIterator>
array<T, N>::array(InputIterator first,
                   InputIterator last) noexcept(std::is_nothrow_copy_assignable<value_type>::value)
    : span(storage::begin(), storage::end())
{
    span::assign(std::forward<decltype(first)>(first),
                 std::forward<decltype(last)>(last));
}

} // namespace circular
} // namespace trial
