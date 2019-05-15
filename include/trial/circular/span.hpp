#ifndef TRIAL_CIRCULAR_SPAN_HPP
#define TRIAL_CIRCULAR_SPAN_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <type_traits>
#include <initializer_list>
#include <iterator>

namespace trial
{
namespace circular
{

template <typename T>
class span
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using pointer = typename std::add_pointer<value_type>::type;
    using reference = typename std::add_lvalue_reference<value_type>::type;
    using const_reference = typename std::add_const<reference>::type;

    //! @brief Creates empty circular span.
    //!
    //! Must be recreated before use.
    //!
    //! @post capacity() == 0
    //! @post size() == 0

    span() noexcept;

    //! @brief Creates circular span by copying.

    span(const span&) noexcept = default;

    //! @brief Creates circular span by moving.
    //!
    //! State of moved-from span is valid but undefined.

    span(span&&) noexcept = default;

    //! @brief Recreates circular span by copying.

    span& operator= (const span&) noexcept = default;

    //! @brief Recreates circular span by moving.
    //!
    //! State of moved-from span is valid but undefined.

    span& operator= (span&&) noexcept = default;

    //! @brief Recreates circular span from initializer list.
    //!
    //! @post capacity() == input.size()
    //! @post size() == input.size()

    span& operator= (std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Creates circular span from iterators.
    //!
    //! The span covers the range from @c begin to @c end.
    //!
    //! @post capacity() == std::distance(begin, end)
    //! @post size() == 0

    template <typename ContiguousIterator>
    span(ContiguousIterator begin,
         ContiguousIterator end) noexcept;

    //! @brief Creates circular span from iterators.
    //!
    //! The span covers the range from @c begin to @c end.
    //!
    //! The span is initialized as if the pre-existing @c length values from
    //! @c first had already been pushed onto the span.
    //!
    //! @post capacity() == std::distance(begin, end)
    //! @post size() == length

    template <typename ContiguousIterator>
    span(ContiguousIterator begin,
         ContiguousIterator end,
         ContiguousIterator first,
         size_type length) noexcept;

    //! @brief Creates circular span from array.
    //!
    //! @post capacity() == N
    //! @post size() == 0

    template <std::size_t N>
    explicit span(value_type (&array)[N]) noexcept;

    //! @brief Checks if span is empty.

    bool empty() const noexcept;

    //! @brief Checks if span is full.

    bool full() const noexcept;

    //! @brief Returns the maximum possible number of elements in span.

    size_type capacity() const noexcept;

    //! @brief Returns the number of elements in span.

    size_type size() const noexcept;

    //! @brief Returns reference to first element in span.
    //!
    //! @pre !empty()

    const_reference front() const noexcept;

    //! @brief Returns reference to last element in span.
    //!
    //! @pre !empty()

    const_reference back() const noexcept;

    //! @brief Clears the span.
    //!
    //! The content of the underlying storage is not modified.
    //!
    //! @post size() == 0

    void clear() noexcept;

    //! @brief Clears span and inserts elements at end of span.
    //!
    //! @post size() == std::min(std::distance(first, last), capacity())

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last) noexcept(std::is_nothrow_copy_assignable<value_type>::value);

    //! @brief Clears span and inserts elements at end of span.
    //!
    //! @post size() == std::min(input.size(), capacity())

    void assign(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Inserts element at beginning of span.

    void push_front(value_type input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Inserts element at end of span.

    void push_back(value_type input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Erases element from beginning of span
    //!
    //! @pre !empty()

    void pop_front() noexcept;

    //! @brief Erases element from end of span
    //!
    //! @pre !empty()

    void pop_back() noexcept;

    //! @brief Moves element from beginning of span
    //!
    //! @pre !empty()

    value_type move_front() noexcept(std::is_nothrow_move_constructible<value_type>::value);

    //! @brief Moves element from end of span
    //!
    //! @pre !empty()

    value_type move_back() noexcept(std::is_nothrow_move_constructible<value_type>::value);

private:
    template <typename U>
    struct basic_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = U;
        using difference_type = std::ptrdiff_t;
        using pointer = typename std::add_pointer<value_type>::type;
        using reference = typename std::add_lvalue_reference<value_type>::type;
        using const_reference = typename std::add_const<reference>::type;
        using iterator_type = basic_iterator<value_type>;

        basic_iterator() noexcept = default;
        basic_iterator(const basic_iterator&) noexcept = default;
        basic_iterator(basic_iterator&&) noexcept = default;
        basic_iterator& operator= (const basic_iterator&) noexcept = default;
        basic_iterator& operator= (basic_iterator&&) noexcept = default;

        iterator_type& operator++ () noexcept;
        iterator_type operator++ (int) noexcept;
        iterator_type& operator-- () noexcept;
        iterator_type operator-- (int) noexcept;

        pointer operator-> () noexcept;
        const_reference operator* () const noexcept;

        bool operator== (const iterator_type&) const noexcept;
        bool operator!= (const iterator_type&) const noexcept;

    private:
        friend class span<T>;

        basic_iterator(const span<T> *parent, const size_type index) noexcept;

    private:
        const span<T> *parent;
        size_type current;
    };

public:
    using iterator = basic_iterator<value_type>;
    using const_iterator = basic_iterator<const value_type>;

    //! @brief Returns iterator to the beginning of the span.

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    //! @brief Returns iterator to the ending of the span.

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

protected:
    size_type index(size_type) const noexcept;
    size_type vindex(size_type) const noexcept;
    size_type front_index() const noexcept;
    size_type back_index() const noexcept;
    reference at(size_type) noexcept;
    const_reference at(size_type) const noexcept;

protected:
    struct
    {
        pointer data;
        size_type capacity;
        size_type size;
        size_type next;
    } member;
};

} // namespace circular
} // namespace trial

#include <trial/circular/detail/span.ipp>

#endif // TRIAL_CIRCULAR_SPAN_HPP
