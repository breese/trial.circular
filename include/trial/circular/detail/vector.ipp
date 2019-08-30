///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <utility> // std::swap

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
      span(static_cast<const span&>(other), &*storage::begin())
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
    : storage(capacity, T{}, allocator),
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
    : storage(input.size(), T{}, allocator),
      span(storage::begin(), storage::end())
{
    span::operator=(std::move(input));
}

template <typename T, typename A>
auto vector<T, A>::operator=(const vector& other) -> vector&
{
    storage::operator=(static_cast<const storage&>(other));
    span::assign(static_cast<const span&>(other), &*storage::begin());
    return *this;
}

template <typename T, typename A>
auto vector<T, A>::operator=(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value) -> vector&
{
    span::clear();
    if (input.size() > storage::size())
    {
        storage::resize(std::min(input.size(), storage::capacity()));
    }
    span::operator=(span(storage::begin(), storage::end(), storage::begin(), storage::size()));
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
void vector<T, A>::reserve(size_type capacity)
{
    if (capacity <= storage::capacity())
        return;

    // Update span in case storage is reallocated
    if (span::capacity() > 0)
    {
        span::rotate_front();
    }
    storage::reserve(capacity);
    span::operator=(span(storage::begin(),
                         storage::end(),
                         storage::begin(),
                         span::size()));
}

template <typename T, typename A>
void vector<T, A>::resize(size_type count)
{
    resize(count, value_type{});
}

template <typename T, typename A>
void vector<T, A>::resize(size_type count, const value_type& value)
{
    span::rotate_front();
    storage::resize(count, value);

    // Fill new span elements with input value
    for (auto k = span::size(); k < storage::size(); ++k)
    {
        storage::operator[](k) = value;
    }
    span::operator=(span(storage::begin(), storage::end(), storage::begin(), storage::size()));
}

template <typename T, typename A>
void vector<T, A>::push_front(value_type input)
{
    if (span::full())
    {
        if (span::capacity() < storage::capacity())
        {
            // Insert element at end where the is spare capacity.
            //
            // Then move element backwards until beginning, whereby the elements
            // before are shifted right by one.
            //
            // For example:
            //
            //   +---+---+---+---+---+
            //   | A | B | C |   |   |
            //   +---+---+---+---+---+
            //
            // Push Z to back:
            //
            //   +---+---+---+---+---+
            //   | A | B | C | Z |   |
            //   +---+---+---+---+---+
            //
            // Rotate Z to beginning:
            //
            //   +---+---+---+---+---+
            //   | Z | A | B | C |   |
            //   +---+---+---+---+---+

            storage::push_back(std::move(input));
            span::operator=(span(storage::begin(), storage::end(), storage::begin(), storage::size()));

            // Move to beginning is a linear operation.

            for (size_type k = size() - 1; k != 0; --k)
            {
                using std::swap;
                swap(span::operator[](k - 1), span::operator[](k));
            }
            return;
        }
    }
    span::push_front(std::move(input));
}

template <typename T, typename A>
void vector<T, A>::push_back(value_type input)
{
    if (span::full())
    {
        if (span::capacity() < storage::capacity())
        {
            // Insert element at end where there is spare capacity.

            storage::push_back(std::move(input));
            span::operator=(span(storage::begin(), storage::end(), storage::begin(), storage::size()));
            return;
        }
    }
    span::push_back(std::move(input));
}

} // namespace circular
} // namespace trial
