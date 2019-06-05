///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>

namespace trial
{
namespace circular
{

//-----------------------------------------------------------------------------
// span<T>
//-----------------------------------------------------------------------------

template <typename T>
constexpr span<T>::span() noexcept
    : member{nullptr, 0, 0, 0}
{
}

// std::addressof(x) and std::distance(a, b) are not constexpr before C++17, so
// we use &x and b - a instead, which ought to work for ContiguousIterator.

template <typename T>
template <typename ContiguousIterator>
constexpr span<T>::span(ContiguousIterator begin,
                        ContiguousIterator end) noexcept
    : member{ &*begin, size_type(end - begin), 0, size_type(end - begin) }
{
}

template <typename T>
template <typename ContiguousIterator>
constexpr span<T>::span(ContiguousIterator begin,
                        ContiguousIterator end,
                        ContiguousIterator first,
                        size_type length) noexcept
    : member{ &*begin, size_type(end - begin), length, size_type(end - begin) + length - size_type(first - begin) }
{
}

template <typename T>
template <std::size_t N>
constexpr span<T>::span(value_type (&array)[N]) noexcept
    : span(array, array + N)
{
}

template <typename T>
constexpr span<T>::span(const span& other, pointer data) noexcept
    : member{data, other.member.capacity, other.member.size, other.member.next}
{
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
auto span<T>::operator=(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value) -> span&
{
    assign(std::move(input));
    return *this;
}

template <typename T>
constexpr bool span<T>::empty() const noexcept
{
    return size() == 0;
}

template <typename T>
constexpr bool span<T>::full() const noexcept
{
    return size() == capacity();
}

template <typename T>
constexpr auto span<T>::capacity() const noexcept -> size_type
{
    return member.capacity;
}

template <typename T>
constexpr auto span<T>::size() const noexcept -> size_type
{
    return member.size;
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
auto span<T>::front() noexcept -> reference
{
    assert(!empty());

    return at(front_index());
}

template <typename T>
constexpr auto span<T>::front() const noexcept -> const_reference
{
    TRIAL_CIRCULAR_CXX14(assert(!empty()));

    return at(front_index());
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
auto span<T>::back() noexcept -> reference
{
    assert(!empty());

    return at(back_index());
}

template <typename T>
constexpr auto span<T>::back() const noexcept -> const_reference
{
    TRIAL_CIRCULAR_CXX14(assert(!empty()));

    return at(back_index());
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
auto span<T>::operator[](size_type position) noexcept -> reference
{
    return at(front_index() + position);
}

template <typename T>
constexpr auto span<T>::operator[](size_type position) const noexcept -> const_reference
{
    return at(front_index() + position);
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::clear() noexcept
{
    member.size = 0;
    member.next = member.capacity;
}

template <typename T>
template <typename InputIterator>
TRIAL_CXX14_CONSTEXPR
void span<T>::assign(InputIterator first, InputIterator last) noexcept(std::is_nothrow_copy_assignable<value_type>::value)
{
    clear();
    while (first != last)
    {
        push_back(*first);
        ++first;
    }
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::assign(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value)
{
    clear();
    for (const auto& value : input)
    {
        push_back(std::move(value));
    }
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::push_front(value_type input) noexcept(std::is_nothrow_move_assignable<value_type>::value)
{
    if (full())
    {
        member.next = member.capacity + index(member.next - 1);
    }
    else
    {
        ++member.size;
    }
    at(member.next - member.size) = std::move(input);
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::push_back(value_type input) noexcept(std::is_nothrow_move_assignable<value_type>::value)
{
    at(member.next) = std::move(input);
    member.next = member.capacity + index(member.next + 1);
    if (!full())
    {
        ++member.size;
    }
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::pop_front() noexcept
{
    assert(!empty());

    --member.size;
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::pop_back() noexcept
{
    assert(!empty());

    member.next = member.capacity + index(member.next - 1);
    --member.size;
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
auto span<T>::move_front() noexcept(std::is_nothrow_move_constructible<value_type>::value) -> value_type
{
    auto old_index = front_index();
    pop_front(); // Item still lingers in storage
    return std::move(at(old_index));
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
auto span<T>::move_back() noexcept(std::is_nothrow_move_constructible<value_type>::value) -> value_type
{
    auto old_index = back_index();
    pop_back(); // Item still lingers in storage
    return std::move(at(old_index));
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::advance_left(size_type count) noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_assignable<value_type>::value)
{
    if (size() < 2)
        return;
    count %= size();
    if (count == 0)
        return;

    if (full())
    {
        member.next = member.capacity + index(member.next + count);
    }
    else
    {
        while (count-- > 0)
        {
            push_back(std::move(move_front()));
        }
    }
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::advance_right(size_type count) noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_assignable<value_type>::value)
{
    if (size() < 2)
        return;
    count %= size();
    if (count == 0)
        return;

    if (full())
    {
        member.next = member.capacity + index(member.next - count);
    }
    else
    {
        while (count-- > 0)
        {
            push_front(std::move(move_back()));
        }
    }
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::normalize() noexcept(detail::is_nothrow_swappable<value_type>::value)
{
    if (empty())
        return;

    const auto first = index(front_index());
    if (first == 0)
        return;

    const auto last = capacity() - first;
    rotate(first, last);
    member.next = member.capacity + size();
}

template <typename T>
constexpr auto span<T>::begin() const noexcept -> const_iterator
{
    return const_iterator(this, vindex(front_index()));
}

template <typename T>
constexpr auto span<T>::cbegin() const noexcept -> const_iterator
{
    return const_iterator(this, vindex(front_index()));
}

template <typename T>
constexpr auto span<T>::end() const noexcept -> const_iterator
{
    return const_iterator(this, vindex(member.next));
}

template <typename T>
constexpr auto span<T>::cend() const noexcept -> const_iterator
{
    return const_iterator(this, vindex(member.next));
}

//-----------------------------------------------------------------------------

template <typename T>
constexpr auto span<T>::index(size_type position) const noexcept -> size_type
{
    return position % member.capacity;
}

template <typename T>
constexpr auto span<T>::vindex(size_type position) const noexcept -> size_type
{
    return position % (2 * member.capacity);
}

template <typename T>
constexpr auto span<T>::front_index() const noexcept -> size_type
{
    return member.next - member.size;
}

template <typename T>
constexpr auto span<T>::back_index() const noexcept -> size_type
{
    return member.next - 1;
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
auto span<T>::at(size_type position) noexcept -> reference
{
    return member.data[index(position)];
}

template <typename T>
constexpr auto span<T>::at(size_type position) const noexcept -> const_reference
{
    return member.data[index(position)];
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::rotate(size_type lower_length,
                     size_type upper_length) noexcept(detail::is_nothrow_swappable<value_type>::value)
{
    // Based on Gries-Mills block swapping rotate
    if (lower_length == 0 || upper_length == 0)
        return;
    const auto position = lower_length;
    while (lower_length != upper_length)
    {
        if (lower_length > upper_length)
        {
            swap_range(position - lower_length, position, upper_length);
            lower_length -= upper_length;
        }
        else
        {
            swap_range(position - lower_length, position + upper_length - lower_length, lower_length);
            upper_length -= lower_length;
        }
    }
    swap_range(position - lower_length, position, lower_length);
}

template <typename T>
TRIAL_CXX14_CONSTEXPR
void span<T>::swap_range(size_type lhs,
                         size_type rhs,
                         size_type length) noexcept(detail::is_nothrow_swappable<value_type>::value)
{
    for (size_type k = 0; k < length; ++k)
    {
        using std::swap;
        swap(span::at(lhs + k), span::at(rhs + k));
    }
 }

//-----------------------------------------------------------------------------
// span<T>::basic_iterator
//-----------------------------------------------------------------------------

template <typename T>
template <typename U>
constexpr span<T>::basic_iterator<U>::basic_iterator(const span<T>* parent,
                                                     size_type position) noexcept
    : parent(parent),
      current(position)
{
}

template <typename T>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T>::basic_iterator<U>::operator++() noexcept -> iterator_type&
{
    assert(parent);

    current = parent->vindex(current + 1);
    return *this;
}

template <typename T>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T>::basic_iterator<U>::operator++(int) noexcept -> iterator_type
{
    assert(parent);

    auto before = *this;
    current = parent->vindex(current + 1);
    return before;
}

template <typename T>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T>::basic_iterator<U>::operator--() noexcept -> iterator_type&
{
    assert(parent);

    current = parent->vindex(current - 1);
    return *this;
}

template <typename T>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T>::basic_iterator<U>::operator--(int) noexcept -> iterator_type
{
    assert(parent);

    auto before = *this;
    current = parent->vindex(current - 1);
    return before;
}

template <typename T>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T>::basic_iterator<U>::operator-> () noexcept -> pointer
{
    assert(parent);

    return *parent->at(current);
}

template <typename T>
template <typename U>
constexpr auto span<T>::basic_iterator<U>::operator*() const noexcept -> const_reference
{
    TRIAL_CIRCULAR_CXX14(assert(parent));

    return parent->at(current);
}

template <typename T>
template <typename U>
constexpr bool span<T>::basic_iterator<U>::operator==(const iterator_type& other) const noexcept
{
    TRIAL_CIRCULAR_CXX14(assert(parent));
    TRIAL_CIRCULAR_CXX14(assert(parent == other.parent));

    return current == other.current;
}

template <typename T>
template <typename U>
constexpr bool span<T>::basic_iterator<U>::operator!=(const iterator_type& other) const noexcept
{
    return !operator==(other);
}

} // namespace circular
} // namespace trial
