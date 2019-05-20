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

template <typename T, std::size_t N>
class array
    : private std::array<T, N>,
      private circular::span<T>
{
    using storage = std::array<T, N>;
    using span = circular::template span<T>;

public:
    using typename span::value_type;
    using typename span::size_type;
    using typename span::reference;
    using typename span::const_reference;
    using typename span::iterator;
    using typename span::const_iterator;

    //! @brief Creates empty circular array.
    //!
    //! @post capacity() == N
    //! @post size() == 0

    array() noexcept;

    // Non-copyable like std::array<T, N>

    array(const array&) = delete;
    array& operator=(const array&) = delete;

    //! @brief Creates circular array by moving.
    //!
    //! @post capacity() == N
    //! @post size() == other.size()

    array(array&& other) noexcept(std::is_nothrow_move_constructible<value_type>::value) = default;

    //! @brief Recreates circular array by moving.
    //!
    //! @post capacity() == N
    //! @post size() == other.size()

    array& operator=(array&& other) noexcept(std::is_nothrow_move_assignable<value_type>::value) = default;

    //! @brief Creates circular array with element from initializer list.
    //!
    //! @post capacity() == N
    //! @post size() == input.size()

    array(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Rereates circular array with element from initializer list.
    //!
    //! @post capacity() == N
    //! @post size() == input.size()

    array& operator=(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Creates circular array with elements from range.
    //!
    //! @post capacity() == N
    //! @post size() == std::min(std::distance(first, last), N)

    template <typename InputIterator>
    array(InputIterator first,
          InputIterator last) noexcept(std::is_nothrow_copy_assignable<value_type>::value);

    //! @brief Checks if circular array is empty.
    using span::empty;

    //! @brief Checks if circular array is full.
    using span::full;

    //! @brief Returns the maximum possible number of elements in circular array.
    using span::capacity;

    //! @brief Returns the number of elements in circular array.
    using span::size;

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

    //! @brief Inserts element at end of circular array.
    using span::push_back;

    //! @brief Erases element from beginning of circular array.
    using span::pop_front;

    //! @brief Erases element from end of circular array.
    using span::pop_back;

    //! @brief Moves element from beginning of circular array.
    using span::move_front;

    //! @brief Moves element from end of circular array.
    using span::move_back;

    //! @brief Returns iterator to beginning of circular array.
    using span::begin;

    //! @brief Returns iterator to ending of circular array.
    using span::end;

    //! @brief Returns const iterator to beginning of circular array.
    using span::cbegin;

    //! @brief Returns const iterator to ending of circular array.
    using span::cend;
};

} // namespace circular
} // namespace trial

#include <trial/circular/detail/array.ipp>

#endif // TRIAL_CIRCULAR_ARRAY_HPP
