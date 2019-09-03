#ifndef TRIAL_CIRCULAR_ARRAY_HPP
#define TRIAL_CIRCULAR_ARRAY_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <array>
#include <trial/circular/span.hpp>

namespace trial
{
namespace circular
{

//! @brief Fixed-sized circular buffer.
//!
//! Size is the current number of elements in the buffer.
//!
//! Capacity is the maximum number of elements that can be inserted without
//! overwriting old elements. Capacity cannot be changed.
//!
//! Violation of any precondition results in undefined behavior.

template <typename T, std::size_t N>
class array
    : private std::array<T, N>,
      private circular::span<T, N>
{
    using storage = std::array<T, N>;
    using span = circular::template span<T, N>;

    static_assert(std::is_destructible<T>::value, "T must be Erasable");
    static_assert(std::is_default_constructible<T>::value, "T must be DefaultConstructible");
    static_assert(N != dynamic_extent, "N cannot be dynamic_extent");

public:
    using element_type = typename span::element_type;
    using value_type = typename span::value_type;
    using size_type = typename span::size_type;
    using reference = typename span::reference;
    using const_reference = typename span::const_reference;
    using iterator = typename span::iterator;
    using const_iterator = typename span::const_iterator;
    using reverse_iterator = typename span::reverse_iterator;
    using const_reverse_iterator = typename span::const_reverse_iterator;
    using segment = typename span::segment;
    using const_segment = typename span::const_segment;

    //! @brief Creates empty circular array.
    //!
    //! @post capacity() == N
    //! @post size() == 0

    constexpr array() noexcept;

    //! @brief Creates circular array by copying.
    //!
    //! @pre T must be copy constructible.
    //!
    //! @post capacity() == N
    //! @post size() == other.size()

    constexpr array(const array& other) noexcept(std::is_nothrow_copy_constructible<value_type>::value);

    //! @brief Recreates circular array by copying.
    //!
    //! @pre T must be copy assignable.
    //!
    //! @post capacity() == N
    //! @post size() == other.size()

    TRIAL_CXX14_CONSTEXPR
    array& operator=(const array& other) noexcept(std::is_nothrow_copy_assignable<value_type>::value);

    //! @brief Creates circular array by moving.
    //!
    //! @post capacity() == N
    //! @post size() == other.size()

    constexpr array(array&& other) noexcept(std::is_nothrow_move_constructible<value_type>::value) = default;

    //! @brief Recreates circular array by moving.
    //!
    //! @post capacity() == N
    //! @post size() == other.size()

    TRIAL_CXX14_CONSTEXPR
    array& operator=(array&& other) noexcept(std::is_nothrow_move_assignable<value_type>::value) = default;

    //! @brief Creates circular array with element from initializer list.
    //!
    //! All elements are inserted, but if input.size() > N then only the last N
    //! input elements will remain in the circular array.
    //!
    //! @post capacity() == N
    //! @post size() == input.size()

    template <typename... Args>
    constexpr array(value_type, Args&&...) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Recreates circular array with element from initializer list.
    //!
    //! All elements are inserted, but if input.size() > N then only the last N
    //! input elements will remain in the circular array.
    //!
    //! @post capacity() == N
    //! @post size() == input.size()

    TRIAL_CXX14_CONSTEXPR
    array& operator=(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Checks if circular array is empty.
    using span::empty;

    //! @brief Checks if circular array is full.
    using span::full;

    //! @brief Returns the maximum possible number of elements in circular array.
    using span::capacity;

    //! @brief Returns the number of elements in circular array.
    using span::size;

    //! @brief Returns the maximum number of possible elements in circular array.
    constexpr size_type max_size() const noexcept;

    //! @brief Returns reference to first element in circular array.
    using span::front;

    //! @brief Returns reference to last element in span.
    using span::back;

    //! @brief Returns reference to element at position.
    using span::operator[];

    //! @brief Clears the circular array.
    using span::clear;

    //! @brief Clears circular array and inserts element at end of circular array.
    using span::assign;

    //! @brief Inserts element at beginning of circular array.
    using span::push_front;

    //! @brief Removes and returns element at end of circular array.
    using span::push_back;

    //! @brief Removes and returns element from beginning of circular array.
    using span::pop_front;

    //! @brief Moves element from end of circular array.
    using span::pop_back;

    //! @brief Inserts unspecified elements at beginning of circular array.
    using span::expand_front;

    //! @brief Inserts unspecified elements at end of circular array.
    using span::expand_back;

    //! @brief Removes elements from beginning of circular array.
    using span::remove_front;

    //! @brief Removes elements from end of circular array.
    using span::remove_back;

    //! @brief Returns iterator to beginning of circular array.
    using span::begin;

    //! @brief Returns iterator to ending of circular array.
    using span::end;

    //! @brief Returns const iterator to beginning of circular array.
    using span::cbegin;

    //! @brief Returns const iterator to ending of circular array.
    using span::cend;

    //! @brief Returns reverse iterator to beginning of circular array.
    using span::rbegin;

    //! @brief Returns reverse iterator to ending of circular array.
    using span::rend;

    //! @brief Returns const reverse iterator to beginning of circular array.
    using span::crbegin;

    //! @brief Returns const reverse iterator to ending of circular array.
    using span::crend;

    //! @brief Returns first contiguous segment of circular array.
    using span::first_segment;

    //! @brief Returns last contiguous segment of circular array.
    using span::last_segment;

    //! @brief Returns first contiguous unused segment of circular array.
    using span::first_unused_segment;

    //! @brief Returns last contiguous unused segment of circular array.
    using span::last_unused_segment;
};

} // namespace circular
} // namespace trial

#include <trial/circular/detail/array.ipp>

#endif // TRIAL_CIRCULAR_ARRAY_HPP
