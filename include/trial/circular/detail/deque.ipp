///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2020 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <iterator> // std::move_iterator
#include <algorithm> // std::max

namespace trial
{
namespace circular
{

template <typename T, typename A>
deque<T, A>::deque(size_type replacement_capacity,
                   const A& alloc)
    : allocator_base(detail::empty_init_t{}, alloc),
      span{ allocate(get_allocator(), replacement_capacity), replacement_capacity }
{
    member.threshold = replacement_capacity;
}

template <typename T, typename A>
deque<T, A>::deque(const A& alloc) noexcept(std::is_nothrow_copy_constructible<A>::value)
    : allocator_base(detail::empty_init_t{}, alloc),
      span { nullptr, 0 }
{
}

template <typename T, typename A>
deque<T, A>::~deque()
{
    if (!std::is_trivially_destructible<T>::value)
    {
        for (size_type k = 0; k < capacity(); ++k)
        {
            destroy(get_allocator(), std::addressof(span::operator[](k)));
        }
    }
    deallocate(get_allocator(), span::data(), capacity());
}

template <typename T, typename A>
void deque<T, A>::push_front(value_type input)
{
    emplace_front(std::move(input));
}

template <typename T, typename A>
void deque<T, A>::push_back(value_type input)
{
    emplace_back(std::move(input));
}

template <typename T, typename A>
template <typename... Args>
void deque<T, A>::emplace_front(Args&&... args)
{
    if (span::full())
    {
        grow();
    }
    span::expand_front();
    construct(get_allocator(), std::addressof(front()), std::forward<Args>(args)...);
}

template <typename T, typename A>
template <typename... Args>
void deque<T, A>::emplace_back(Args&&... args)
{
    if (span::full())
    {
        grow();
    }
    span::expand_back();
    construct(get_allocator(), std::addressof(back()), std::forward<Args>(args)...);
}

template <typename T, typename A>
auto deque<T, A>::pop_front() -> value_type
{
    if (half())
    {
        shrink();
    }
    auto result = std::move(front());
    destroy(get_allocator(), std::addressof(front()));
    span::remove_front();
    return result;
}

template <typename T, typename A>
auto deque<T, A>::pop_back() -> value_type
{
    if (half())
    {
        shrink();
    }
    auto result = std::move(back());
    destroy(get_allocator(), std::addressof(back()));
    span::remove_back();
    return result;
}

template <typename T, typename A>
void deque<T, A>::reserve(size_type replacement_capacity)
{
    member.threshold = replacement_capacity;
    if (replacement_capacity > capacity())
    {
        grow();
    }
}

template <typename T, typename A>
auto deque<T, A>::get_allocator() noexcept -> allocator_type&
{
    return allocator_base::get();
}

template <typename T, typename A>
auto deque<T, A>::allocate(allocator_type& allocator, size_type length) -> pointer
{
    return std::allocator_traits<allocator_type>::allocate(allocator, length);
}

template <typename T, typename A>
void deque<T, A>::deallocate(allocator_type& allocator, pointer ptr, size_type length)
{
    return std::allocator_traits<allocator_type>::deallocate(allocator, ptr, length);
}

template <typename T, typename A>
template <typename... Args>
void deque<T, A>::construct(allocator_type& allocator, pointer ptr, Args&&... args)
{
    std::allocator_traits<allocator_type>::construct(allocator,
                                                     ptr,
                                                     std::forward<Args>(args)...);
}

template <typename T, typename A>
void deque<T, A>::destroy(allocator_type& allocator, pointer ptr)
{
    std::allocator_traits<allocator_type>::destroy(allocator, ptr);
}

template <typename T, typename A>
bool deque<T, A>::half() const
{
    return size() <= (capacity() / 2);
}

template <typename T, typename A>
void deque<T, A>::grow()
{
    // Growth factor of 3/2
    const auto grow_capacity = std::max(capacity() + (capacity() / 2), member.threshold);
    reallocate(grow_capacity);
}

template <typename T, typename A>
void deque<T, A>::shrink()
{
    // Shrink factor is 3/4
    const auto shrink_capacity = std::max((capacity() / 2) + (capacity() / 4), member.threshold);
    reallocate(shrink_capacity);
}

template <typename T, typename A>
void deque<T, A>::reallocate(size_type replacement_capacity)
{
    if (replacement_capacity == capacity())
        return;

    // Allocation and moving may throw
    auto deleter = [this, replacement_capacity] (element_type *ptr)
        {
            deallocate(get_allocator(), ptr, replacement_capacity);
        };
    auto storage = std::unique_ptr<element_type, decltype(deleter)>(allocate(get_allocator(), replacement_capacity), deleter);
    if (capacity() > 0)
    {
        // Move into uninitialized storage
        std::uninitialized_copy(std::make_move_iterator(span::begin()),
                                std::make_move_iterator(span::end()),
                                storage.get()); // May throw if element_type not nothrow move assignable
    }

    // Destroy old storage
    if (!std::is_trivially_destructible<T>::value)
    {
        for (size_type k = 0; k < capacity(); ++k)
        {
            destroy(get_allocator(), std::addressof(span::operator[](k)));
        }
    }
    deallocate(get_allocator(), span::data(), capacity());

    // Reseat the span
    auto beginning = storage.release();
    span::operator=(span(beginning, beginning + replacement_capacity, beginning, size()));
}

} // namespace circular
} // namespace trial
