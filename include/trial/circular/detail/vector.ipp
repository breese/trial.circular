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

template <typename T, typename A>
vector<T, A>::vector() noexcept(std::is_nothrow_default_constructible<storage>::value)
    : span(storage::begin(), storage::end())
{
}

template <typename T, typename A>
vector<T, A>::vector(const allocator_type& allocator) noexcept(std::is_nothrow_constructible<storage, const allocator_type&>::value)
    : storage(allocator),
      span(storage::begin(), storage::end())
{
}

template <typename T, typename A>
vector<T, A>::vector(const vector& other)
    : storage(static_cast<const storage&>(other)),
      span(static_cast<const span&>(other), &*storage::begin())
{
}

template <typename T, typename A>
vector<T, A>::vector(const vector& other,
                     const allocator_type& allocator)
    : storage(static_cast<const storage&>(other), allocator),
      span(storage::begin(), storage::end())
{
}

template <typename T, typename A>
vector<T, A>::vector(vector&& other,
                     const allocator_type& allocator) noexcept(std::is_nothrow_constructible<storage, storage&&, const allocator_type&>::value)
    : storage(std::forward<storage>(other), allocator),
      span(std::forward<span>(other), &*storage::begin())
{
}

template <typename T, typename A>
vector<T, A>::vector(size_type capacity)
    : storage(capacity),
      span(storage::begin(), storage::end())
{
}

template <typename T, typename A>
vector<T, A>::vector(size_type capacity,
                     const allocator_type& allocator)
    : storage(capacity, allocator),
      span(storage::begin(), storage::end())
{
}

template <typename T, typename A>
vector<T, A>::vector(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value)
    : storage(input.size()),
      span(storage::begin(), storage::end())
{
    span::operator=(std::move(input));
}

template <typename T, typename A>
vector<T, A>::vector(std::initializer_list<value_type> input,
                     const allocator_type& allocator) noexcept(std::is_nothrow_move_assignable<value_type>::value)
    : storage(input.size(), allocator),
      span(storage::begin(), storage::end())
{
    span::operator=(std::move(input));
}

template <typename T, typename A>
auto vector<T, A>::operator=(const vector& other) -> vector&
{
    storage::operator=(static_cast<const storage&>(other));
    span::operator=(span(static_cast<const span&>(other), &*storage::begin()));
    return *this;
}

template <typename T, typename A>
auto vector<T, A>::operator=(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value) -> vector&
{
    span::clear();
    span::operator=(std::move(input));
    return *this;
}

template <typename T, typename A>
template <typename InputIterator>
vector<T, A>::vector(InputIterator first,
                     InputIterator last) noexcept(std::is_nothrow_copy_assignable<value_type>::value)
    : storage(first, last),
      span(storage::begin(), storage::end(), storage::begin(), storage::size())
{
}

template <typename T, typename A>
template <typename InputIterator>
vector<T, A>::vector(InputIterator first,
                     InputIterator last,
                     const allocator_type& allocator) noexcept(std::is_nothrow_copy_assignable<value_type>::value)
    : storage(first, last, allocator),
      span(storage::begin(), storage::end(), storage::begin(), storage::size())
{
}

template <typename T, typename A>
auto vector<T, A>::get_allocator() const -> allocator_type
{
    return storage::get_allocator();
}

template <typename T, typename A>
auto vector<T, A>::max_size() const noexcept -> size_type
{
    return span::capacity();
}

} // namespace circular
} // namespace trial
