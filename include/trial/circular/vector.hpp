#ifndef TRIAL_CIRCULAR_VECTOR_HPP
#define TRIAL_CIRCULAR_VECTOR_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <trial/circular/span.hpp>
#include <trial/circular/detail/type_traits.hpp>

namespace trial
{
namespace circular
{

//! @brief Dynamically resizable circular buffer.
//!
//! Size is the current number of elements in the buffer.
//!
//! Capacity is the maximum number of elements that can be inserted without
//! overwriting old elements. Capacity can only be changed by explicit calls
//! to @c reserve() or @c resize().
//!
//! Violation of any precondition results in undefined behavior.

template <typename T, typename Allocator = typename std::vector<T>::allocator_type>
class vector
    : private std::vector<T, Allocator>,
      private circular::span<T>
{
    using storage = std::vector<T, Allocator>;
    using span = circular::template span<T>;

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
    using allocator_type = Allocator;

    //! @brief Creates an empty circular vector with no capacity.
    //!
    //! @post capacity() == 0
    //! @post size() == 0

    vector() noexcept(std::is_nothrow_default_constructible<storage>::value);

    //! @brief Creates empty circular vector with no capacity using allocator.
    //!
    //! @post capacity() == 0
    //! @post size() == 0

    explicit vector(const allocator_type&) noexcept(std::is_nothrow_constructible<storage, const allocator_type&>::value);

    //! @brief Creates circular vector by copy construction.
    //!
    //! @post capacity() == other.capacity()
    //! @post size() == other.size()

    vector(const vector& other);

    //! @brief Creates circular vector by copy construction with allocator.
    //!
    //! @post capacity() == other.capacity()
    //! @post size() == other.size()

    vector(const vector&,
           const allocator_type&);

    //! @brief Creates circular vector by move construction.
    //!
    //! @post capacity() == other.capacity()
    //! @post size() == other.size()

    vector(vector&& other) noexcept(std::is_nothrow_move_constructible<storage>::value) = default;

    //! @brief Creates circular vector by move construction with allocator.
    //!
    //! @post capacity() == other.capacity()
    //! @post size() == other.size()

    vector(vector&& other,
           const allocator_type&) noexcept(std::is_nothrow_constructible<storage, storage&&, const allocator_type&>::value);

    //! @brief Recreates circular vector by copy assignment.
    //!
    //! @post capacity() == other.capacity()
    //! @post size() == other.size()

    vector& operator=(const vector& other);

    //! @brief Recreates circular vector by move assignment.
    //!
    //! @post capacity() == other.capacity()
    //! @post size() == other.size()

    vector& operator=(vector&& other) noexcept(std::is_nothrow_move_assignable<storage>::value) = default;

    //! @brief Creates empty circular vector with capacity.
    //!
    //! @pre T must be default constructible.
    //!
    //! @post capacity() == capacity
    //! @post size() == 0

    explicit vector(size_type capacity);

    //! @brief Creates empty circular vector with capacity using allocator.
    //!
    //! @pre T must be default constructible.
    //!
    //! @post capacity() == capacity
    //! @post size() == 0

    vector(size_type capacity,
           const allocator_type&);

    //! @brief Creates circular vector with element from initializer list.
    //!
    //! @post capacity() == input.size()
    //! @post size() == input.size()

    vector(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Creates circular vector with element from initializer list with allocator.
    //!
    //! @post capacity() == input.size()
    //! @post size() == input.size()

    vector(std::initializer_list<value_type> input,
           const allocator_type&) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Recreates circular vector from initializer list.
    //!
    //! @post capacity() == input.size()
    //! @post size() == input.size()

    vector& operator=(std::initializer_list<value_type> input) noexcept(std::is_nothrow_move_assignable<value_type>::value);

    //! @brief Creates circular vector with elements from range.
    //!
    //! @post capacity() == std::distance(first, last)
    //! @post size() == capacity()

    template <typename InputIterator>
    vector(InputIterator first,
           InputIterator last) noexcept(std::is_nothrow_copy_assignable<value_type>::value);

    //! @brief Creates circular vector with elements from range using allocator.
    //!
    //! @post capacity() == std::distance(first, last)
    //! @post size() == capacity()

    template <typename InputIterator>
    vector(InputIterator first,
           InputIterator last,
           const allocator_type&) noexcept(std::is_nothrow_copy_assignable<value_type>::value);

    //! @brief Returns underlying allocator.

    allocator_type get_allocator() const;

    //! @brief Checks if circular vector is empty.

    using span::empty;

    //! @brief Checks if circular vector is full.

    using span::full;

    //! @brief Returns the maximum possible number of elements in circular vector.

    using storage::capacity;

    //! @brief Increases capacity of circular vector.
    //!
    //! If the requested capacity is less than the current capacity then nothing
    //! is modified. Otherwise, the current capacity is increased to the requested
    //! capacity. The storage is normalized before the capacity is increased.
    //!
    //! @pre capacity <= max_size()
    //!
    //! @post capacity() == std::max(capacity, capacity())
    //! @post is_normalized()

    void reserve(size_type capacity);

    //! @brief Returns the number of elements in circular vector.

    using span::size;

    //! @brief Returns the maximum number of possible elements in circular vector.

    using storage::max_size;

    //! @brief Increases the number of elements by default construction.

    void resize(size_type count);

    //! @brief Increases the number of elements by copy construction.
    //!
    //! If the requested resize count is less than the current size, the storage
    //! is reduced to the requested count, and excess elements are erased as if
    //! then have been removed with multiple @c pop_back calls. Otherwise the
    //! storage is grown to the requested count, and new elements are copy
    //! constructed from @c input.
    //!
    //! The storage is normalized before resize.
    //!
    //! @post size() == count
    //! @post is_normalized()

    void resize(size_type count, const value_type& input);

    //! @brief Removes excess capacity.

    using storage::shrink_to_fit;

    //! @brief Returns reference to first element in circular vector.

    using span::front;

    //! @brief Returns reference to last element in span.

    using span::back;

    //! @brief Returns reference to element at position.

    using span::operator[];

    //! @brief Clears the circular vector.

    using span::clear;

    //! @brief Clears circular vector and inserts element at end of circular vector.

    using span::assign;

    //! @brief Inserts element at beginning of circular vector.
    //!
    //! Time complexity is amortized constant when there is spare capacity,
    //! otherwise it is linear.

    void push_front(value_type);

    //! @brief Inserts element at end of circular vector.

    void push_back(value_type);

    //! @brief Removes and returns element from beginning of circular vector.

    using span::pop_front;

    //! @brief Removes and returns element from end of circular vector.

    using span::pop_back;

    //! @brief Inserts uninitialized element at beginning of circular vector.

    using span::append_front;

    //! @brief Inserts uninitialized element at end of circular vector.

    using span::append_back;

    //! @brief Erases element from beginning of circular vector.

    using span::remove_front;

    //! @brief Erases element from end of circular vector.

    using span::remove_back;

    //! @brief Returns iterator to beginning of circular vector.

    using span::begin;

    //! @brief Returns iterator to ending of circular vector.

    using span::end;

    //! @brief Returns const iterator to beginning of circular vector.

    using span::cbegin;

    //! @brief Returns const iterator to ending of circular vector.

    using span::cend;

    //! @brief Returns reverse iterator to beginning of circular array.

    using span::rbegin;

    //! @brief Returns reverse iterator to ending of circular array.

    using span::rend;

    //! @brief Returns const reverse iterator to beginning of circular array.

    using span::crbegin;

    //! @brief Returns const reverse iterator to ending of circular array.

    using span::crend;
};

} // namespace circular
} // namespace trial

#include <trial/circular/detail/vector.ipp>

#endif // TRIAL_CIRCULAR_VECTOR_HPP
