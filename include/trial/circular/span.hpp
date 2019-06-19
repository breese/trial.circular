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
#include <limits>
#include <trial/circular/detail/config.hpp>
#include <trial/circular/detail/type_traits.hpp>

namespace trial
{
namespace circular
{

//! @brief Circular span.
//!
//! A view that turns contiguous memory into a circular double-ended queue.
//! Inserting new elements will overwrite old elements when the queue is full.
//!
//! The memory is not owned by the span. The owner must ensure that the span is
//! destroyed before the memory is released.
//!
//! Size is the current number of elements in the span.
//!
//! Capacity is the maximum number of elements that can be inserted without
//! overwriting hold elements. Capacity cannot be changed.
//!
//! Violation of any precondition results in undefined behavior.

enum : std::size_t { dynamic_extent = std::numeric_limits<std::size_t>::max() };

template <typename T, std::size_t Extent = dynamic_extent>
class span
{
public:
    using element_type = T;
    using value_type = typename std::remove_cv<element_type>::type;
    using size_type = std::size_t;
    using pointer = typename std::add_pointer<element_type>::type;
    using reference = typename std::add_lvalue_reference<element_type>::type;
    using const_reference = typename std::add_const<reference>::type;

private:
    template <typename, std::size_t>
    friend class span;

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
        friend class span<T, Extent>;

        constexpr basic_iterator(const span<T, Extent>* parent, const size_type index) noexcept;

    private:
        const span<T, Extent>* parent;
        size_type current;
    };

public:
    //! @brief Bi-directional iterator.
    //!
    //! Mutable iterators are not supported to avoid incorrect use in mutating
    //! algorithms.

    using const_iterator = basic_iterator<typename std::add_const<value_type>::type>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    //! @brief Creates empty circular span.
    //!
    //! No elements can be inserted into a zero capacity span. The span must
    //! therefore be recreated before use.
    //!
    //! @post capacity() == 0
    //! @post size() == 0

    constexpr span() noexcept;

    //! @brief Creates circular span by copying.

    constexpr span(const span&) noexcept = default;

    //! @brief Creates circular span by copying.
    //!
    //! Enables copying mutable span to immutable span.

    template <typename OtherT,
              std::size_t OtherExtent,
              typename std::enable_if<(Extent == OtherExtent || Extent == dynamic_extent) && std::is_convertible<OtherT (*)[], T (*)[]>::value, int>::type = 0>
    explicit constexpr span(const span<OtherT, OtherExtent>& other) noexcept;

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

    //! @brief Replaces circular span with elements from initializer list.
    //!
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

    template <std::size_t N,
              typename std::enable_if<(Extent == N || Extent == dynamic_extent), int>::type = 0>
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

    //! @brief Replaces circular span with elements from range.
    //!
    //! @post size() == std::min(std::distance(first, last), capacity())

    template <typename InputIterator>
    TRIAL_CXX14_CONSTEXPR
    void assign(InputIterator first, InputIterator last) noexcept(std::is_nothrow_copy_assignable<value_type>::value);

    //! @brief Replaces circular span with elements from intializer list.
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

    //! @brief Erases elements from beginning of span.
    //!
    //! By default only one element is erased.
    //!
    //! @pre 0 < count <= size()

    TRIAL_CXX14_CONSTEXPR
    void pop_front(size_type count = 1U) noexcept;

    //! @brief Erases elements from end of span.
    //!
    //! By default only one element is erased.
    //!
    //! @pre 0 < count <= size()

    TRIAL_CXX14_CONSTEXPR
    void pop_back(size_type count = 1U) noexcept;

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

    //! @brief Returns iterator to the beginning of the span.

    constexpr const_iterator begin() const noexcept;

    //! @brief Returns iterator to the ending of the span.

    constexpr const_iterator end() const noexcept;

    //! @brief Returns iterator to the beginning of the span.

    constexpr const_iterator cbegin() const noexcept;

    //! @brief Returns iterator to the ending of the span.

    constexpr const_iterator cend() const noexcept;

    //! @brief Returns reverse iterator to the beginning of the span.

    constexpr const_reverse_iterator rbegin() const noexcept;

    //! @brief Returns reverse iterator to the end of the span.

    constexpr const_reverse_iterator rend() const noexcept;

    //! @brief Returns reverse iterator to the beginning of the span.

    constexpr const_reverse_iterator crbegin() const noexcept;

    //! @brief Returns reverse iterator to the end of the span.

    constexpr const_reverse_iterator crend() const noexcept;

protected:
    //! @brief Creates circular span by copying.
    //!
    //! The pointer parameter overrides the pointer of the input span.

    constexpr span(const span&, pointer) noexcept;

    //! @brief Recreates circular span by copying.
    //!
    //! The pointer parameter overrides the pointer of the input span.

    TRIAL_CXX14_CONSTEXPR
    void assign(const span&, pointer) noexcept;

private:
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

private:
    template <typename T1, std::size_t E1>
    struct member_storage
    {
        constexpr member_storage() noexcept;
        constexpr member_storage(const member_storage&) noexcept = default;
        constexpr member_storage(member_storage&&) noexcept = default;
        TRIAL_CXX14_CONSTEXPR
        member_storage& operator=(const member_storage&) noexcept = default;
        TRIAL_CXX14_CONSTEXPR
        member_storage& operator=(member_storage&&) noexcept = default;

        constexpr member_storage(pointer data, size_type size, size_type next) noexcept;

        constexpr member_storage(const member_storage&, pointer data) noexcept;

        template <typename OtherT, std::size_t OtherExtent>
        explicit constexpr member_storage(const span<OtherT, OtherExtent>&) noexcept;

        template <typename ContiguousIterator>
        constexpr member_storage(ContiguousIterator, ContiguousIterator) noexcept;

        template <typename ContiguousIterator>
        constexpr member_storage(ContiguousIterator, ContiguousIterator, ContiguousIterator, size_type) noexcept;

        template <std::size_t N>
        explicit constexpr member_storage(value_type (&array)[N]) noexcept;

        constexpr size_type capacity() const noexcept;

        TRIAL_CXX14_CONSTEXPR
        void capacity(size_type) noexcept;

        void assign(const member_storage&, pointer) noexcept;

        pointer data;
        size_type size;
        size_type next;
    };

    template <typename T1>
    struct member_storage<T1, dynamic_extent>
    {
        constexpr member_storage() noexcept;
        constexpr member_storage(const member_storage&) noexcept = default;
        constexpr member_storage(member_storage&&) noexcept = default;
        TRIAL_CXX14_CONSTEXPR
        member_storage& operator=(const member_storage&) noexcept = default;
        TRIAL_CXX14_CONSTEXPR
        member_storage& operator=(member_storage&&) noexcept = default;

        constexpr member_storage(pointer data, size_type capacity, size_type size, size_type next) noexcept;

        constexpr member_storage(const member_storage&, pointer data) noexcept;

        template <typename OtherT, std::size_t OtherExtent>
        explicit constexpr member_storage(const span<OtherT, OtherExtent>&) noexcept;

        template <typename ContiguousIterator>
        constexpr member_storage(ContiguousIterator, ContiguousIterator) noexcept;

        template <typename ContiguousIterator>
        constexpr member_storage(ContiguousIterator, ContiguousIterator, ContiguousIterator, size_type) noexcept;

        template <std::size_t N>
        explicit constexpr member_storage(value_type (&array)[N]) noexcept;

        constexpr size_type capacity() const noexcept;

        TRIAL_CXX14_CONSTEXPR
        void capacity(size_type input) noexcept;

        void assign(const member_storage&, pointer) noexcept;

        pointer data;
        size_type cap;
        size_type size;
        size_type next;
    };

    struct member_storage<T, Extent> member;
};

} // namespace circular
} // namespace trial

#include <trial/circular/detail/span.ipp>

#endif // TRIAL_CIRCULAR_SPAN_HPP
