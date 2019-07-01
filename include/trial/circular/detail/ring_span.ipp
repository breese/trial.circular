///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

namespace trial
{
namespace circular
{

//-----------------------------------------------------------------------------
// Popper
//-----------------------------------------------------------------------------

template <typename T>
void null_popper<T>::operator()(T&) const noexcept
{
}

template <typename T>
T default_popper<T>::operator()(T& t) const noexcept(std::is_nothrow_move_constructible<T>::value)
{
    return std::move(t);
}

template <typename T>
copy_popper<T>::copy_popper(T&& t) noexcept(std::is_nothrow_move_constructible<T>::value)
    : copy(std::move(t))
{
}

template <typename T>
T copy_popper<T>::operator()(T& t) const noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_copy_assignable<T>::value)
{
    T old = std::move(t);
    t = copy;
    return old;
}

//-----------------------------------------------------------------------------
// ring_span<T>
//-----------------------------------------------------------------------------

template <typename T, typename P>
template <typename ContiguousIterator>
ring_span<T, P>::ring_span(ContiguousIterator begin,
                           ContiguousIterator end,
                           P popper) noexcept(std::is_nothrow_move_constructible<P>::value)
    : super(begin, end),
      popper(std::move(popper))
{
}

template <typename T, typename P>
template <typename ContiguousIterator>
ring_span<T, P>::ring_span(ContiguousIterator begin,
                           ContiguousIterator end,
                           ContiguousIterator first,
                           size_type length,
                           P popper) noexcept(std::is_nothrow_move_constructible<P>::value)
    : super(begin, end, first, length),
      popper(std::move(popper))
{
}

template <typename T, typename P>
auto ring_span<T, P>::pop_front() noexcept(noexcept(std::declval<P>().operator()(std::declval<value_type&>())))
{
    auto& old_front = super::front();
    super::pop_front(); // Element still lingers in storage
    return popper(old_front);
}

template <typename T, typename P>
template <typename... Args>
void ring_span<T, P>::emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible<value_type, Args...>::value && std::is_nothrow_move_assignable<value_type>::value)
{
    super::push_back(value_type(std::forward<Args>(args)...));
}

template <typename T, typename P>
void ring_span<T, P>::swap(type& other) noexcept(detail::is_nothrow_swappable<P>::value)
{
    using std::swap;
    swap(static_cast<super&>(*this), static_cast<super&>(other));
    swap(popper, other.popper);
}

} // namespace circular
} // namespace trial
