#ifndef TRIAL_CIRCULAR_DEQUE_HPP
#define TRIAL_CIRCULAR_DEQUE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2020 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <type_traits>
#include <trial/circular/span.hpp>

namespace trial
{
namespace circular
{

//! @brief Dynamically resizable circular buffer.
//!
//! The internal storage is only reallocated when inserting an element into a
//! full queue, or removing an element from a half-full queue.
//!
//! Violation of any precondition results in undefined behavior.

template <typename T, typename Allocator = std::allocator<T>>
class deque
    : private Allocator, // FIXME: empty_value<Allocator>
      private circular::span<T>
{
    using span = circular::template span<T>;

public:
    using element_type = typename span::element_type;
    using value_type = typename span::value_type;
    using size_type = typename span::size_type;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<element_type>;

    //! @brief Creates an empty circular queue.
    //!
    //! @post capacity() == 0
    //! @post size() == 0

    deque() noexcept = default;

    //! @brief Creates an empty circular queue with capacity.
    //!
    //! @post capacity() == capacity
    //! @post size() == 0

    explicit deque(size_type capacity);

    // FIXME: const allocator_type& argument
    // FIXME: copy/move etc.
    // FIXME: assign, operator=

    ~deque();

    //! @brief Checks if circular queue is empty.

    using span::empty;

    //! @brief Returns the number of elements in circular queue.

    using span::size;

    //! @brief Returns the maximum possible number of elements in circular queue.

    using span::capacity;

    //! @brief Returns reference to first element in circular queue.

    using span::front;

    //! @brief Returns reference to last element in circular queue.

    using span::back;

    //! @brief Returns reference to element at position.

    using span::operator[];

    //! @brief Clears the circular queue.

    using span::clear;

    //! @brief Inserts element at beginning of circular queue.
    //!
    //! Time complexity is amortized constant when there is spare capacity,
    //! otherwise it is linear.

    void push_front(value_type);

    //! @brief Inserts element at end of circular queue.

    void push_back(value_type);

    //! @brief Creates element at beginning of circular queue.

    template <typename... Args>
    void emplace_front(Args&&...);

    //! @brief Creates element at end of circular queue.

    template <typename... Args>
    void emplace_back(Args&&...);

    //! @brief Removes and returns element from beginning of circular queue.

    value_type pop_front();

    //! @brief Removes and returns element from end of circular queue.

    value_type pop_back();

    //! @brief Increases capacity of circular queue.
    //!
    //! Set the minimum capacity.

    void reserve(size_type);

    //! @brief Returns underlying allocator.

    allocator_type& get_allocator() noexcept;

    //! @brief Returns iterator to beginning of circular queue.

    using span::begin;

    //! @brief Returns iterator to ending of circular queue.

    using span::end;

    //! @brief Returns const iterator to beginning of circular queue.

    using span::cbegin;

    //! @brief Returns const iterator to ending of circular queue.

    using span::cend;

private:
    using pointer = typename span::pointer;

    static pointer allocate(allocator_type, size_type);
    static void deallocate(allocator_type, pointer, size_type);
    template <typename... Args>
    static void construct(allocator_type, pointer, Args&&...);
    static void destroy(allocator_type, pointer);

    bool half() const;
    void grow();
    void shrink();
    void reallocate(size_type);

private:
    struct
    {
        // Minimum capacity reserved by user
        size_type threshold = 2;
    } member;
};

} // namespace circular
} // namespace trial

#include <trial/circular/detail/deque.ipp>

#endif // TRIAL_CIRCULAR_DEQUE_HPP
