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
#include <initializer_list>
#include <iterator>
#include <trial/circular/detail/config.hpp>
#include <trial/circular/detail/type_traits.hpp>

namespace trial
{
namespace circular
{

//! @brief Circular span.
//!
//! Violation of any precondition results in undefined behavior.

template <typename T>
class span
{
public:
    using element_type = T;
    using value_type = typename std::remove_cv<element_type>::type;
    using size_type = std::size_t;
    using pointer = typename std::add_pointer<element_type>::type;
    using reference = typename std::add_lvalue_reference<element_type>::type;
    using const_reference = typename std::add_const<reference>::type;

    //! @brief Creates empty circular span.
    //!
    //! Must be recreated before use.
    //!
    //! @post capacity() == 0
    //! @post size() == 0

    constexpr span() noexcept;

    //! @brief Creates circular span by copying.

    constexpr span(const span&) noexcept = default;

    //! @brief Creates circular span by copying.
    //!
    //! The pointer parameter overrides the pointer of the input span.

    constexpr span(const span&, pointer) noexcept;

    //! @brief Creates circular span by moving.
    //!
    //! State of moved-from span is valid but undefined.

    constexpr span(span&&) noexcept = default;

    //! @brief Recreates circular span by copying.

    TRIAL_CXX14_CONSTEXPR
    span& operator=(const span&) noexcept = default;

    //! @brief Recreates circular span by moving.
    //!
    //! State of moved-from span is valid but undefined.

    TRIAL_CXX14_CONSTEXPR
    span& operator=(span&&) noexcept = default;

    //! @brief Recreates circular span from initializer list.
    //!
    //! @post capacity() == input.size()
    //! @post size() == input.size()

    TRIAL_CXX14_CONSTEXPR
    span& operator=(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Creates circular span from iterators.
    //!
    //! The span covers the range from @c begin to @c end.
    //!
    //! @post capacity() == std::distance(begin, end)
    //! @post size() == 0

    template <typename ContiguousIterator>
    constexpr span(ContiguousIterator begin,
                   ContiguousIterator end) noexcept;

    //! @brief Creates circular span from iterators.
    //!
    //! The span covers the range from @c begin to @c end.
    //!
    //! The span is initialized as if the pre-existing @c length values from
    //! @c first had already been pushed onto the span.
    //!
    //! @pre length <= std::distance(first, end)
    //! @post capacity() == std::distance(begin, end)
    //! @post size() == length

    template <typename ContiguousIterator>
    constexpr span(ContiguousIterator begin,
                   ContiguousIterator end,
                   ContiguousIterator first,
                   size_type length) noexcept;

    //! @brief Creates circular span from array.
    //!
    //! @post capacity() == N
    //! @post size() == 0

    template <std::size_t N>
    explicit constexpr span(value_type (&array)[N]) noexcept;

    //! @brief Checks if span is empty.

    constexpr bool empty() const noexcept;

    //! @brief Checks if span is full.

    constexpr bool full() const noexcept;

    //! @brief Returns the maximum possible number of elements in span.

    constexpr size_type capacity() const noexcept;

    //! @brief Returns the number of elements in span.

    constexpr size_type size() const noexcept;

    //! @brief Returns reference to first element in span.
    //!
    //! @pre !empty()

    TRIAL_CXX14_CONSTEXPR
    reference front() noexcept;

    //! @brief Returns reference to first element in span.
    //!
    //! @pre !empty()

    constexpr const_reference front() const noexcept;

    //! @brief Returns reference to last element in span.
    //!
    //! @pre !empty()

    TRIAL_CXX14_CONSTEXPR
    reference back() noexcept;

    //! @brief Returns reference to last element in span.
    //!
    //! @pre !empty()

    constexpr const_reference back() const noexcept;

    //! @brief Returns reference to element at position.
    //!
    //! @pre size() > position

    TRIAL_CXX14_CONSTEXPR
    reference operator[](size_type position) noexcept;

    //! @brief Returns reference to element at position.
    //!
    //! @pre size() > position

    constexpr const_reference operator[](size_type position) const noexcept;

    //! @brief Clears the span.
    //!
    //! The content of the underlying storage is not modified.
    //!
    //! @post size() == 0

    TRIAL_CXX14_CONSTEXPR
    void clear() noexcept;

    //! @brief Clears span and inserts elements at end of span.
    //!
    //! @post size() == std::min(std::distance(first, last), capacity())

    template <typename InputIterator>
    TRIAL_CXX14_CONSTEXPR
    void assign(InputIterator first, InputIterator last) noexcept(std::is_nothrow_copy_assignable<value_type>::value);

    //! @brief Clears span and inserts elements at end of span.
    //!
    //! @post size() == std::min(input.size(), capacity())

    TRIAL_CXX14_CONSTEXPR
    void assign(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Inserts element at beginning of span.
    //!
    //! If span is full, then the element at the end of the span is silently erased
    //! to make room for the @c input value.

    TRIAL_CXX14_CONSTEXPR
    void push_front(value_type input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Inserts element at end of span.
    //!
    //! If span is full, then the element at the end of the span is silently erased
    //! to make room for the @c input value.

    TRIAL_CXX14_CONSTEXPR
    void push_back(value_type input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Erases element from beginning of span.
    //!
    //! @pre !empty()

    TRIAL_CXX14_CONSTEXPR
    void pop_front() noexcept;

    //! @brief Erases element from end of span.
    //!
    //! @pre !empty()

    TRIAL_CXX14_CONSTEXPR
    void pop_back() noexcept;

    //! @brief Removes and returns element from beginning of span.
    //!
    //! @pre !empty()

    TRIAL_CXX14_CONSTEXPR
    value_type move_front() noexcept(std::is_nothrow_move_constructible<value_type>::value);

    //! @brief Removes and returns element from end of span.
    //!
    //! @pre !empty()

    TRIAL_CXX14_CONSTEXPR
    value_type move_back() noexcept(std::is_nothrow_move_constructible<value_type>::value);

    //! @brief Rotates span left by amount.
    //!
    //! If the span is full, then internal state is updated to emulate a rotation, but
    //! leaving the elements in their original memory position. Constant time complexity.
    //!
    //! If the span is not full, then elements are moved. Linear time complexity.

    TRIAL_CXX14_CONSTEXPR
    void advance_left(size_type amount) noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Rotates span right by amount.
    //!
    //! If the span is full, then internal state is updated to emulate a rotation, but
    //! leaving the elements in their original memory position. Constant time complexity.
    //!
    //! If the span is not full, then elements are moved. Linear time complexity.

    TRIAL_CXX14_CONSTEXPR
    void advance_right(size_type amount) noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Rotates elements so span starts at beginning of storage.
    //!
    //! For instance, a span consisting of the sequence A, B, C, may be stored
    //! in memory as:
    //!
    //!   +---+---+---+---+---+
    //!   | C |   |   | A | B |
    //!   +---+---+---+---+---+
    //!
    //! After normalization the elements are stored in memory as:
    //!
    //!   +---+---+---+---+---+
    //!   | A | B | C |   |   |
    //!   +---+---+---+---+---+
    //!
    //! Normalization invalidates pointers and references.
    //! Normalization does not invalidate iterators.
    //!
    //! Linear time complexity.

    TRIAL_CXX14_CONSTEXPR
    void normalize() noexcept(detail::is_nothrow_swappable<value_type>::value);

    constexpr bool is_normalized() const noexcept;

private:
    template <typename U>
    struct basic_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using element_type = U;
        using value_type = typename std::remove_cv<element_type>::type;
        using difference_type = std::ptrdiff_t;
        using pointer = typename std::add_pointer<element_type>::type;
        using reference = typename std::add_lvalue_reference<element_type>::type;
        using const_reference = typename std::add_const<reference>::type;
        using iterator_type = basic_iterator<U>;

        constexpr basic_iterator() noexcept = default;
        constexpr basic_iterator(const basic_iterator&) noexcept = default;
        constexpr basic_iterator(basic_iterator&&) noexcept = default;
        TRIAL_CXX14_CONSTEXPR
        basic_iterator& operator=(const basic_iterator&) noexcept = default;
        TRIAL_CXX14_CONSTEXPR
        basic_iterator& operator=(basic_iterator&&) noexcept = default;

        TRIAL_CXX14_CONSTEXPR
        iterator_type& operator++() noexcept;
        TRIAL_CXX14_CONSTEXPR
        iterator_type operator++(int) noexcept;
        TRIAL_CXX14_CONSTEXPR
        iterator_type& operator--() noexcept;
        TRIAL_CXX14_CONSTEXPR
        iterator_type operator--(int) noexcept;

        TRIAL_CXX14_CONSTEXPR
        pointer operator->() noexcept;
        constexpr const_reference operator*() const noexcept;

        constexpr bool operator==(const iterator_type&) const noexcept;
        constexpr bool operator!=(const iterator_type&) const noexcept;

    private:
        friend class span<T>;

        constexpr basic_iterator(const span<T>* parent, const size_type index) noexcept;

    private:
        const span<T>* parent;
        size_type current;
    };

public:
    //! @brief Bi-directional iterator.
    //!
    //! Mutable iterators are not supported to avoid incorrect use in mutating
    //! algorithms.

    using const_iterator = basic_iterator<typename std::add_const<value_type>::type>;

    //! @brief Returns iterator to the beginning of the span.

    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator cbegin() const noexcept;

    //! @brief Returns iterator to the ending of the span.

    constexpr const_iterator end() const noexcept;
    constexpr const_iterator cend() const noexcept;

protected:
    constexpr size_type index(size_type) const noexcept;
    constexpr size_type vindex(size_type) const noexcept;

    constexpr size_type front_index() const noexcept;
    constexpr size_type back_index() const noexcept;

    TRIAL_CXX14_CONSTEXPR
    reference at(size_type) noexcept;
    constexpr const_reference at(size_type) const noexcept;

    TRIAL_CXX14_CONSTEXPR
    void rotate(size_type lower_length, size_type upper_length) noexcept(detail::is_nothrow_swappable<value_type>::value);

    TRIAL_CXX14_CONSTEXPR
    void swap_range(size_type lhs, size_type rhs, size_type length) noexcept(detail::is_nothrow_swappable<value_type>::value);

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
