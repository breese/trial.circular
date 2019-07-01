#ifndef TRIAL_CIRCULAR_RING_SPAN_HPP
#define TRIAL_CIRCULAR_RING_SPAN_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <trial/circular/span.hpp>

namespace trial
{
namespace circular
{

// Implementation of P0059R4 std::experimental::ring_span

template <typename T>
struct null_popper
{
    null_popper() noexcept = default;
    void operator()(T&) const noexcept;
};

template <typename T>
struct default_popper
{
    default_popper() noexcept = default;
    T operator()(T&) const noexcept(std::is_nothrow_move_constructible<T>::value);
};

template <typename T>
struct copy_popper
{
    explicit copy_popper(T&&) noexcept(std::is_nothrow_move_constructible<T>::value);
    T operator()(T&) const noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_copy_assignable<T>::value);
    T copy;
};

template <typename T, typename Popper = default_popper<T>>
class ring_span
    : private circular::span<T>
{
    using super = typename circular::span<T>;

public:
    using type = ring_span<T, Popper>;
    using value_type = typename super::value_type;
    using size_type = typename super::size_type;
    using pointer = typename super::pointer;
    using reference = typename super::reference;
    using const_reference = typename super::const_reference;
    // Mutable iterator not available
    using const_iterator = typename super::const_iterator;

    template <typename ContiguousIterator>
    ring_span(ContiguousIterator begin,
              ContiguousIterator end,
              Popper p = Popper()) noexcept(std::is_nothrow_move_constructible<Popper>::value);
        
    template <typename ContiguousIterator>
    ring_span(ContiguousIterator begin,
              ContiguousIterator end,
              ContiguousIterator first,
              size_type length,
              Popper p = Popper()) noexcept(std::is_nothrow_move_constructible<Popper>::value);

    ring_span(ring_span&&) = default;
    ring_span& operator=(ring_span&&) = default;

    using super::empty;
    using super::full;
    using super::size;
    using super::capacity;

    using super::front;
    using super::back;

    using super::begin;
    using super::end;
    using super::cbegin;
    using super::cend;

    using super::push_back;
    template <typename... Args>
    void emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible<value_type, Args...>::value && std::is_nothrow_move_assignable<value_type>::value);
    auto pop_front() noexcept(noexcept(std::declval<Popper>().operator()(std::declval<value_type&>()))); // C++14 auto return type
    void swap(type&) noexcept(detail::is_nothrow_swappable<Popper>::value);

private:
    Popper popper;
};

} // namespace circular
} // namespace trial

#include <trial/circular/detail/ring_span.ipp>

#endif // TRIAL_CIRCULAR_RING_SPAN_HPP
