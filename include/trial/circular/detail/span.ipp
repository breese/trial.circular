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

template <typename T, std::size_t E>
constexpr span<T, E>::span() noexcept
{
}

template <typename T, std::size_t E>
template <typename ContiguousIterator>
constexpr span<T, E>::span(ContiguousIterator begin,
                           ContiguousIterator end) noexcept
    : member(std::move(begin), std::move(end))
{
}

template <typename T, std::size_t E>
template <typename ContiguousIterator>
constexpr span<T, E>::span(ContiguousIterator begin,
                           ContiguousIterator end,
                           ContiguousIterator first,
                           size_type length) noexcept
    : member(std::move(begin), std::move(end), std::move(first), length)
{
}

template <typename T, std::size_t E>
template <std::size_t N>
constexpr span<T, E>::span(value_type (&array)[N]) noexcept
    : member(array)
{
}

template <typename T, std::size_t E>
constexpr span<T, E>::span(const span& other, pointer data) noexcept
    : member{data, other.member.capacity(), other.member.size, other.member.next}
{
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::assign(const span& other, pointer data) noexcept
{
    member.assign(other.member, data);
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::operator=(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value) -> span&
{
    assign(std::move(input));
    return *this;
}

template <typename T, std::size_t E>
constexpr bool span<T, E>::empty() const noexcept
{
    return size() == 0;
}

template <typename T, std::size_t E>
constexpr bool span<T, E>::full() const noexcept
{
    return size() == capacity();
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::capacity() const noexcept -> size_type
{
    return member.capacity();
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::size() const noexcept -> size_type
{
    return member.size;
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::front() noexcept -> reference
{
    assert(!empty());

    return at(front_index());
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::front() const noexcept -> const_reference
{
    TRIAL_CIRCULAR_CXX14(assert(!empty()));

    return at(front_index());
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::back() noexcept -> reference
{
    assert(!empty());

    return at(back_index());
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::back() const noexcept -> const_reference
{
    TRIAL_CIRCULAR_CXX14(assert(!empty()));

    return at(back_index());
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::operator[](size_type position) noexcept -> reference
{
    return at(front_index() + position);
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::operator[](size_type position) const noexcept -> const_reference
{
    return at(front_index() + position);
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::clear() noexcept
{
    member.size = 0;
    member.next = member.capacity();
}

template <typename T, std::size_t E>
template <typename InputIterator>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::assign(InputIterator first, InputIterator last) noexcept(std::is_nothrow_copy_assignable<value_type>::value)
{
    clear();
    while (first != last)
    {
        push_back(*first);
        ++first;
    }
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::assign(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value)
{
    clear();
    for (const auto& value : input)
    {
        push_back(std::move(value));
    }
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::push_front(value_type input) noexcept(std::is_nothrow_move_assignable<value_type>::value)
{
    if (full())
    {
        member.next = member.capacity() + index(member.next - 1);
    }
    else
    {
        ++member.size;
    }
    front() = std::move(input);
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::push_back(value_type input) noexcept(std::is_nothrow_move_assignable<value_type>::value)
{
    member.next = member.capacity() + index(member.next + 1);
    if (!full())
    {
        ++member.size;
    }
    back() = std::move(input);
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::pop_front(size_type count) noexcept
{
    assert(count > 0);
    assert(count <= size());

    member.size -= count;
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::pop_back(size_type count) noexcept
{
    assert(count > 0);
    assert(count <= size());

    member.next = member.capacity() + index(member.next - count);
    member.size -= count;
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::move_front() noexcept(std::is_nothrow_move_constructible<value_type>::value) -> value_type
{
    auto& old_front = front();
    pop_front(); // Item still lingers in storage
    return std::move(old_front);
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::move_back() noexcept(std::is_nothrow_move_constructible<value_type>::value) -> value_type
{
    auto& old_back = back();
    pop_back(); // Item still lingers in storage
    return std::move(old_back);
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::advance_left(size_type count) noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_assignable<value_type>::value)
{
    if (size() < 2)
        return;
    count %= size();
    if (count == 0)
        return;

    if (full())
    {
        member.next = member.capacity() + index(member.next + count);
    }
    else
    {
        while (count-- > 0)
        {
            push_back(std::move(move_front()));
        }
    }
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::advance_right(size_type count) noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_assignable<value_type>::value)
{
    if (size() < 2)
        return;
    count %= size();
    if (count == 0)
        return;

    if (full())
    {
        member.next = member.capacity() + index(member.next - count);
    }
    else
    {
        while (count-- > 0)
        {
            push_front(std::move(move_back()));
        }
    }
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::normalize() noexcept(detail::is_nothrow_swappable<value_type>::value)
{
    if (empty())
        return;

    const auto first = index(front_index());
    if (first == 0)
        return;

    const auto last = capacity() - first;
    rotate(first, last);
    member.next = member.capacity() + size();
}

template <typename T, std::size_t E>
constexpr bool span<T, E>::is_normalized() const noexcept
{
    return index(front_index()) == 0;
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::begin() const noexcept -> const_iterator
{
    return const_iterator(this, vindex(front_index()));
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::cbegin() const noexcept -> const_iterator
{
    return const_iterator(this, vindex(front_index()));
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::end() const noexcept -> const_iterator
{
    return const_iterator(this, vindex(member.next));
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::cend() const noexcept -> const_iterator
{
    return const_iterator(this, vindex(member.next));
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::rbegin() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::rend() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::crbegin() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::crend() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}

//-----------------------------------------------------------------------------

template <typename T, std::size_t E>
constexpr auto span<T, E>::index(size_type position) const noexcept -> size_type
{
    return position % member.capacity();
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::vindex(size_type position) const noexcept -> size_type
{
    return position % (2 * member.capacity());
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::front_index() const noexcept -> size_type
{
    return member.next - member.size;
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::back_index() const noexcept -> size_type
{
    return member.next - 1;
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::at(size_type position) noexcept -> reference
{
    return member.data[index(position)];
}

template <typename T, std::size_t E>
constexpr auto span<T, E>::at(size_type position) const noexcept -> const_reference
{
    return member.data[index(position)];
}

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::rotate(size_type lower_length,
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

template <typename T, std::size_t E>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::swap_range(size_type lhs,
                            size_type rhs,
                            size_type length) noexcept(detail::is_nothrow_swappable<value_type>::value)
{
    for (size_type k = 0; k < length; ++k)
    {
        using std::swap;
        swap(at(lhs + k), at(rhs + k));
    }
 }

//-----------------------------------------------------------------------------
// span<T>::member_storage fixed extent
//-----------------------------------------------------------------------------

// std::addressof(x) and std::distance(a, b) are not constexpr before C++17, so
// we use &x and b - a instead, which ought to work for ContiguousIterator.

template <typename T, std::size_t E>
template <typename T1, std::size_t E1>
constexpr span<T, E>::member_storage<T1, E1>::member_storage() noexcept
    : data(nullptr),
      size(0),
      next(0)
{
}

template <typename T, std::size_t E>
template <typename T1, std::size_t E1>
constexpr span<T, E>::member_storage<T1, E1>::member_storage(pointer data, size_type size, size_type next) noexcept
    : data(data),
      size(size),
      next(next)
{
}

template <typename T, std::size_t E>
template <typename T1, std::size_t E1>
template <typename ContiguousIterator>
constexpr span<T, E>::member_storage<T1, E1>::member_storage(ContiguousIterator begin, ContiguousIterator end) noexcept
    : data(&*begin),
      size(0),
      next(size_type(end - begin))
{
}

template <typename T, std::size_t E>
template <typename T1, std::size_t E1>
template <typename ContiguousIterator>
constexpr span<T, E>::member_storage<T1, E1>::member_storage(ContiguousIterator begin,
                                                             ContiguousIterator end,
                                                             ContiguousIterator first,
                                                             size_type length) noexcept
    : data(&*begin),
      size(length),
      next(size_type(end - begin) + length - size_type(first - begin))
{
}

template <typename T, std::size_t E>
template <typename T1, std::size_t E1>
template <std::size_t N>
constexpr span<T, E>::member_storage<T1, E1>::member_storage(value_type (&array)[N]) noexcept
    : member_storage(array, array + N)
{
    static_assert(N >= E1, "N cannot be smaller than capacity");
}

template <typename T, std::size_t E>
template <typename T1, std::size_t E1>
constexpr auto span<T, E>::member_storage<T1, E1>::capacity() const noexcept -> size_type
{
    return E1;
}

template <typename T, std::size_t E>
template <typename T1, std::size_t E1>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::member_storage<T1, E1>::capacity(size_type) noexcept
{
}

template <typename T, std::size_t E>
template <typename T1, std::size_t E1>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::member_storage<T1, E1>::assign(const member_storage& other, pointer data) noexcept
{
    this->data = data;
    this->size = other.size;
    this->next = other.nexxt;
}

//-----------------------------------------------------------------------------
// span<T>::member_storage dynamic extent
//-----------------------------------------------------------------------------

template <typename T, std::size_t E>
template <typename T1>
constexpr span<T, E>::member_storage<T1, dynamic_extent>::member_storage() noexcept
    : data(nullptr),
      cap(0),
      size(0),
      next(0)
{
}

template <typename T, std::size_t E>
template <typename T1>
constexpr span<T, E>::member_storage<T1, dynamic_extent>::member_storage(pointer data, size_type capacity, size_type size, size_type next) noexcept
    : data(data),
      cap(capacity),
      size(size),
      next(next)
{
}

template <typename T, std::size_t E>
template <typename T1>
template <typename ContiguousIterator>
constexpr span<T, E>::member_storage<T1, dynamic_extent>::member_storage(ContiguousIterator begin, ContiguousIterator end) noexcept
    : data(&*begin),
      cap(size_type(end - begin)),
      size(0),
      next(size_type(end - begin))
{
}

template <typename T, std::size_t E>
template <typename T1>
template <typename ContiguousIterator>
constexpr span<T, E>::member_storage<T1, dynamic_extent>::member_storage(ContiguousIterator begin,
                                                                         ContiguousIterator end,
                                                                         ContiguousIterator first,
                                                                         size_type length) noexcept
    : data(&*begin),
      cap(size_type(end - begin)),
      size(length),
      next(size_type(end - begin) + length - size_type(first - begin))
{
}

template <typename T, std::size_t E>
template <typename T1>
template <std::size_t N>
constexpr span<T, E>::member_storage<T1, dynamic_extent>::member_storage(value_type (&array)[N]) noexcept
    : member_storage(array, array + N)
{
}

template <typename T, std::size_t E>
template <typename T1>
constexpr auto span<T, E>::member_storage<T1, dynamic_extent>::capacity() const noexcept -> size_type
{
    return cap;
}

template <typename T, std::size_t E>
template <typename T1>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::member_storage<T1, dynamic_extent>::capacity(size_type value) noexcept
{
    cap = value;
}

template <typename T, std::size_t E>
template <typename T1>
TRIAL_CXX14_CONSTEXPR
void span<T, E>::member_storage<T1, dynamic_extent>::assign(const member_storage& other, pointer data) noexcept
{
    this->data = data;
    capacity(other.capacity());
    this->size = other.size;
    this->next = other.next;
}

//-----------------------------------------------------------------------------
// span<T>::basic_iterator
//-----------------------------------------------------------------------------

template <typename T, std::size_t E>
template <typename U>
constexpr span<T, E>::basic_iterator<U>::basic_iterator(const span<T, E>* parent,
                                                        size_type position) noexcept
    : parent(parent),
      current(position)
{
}

template <typename T, std::size_t E>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::basic_iterator<U>::operator++() noexcept -> iterator_type&
{
    assert(parent);

    current = parent->vindex(current + 1);
    return *this;
}

template <typename T, std::size_t E>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::basic_iterator<U>::operator++(int) noexcept -> iterator_type
{
    assert(parent);

    auto before = *this;
    current = parent->vindex(current + 1);
    return before;
}

template <typename T, std::size_t E>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::basic_iterator<U>::operator--() noexcept -> iterator_type&
{
    assert(parent);

    current = parent->vindex(current - 1);
    return *this;
}

template <typename T, std::size_t E>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::basic_iterator<U>::operator--(int) noexcept -> iterator_type
{
    assert(parent);

    auto before = *this;
    current = parent->vindex(current - 1);
    return before;
}

template <typename T, std::size_t E>
template <typename U>
TRIAL_CXX14_CONSTEXPR
auto span<T, E>::basic_iterator<U>::operator-> () noexcept -> pointer
{
    assert(parent);

    return *parent->at(current);
}

template <typename T, std::size_t E>
template <typename U>
constexpr auto span<T, E>::basic_iterator<U>::operator*() const noexcept -> const_reference
{
    TRIAL_CIRCULAR_CXX14(assert(parent));

    return parent->at(current);
}

template <typename T, std::size_t E>
template <typename U>
constexpr bool span<T, E>::basic_iterator<U>::operator==(const iterator_type& other) const noexcept
{
    TRIAL_CIRCULAR_CXX14(assert(parent));
    TRIAL_CIRCULAR_CXX14(assert(parent == other.parent));

    return current == other.current;
}

template <typename T, std::size_t E>
template <typename U>
constexpr bool span<T, E>::basic_iterator<U>::operator!=(const iterator_type& other) const noexcept
{
    return !operator==(other);
}

} // namespace circular
} // namespace trial
