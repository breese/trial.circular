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
constexpr array<T, N>::array() noexcept
    : span(storage::begin(), storage::end())
{
}

template <typename T, std::size_t N>
template <typename... Args>
constexpr array<T, N>::array(value_type arg1, Args&&... args) noexcept(std::is_nothrow_move_assignable<value_type>::value)
    : storage{std::move(arg1), std::forward<decltype(args)>(args)...},
      span(storage::begin(), storage::end(), storage::begin(), 1 + sizeof...(args))
{
}

template <typename T, std::size_t N>
TRIAL_CXX14_CONSTEXPR
auto array<T, N>::operator=(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value) -> array&
{
    span::operator=(std::move(input));
    return *this;
}

template <typename T, std::size_t N>
constexpr auto array<T, N>::max_size() const noexcept -> size_type
{
    return capacity();
}

} // namespace circular
} // namespace trial
