///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <trial/detail/lightweight_test.hpp>
#include <trial/circular/vector.hpp>

using namespace trial;

//-----------------------------------------------------------------------------

namespace test
{

template <typename T>
struct allocator
{
    using value_type = T;
    using size_type = std::size_t;
    using pointer = T*;

    pointer allocate(size_type size)
    {
        count.allocate += size;
        return static_cast<pointer>(::operator new(size * sizeof(value_type)));
    }

    void deallocate(pointer p, size_type size)
    {
        count.deallocate += size;
        ::operator delete(p);
    }

    friend bool operator==(const allocator& lhs, const allocator& rhs)
    {
        return std::addressof(lhs) == std::addressof(rhs);
    }

    friend bool operator!=(const allocator& lhs, const allocator& rhs)
    {
        return !(lhs == rhs);
    }

    struct
    {
        size_type allocate = 0;
        size_type deallocate = 0;
    } count;
};

} // namespace test

//-----------------------------------------------------------------------------

namespace api_suite
{

void api_ctor_default()
{
    circular::vector<int> data;
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 0);
}

void api_ctor_default_alloc()
{
    std::allocator<int> allocator;
    circular::vector<int> data(allocator);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 0);
}

void api_ctor_copy()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(data);
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_copy_alloc()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(data, allocator);
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_move()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data));
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_move_alloc()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data), allocator);
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_copy_assign()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(2);
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 2);
    copy = data;
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_move_assign()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(2);
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 2);
    copy = std::move(data);
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_capacity()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void api_ctor_capacity_alloc()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void api_ctor_initializer_list()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 2);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void api_ctor_initializer_list_alloc()
{
    std::allocator<int> allocator;
    circular::vector<int> data({ 11, 22 }, allocator);
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 2);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void api_ctor_iterator()
{
    std::vector<int> input = { 11, 22 };
    circular::vector<int> data(input.begin(), input.end());
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 2);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void api_ctor_iterator_alloc()
{
    std::allocator<int> allocator;
    std::vector<int> input({ 11, 22 }, allocator);
    circular::vector<int> data(input.begin(), input.end(), allocator);
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 2);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void api_assign_initializer_list()
{
    circular::vector<int> data(4);
    data.assign({ 11, 22 });
    TRIAL_TEST_EQ(data.size(), 2);
}

void api_assign_iterator()
{
    circular::vector<int> data(4);
    std::vector<int> input = { 11, 22 };
    data.assign(input.begin(), input.end());
    TRIAL_TEST_EQ(data.size(), 2);
}

void api_empty()
{
    circular::vector<int> data(4);
    TRIAL_TEST(data.empty());
}

void api_capacity()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void api_reserve()
{
    circular::vector<int> data(4);
    data.reserve(2);
    TRIAL_TEST_EQ(data.capacity(), 4);
    data.reserve(8);
    TRIAL_TEST_EQ(data.capacity(), 8);
}

void api_shrink_to_fit()
{
    circular::vector<int> data(4);
    data.shrink_to_fit(); // std::vector decides
}

void api_size()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_max_size()
{
    circular::vector<int> data(4);
    TRIAL_TEST(data.max_size() >= 4U);
}

void api_resize()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    data.resize(2);
    TRIAL_TEST_EQ(data.size(), 2);
    data.resize(4, 42);
    TRIAL_TEST_EQ(data.size(), 4);
}

void api_full()
{
    circular::vector<int> data(4);
    TRIAL_TEST(!data.full());
}

void api_front()
{
    circular::vector<int> data(4);
    data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data.front(), 11);
    data.front() = 22;
    TRIAL_TEST_EQ(data.front(), 22);
}

void api_front_const()
{
    const circular::vector<int> data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data.front(), 11);
}

void api_back()
{
    circular::vector<int> data(4);
    data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data.back(), 44);
    data.back() = 55;
    TRIAL_TEST_EQ(data.back(), 55);
}

void api_back_const()
{
    const circular::vector<int> data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data.back(), 44);
}

void api_operator_index()
{
    circular::vector<int> data(4);
    data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data[0], 11);
    TRIAL_TEST_EQ(data[1], 22);
    TRIAL_TEST_EQ(data[2], 33);
    TRIAL_TEST_EQ(data[3], 44);
    data.push_back(55);
    TRIAL_TEST_EQ(data[0], 22);
    TRIAL_TEST_EQ(data[1], 33);
    TRIAL_TEST_EQ(data[2], 44);
    TRIAL_TEST_EQ(data[3], 55);
    data[0] = 11;
    TRIAL_TEST_EQ(data[0], 11);
    TRIAL_TEST_EQ(data[1], 33);
    TRIAL_TEST_EQ(data[2], 44);
    TRIAL_TEST_EQ(data[3], 55);
}

void api_operator_index_const()
{
    const circular::vector<int> data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data[0], 11);
    TRIAL_TEST_EQ(data[1], 22);
    TRIAL_TEST_EQ(data[2], 33);
    TRIAL_TEST_EQ(data[3], 44);
}

void api_clear()
{
    circular::vector<int> data(4);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.clear();
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_push_front()
{
    circular::vector<int> data(4);
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void api_push_back()
{
    circular::vector<int> data(4);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void api_move_front()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.move_front(), 11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void api_move_back()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.move_back(), 22);
    TRIAL_TEST_EQ(data.size(), 1);
}

void api_increment_front()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.increment_front();
    TRIAL_TEST_EQ(data.size(), 2); // capacity
}

void api_increment_front_n()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.increment_front(2);
    TRIAL_TEST_EQ(data.size(), 2); // capacity
}

void api_decrement_front()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.decrement_front();
    TRIAL_TEST_EQ(data.size(), 1);
}

void api_decrement_front_n()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.decrement_front(2);
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_increment_back()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.increment_back();
    TRIAL_TEST_EQ(data.size(), 2); // capacity
}

void api_increment_back_n()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.increment_back(2);
    TRIAL_TEST_EQ(data.size(), 2); // capacity
}

void api_decrement_back()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.decrement_back();
    TRIAL_TEST_EQ(data.size(), 1);
}

void api_decrement_back_n()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.decrement_back(2);
    TRIAL_TEST_EQ(data.size(), 0);
}

void run()
{
    api_ctor_default();
    api_ctor_default_alloc();
    api_ctor_copy();
    api_ctor_copy_alloc();
    api_ctor_move();
    api_ctor_move_alloc();
    api_ctor_copy_assign();
    api_ctor_move_assign();
    api_ctor_capacity();
    api_ctor_capacity_alloc();
    api_ctor_initializer_list();
    api_ctor_initializer_list_alloc();
    api_ctor_iterator();
    api_ctor_iterator_alloc();
    api_assign_initializer_list();
    api_assign_iterator();
    api_empty();
    api_capacity();
    api_reserve();
    api_shrink_to_fit();
    api_size();
    api_max_size();
    api_resize();
    api_full();
    api_front();
    api_front_const();
    api_back();
    api_back_const();
    api_operator_index();
    api_operator_index_const();
    api_clear();
    api_push_front();
    api_push_back();
    api_move_front();
    api_move_back();
    api_increment_front();
    api_increment_front_n();
    api_decrement_front();
    api_decrement_front_n();
    api_increment_back();
    api_increment_back_n();
    api_decrement_back();
    api_decrement_back_n();
}

} // namespace api_suite

//-----------------------------------------------------------------------------

namespace assign_suite
{

void assign_below_capacity()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    data = {11, 22, 33};
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::vector<int> expect = {11, 22, 33};
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void assign_at_capacity()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::vector<int> expect = {11, 22, 33, 44};
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void assign_above_capacity()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    data = {11, 22, 33, 44, 55};
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::vector<int> expect = {22, 33, 44, 55};
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void assign_after_resize()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    data = {11, 22, 33, 44 };
    data.resize(2);
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    // Triggers internal resize
    data = { 11, 22, 33, 44 };
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    assign_below_capacity();
    assign_at_capacity();
    assign_above_capacity();
    assign_after_resize();
}

} // namespace assign_suite

//-----------------------------------------------------------------------------

namespace move_suite
{

void ctor_move_alloc_3()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    data = { 11, 22, 33 };
    TRIAL_TEST_EQ(data.size(), 3);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data), allocator);
    TRIAL_TEST_EQ(copy.size(), 3);
    TRIAL_TEST_EQ(copy.capacity(), 4);
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_move_alloc_4()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    data = { 11, 22, 33, 44 };
    TRIAL_TEST_EQ(data.size(), 4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data), allocator);
    TRIAL_TEST_EQ(copy.size(), 4);
    TRIAL_TEST_EQ(copy.capacity(), 4);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_move_alloc_5()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    data = { 11, 22, 33, 44, 55 };
    TRIAL_TEST_EQ(data.size(), 4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data), allocator);
    TRIAL_TEST_EQ(copy.size(), 4);
    TRIAL_TEST_EQ(copy.capacity(), 4);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_move_alloc_6()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    data = { 11, 22, 33, 44, 55, 66 };
    TRIAL_TEST_EQ(data.size(), 4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data), allocator);
    TRIAL_TEST_EQ(copy.size(), 4);
    TRIAL_TEST_EQ(copy.capacity(), 4);
    {
        std::vector<int> expect = { 33, 44, 55, 66 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_move_alloc_7()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    data = { 11, 22, 33, 44, 55, 66, 77 };
    TRIAL_TEST_EQ(data.size(), 4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data), allocator);
    TRIAL_TEST_EQ(copy.size(), 4);
    TRIAL_TEST_EQ(copy.capacity(), 4);
    {
        std::vector<int> expect = { 44, 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_move_alloc_8()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    data = { 11, 22, 33, 44, 55, 66, 77, 88 };
    TRIAL_TEST_EQ(data.size(), 4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data), allocator);
    TRIAL_TEST_EQ(copy.size(), 4);
    TRIAL_TEST_EQ(copy.capacity(), 4);
    {
        std::vector<int> expect = { 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    ctor_move_alloc_3();
    ctor_move_alloc_4();
    ctor_move_alloc_5();
    ctor_move_alloc_6();
    ctor_move_alloc_7();
    ctor_move_alloc_8();
}

} // namespace move_suite

//-----------------------------------------------------------------------------

namespace capacity_suite
{

void reserve_default_constructed()
{
    circular::vector<int> data;
    TRIAL_TEST_EQ(data.capacity(), 0);
    TRIAL_TEST_EQ(data.size(), 0);
    data.reserve(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.size(), 0);
    data.reserve(2); // Does not shrink
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.size(), 0);
    data.reserve(8); // Grows
    TRIAL_TEST_EQ(data.capacity(), 8);
    TRIAL_TEST_EQ(data.size(), 0);
}

void reserve_push_back()
{
    circular::vector<int> data(4);
    data = { 11, 22, 33 };
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_back(44);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    // 11 22 33 44 => 11 22 33 44 X X
    data.reserve(6);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_back(55);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_back(66);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_back(77);
    {
        std::vector<int> expect = { 22, 33, 44, 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void reserve_overfull_push_back()
{
    circular::vector<int> data(4);
    data = { 11, 22, 33, 44, 55 };
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    // 22 33 44 55 => 22 33 44 55 X X
    data.reserve(6);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_back(66);
    {
        std::vector<int> expect = { 22, 33, 44, 55, 66 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_back(77);
    {
        std::vector<int> expect = { 22, 33, 44, 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_back(88);
    {
        std::vector<int> expect = { 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void reserve_push_front()
{
    circular::vector<int> data(4);
    data = { 33, 22, 11 };
    {
        std::vector<int> expect = { 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_front(44);
    {
        std::vector<int> expect = { 44, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    // 44 33 22 11 => 44 33 22 11 X X
    data.reserve(6);
    {
        std::vector<int> expect = { 44, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_front(55);
    {
        std::vector<int> expect = { 55, 44, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_front(66);
    {
        std::vector<int> expect = { 66, 55, 44, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_front(77);
    {
        std::vector<int> expect = { 77, 66, 55, 44, 33, 22 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void reserve_overfull_push_front()
{
    circular::vector<int> data(4);
    data = { 11, 22, 33, 44, 55 };
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    // 55 22 33 44 => 22 33 44 55 X X
    data.reserve(6);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    // 22 33 44 55 X X => 22 33 44 55 X 66
    data.push_front(66);
    {
        std::vector<int> expect = { 66, 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    // 22 33 44 55 X 66 => 22 33 44 55 77 66
    data.push_front(77);
    {
        std::vector<int> expect = { 77, 66, 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_front(88);
    {
        std::vector<int> expect = { 88, 77, 66, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void reserve_one()
{
    // First
    {
        circular::vector<int> data(4);
        data = { 01, 11, 22, 33 };
        data.decrement_front();
        // X 11 22 33 => 11 22 33 X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
    // Second
    {
        circular::vector<int> data(4);
        data = { 01, 02, 11, 22, 33 };
        data.decrement_front();
        // 33 X 11 22 => 11 22 33 X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
    // Third
    {
        circular::vector<int> data(4);
        data = { 01, 02, 03, 11, 22, 33 };
        data.decrement_front();
        // 22 33 X 11 => 11 22 33 X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
    // Fourth
    {
        circular::vector<int> data(4);
        data = { 01, 02, 02, 04, 11, 22, 33 };
        data.decrement_front();
        // 11 22 33 X => 11 22 33 X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
}

void reserve_two()
{
    // First
    {
        circular::vector<int> data(4);
        data = { 01, 02, 11, 22 };
        data.decrement_front(2);
        // X X 11 22 => 11 22 X X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11, 22 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(33);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
    // Second
    {
        circular::vector<int> data(4);
        data = { 01, 02, 03, 11, 22 };
        data.decrement_front(2);
        // 22 X X 11 => 11 22 X X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11, 22 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(33);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
    // Third
    {
        circular::vector<int> data(4);
        data = { 01, 02, 03, 04, 11, 22 };
        data.decrement_front(2);
        // 11 22 X X => 11 22 X X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11, 22 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(33);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
    // Fourth
    {
        circular::vector<int> data(4);
        data = { 01, 02, 03, 04, 05, 11, 22 };
        data.decrement_front(2);
        // X 11 22 X => 11 22 X X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11, 22 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(33);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
}

void reserve_three()
{
    // First
    {
        circular::vector<int> data(4);
        data = { 01, 02, 03, 11 };
        data.decrement_front(3);
        // X X X 11 => 11 X X X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(22);
        {
            std::vector<int> expect = { 11, 22 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(33);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
    // Second
    {
        circular::vector<int> data(4);
        data = { 01, 02, 03, 04, 11 };
        data.decrement_front(3);
        // 11 X X X => 11 X X X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(22);
        {
            std::vector<int> expect = { 11, 22 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(33);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
    // Third
    {
        circular::vector<int> data(4);
        data = { 01, 02, 03, 04, 05, 11 };
        data.decrement_front(3);
        // X 11 X X => 11 X X X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(22);
        {
            std::vector<int> expect = { 11, 22 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(33);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
    // Fourth
    {
        circular::vector<int> data(4);
        data = { 01, 02, 03, 04, 05, 06, 11 };
        data.decrement_front(3);
        // X X 11 X => 11 X X X X X
        data.reserve(6);
        {
            std::vector<int> expect = { 11 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(22);
        {
            std::vector<int> expect = { 11, 22 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(33);
        {
            std::vector<int> expect = { 11, 22, 33 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(44);
        {
            std::vector<int> expect = { 11, 22, 33, 44 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
        data.push_back(55);
        {
            std::vector<int> expect = { 11, 22, 33, 44, 55 };
            TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                              expect.begin(), expect.end());
        }
    }
}

void resize_empty()
{
    circular::vector<int> data(4);
    data.resize(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::vector<int> expect = { 0, 0, 0, 0 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    // Increase capacity
    data.resize(8, 11);
    TRIAL_TEST_EQ(data.capacity(), 8);
    {
        std::vector<int> expect = { 0, 0, 0, 0, 11, 11, 11, 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void resize_default_constructed()
{
    circular::vector<int> data;
    TRIAL_TEST_EQ(data.capacity(), 0);
    data.resize(4, 11);
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::vector<int> expect = { 11, 11, 11, 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void resize_smaller()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    data.resize(3, 11);
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::vector<int> expect = { 11, 11, 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.resize(2);
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::vector<int> expect = { 11, 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data = { 11, 22, 33 };
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.resize(2);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void resize_push_back()
{
    circular::vector<int> data(4);
    data = { 11, 22, 33, 44 };
    data.resize(8, 55);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 55, 55, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_back(66);
    {
        std::vector<int> expect = { 22, 33, 44, 55, 55, 55, 55, 66 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void resize_push_front()
{
    circular::vector<int> data(4);
    data = { 11, 22, 33, 44 };
    data.resize(8, 55);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 55, 55, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
    data.push_front(66);
    {
        std::vector<int> expect = { 66, 11, 22, 33, 44, 55, 55, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    reserve_default_constructed();
    reserve_push_back();
    reserve_overfull_push_back();
    reserve_push_front();
    reserve_overfull_push_front();
    reserve_one();
    reserve_two();
    reserve_three();

    resize_empty();
    resize_default_constructed();
    resize_smaller();
    resize_push_back();
    resize_push_front();
}

} // namespace capacity_suite

//-----------------------------------------------------------------------------

namespace allocator_suite
{

void ctor_default()
{
    circular::vector<int, test::allocator<int>> data;
    TRIAL_TEST_EQ(data.capacity(), 0);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 0);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
}

void ctor_default_alloc()
{
    test::allocator<int> allocator;
    circular::vector<int, decltype(allocator)> data(allocator);
    TRIAL_TEST_EQ(data.capacity(), 0);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 0);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
}

void ctor_capacity()
{
    circular::vector<int, test::allocator<int>> data(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
}

void ctor_capacity_alloc()
{
    test::allocator<int> allocator;
    circular::vector<int, decltype(allocator)> data(4, allocator);
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
}

void ctor_copy()
{
    circular::vector<int, test::allocator<int>> data(4);
    data = { 11, 22, 33, 44, 55 };
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    circular::vector<int, test::allocator<int>> copy(data);
    TRIAL_TEST_EQ(copy.capacity(), 4);
    TRIAL_TEST_EQ(copy.get_allocator().count.allocate, 8);
    TRIAL_TEST_EQ(copy.get_allocator().count.deallocate, 0);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(copy.begin(), copy.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_copy_alloc()
{
    test::allocator<int> allocator;
    circular::vector<int, decltype(allocator)> data(4, allocator);
    data = { 11, 22, 33, 44, 55 };
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    circular::vector<int, test::allocator<int>> copy(data);
    TRIAL_TEST_EQ(copy.capacity(), 4);
    TRIAL_TEST_EQ(copy.get_allocator().count.allocate, 8);
    TRIAL_TEST_EQ(copy.get_allocator().count.deallocate, 0);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(copy.begin(), copy.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_move()
{
    circular::vector<int, test::allocator<int>> data(4);
    data = { 11, 22, 33, 44, 55 };
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    circular::vector<int, test::allocator<int>> copy(std::move(data));
    TRIAL_TEST_EQ(copy.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(copy.begin(), copy.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_move_alloc()
{
    test::allocator<int> allocator;
    circular::vector<int, decltype(allocator)> data(4, allocator);
    data = { 11, 22, 33, 44, 55 };
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    circular::vector<int, test::allocator<int>> copy(std::move(data), allocator);
    TRIAL_TEST_EQ(copy.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(copy.begin(), copy.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_initializer_list()
{
    circular::vector<int, test::allocator<int>> data = { 11, 22, 33, 44 };
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_initializer_list_alloc()
{
    test::allocator<int> allocator;
    circular::vector<int, test::allocator<int>> data({ 11, 22, 33, 44 }, allocator);
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_iterator()
{
    std::vector<int, test::allocator<int>> input = { 11, 22, 33, 44 };
    circular::vector<int, test::allocator<int>> data(input.begin(), input.end());
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void ctor_iterator_alloc()
{
    test::allocator<int> allocator;
    std::vector<int, test::allocator<int>> input({ 11, 22, 33, 44 }, allocator);
    circular::vector<int, test::allocator<int>> data(input.begin(), input.end(), allocator);
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void assign_beyond_capacity()
{
    circular::vector<int, test::allocator<int>> data(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    data = { 11, 22, 33, 44, 55, 66, 77 };
    // Circular assignment does not cause additional container allocation
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
}

void assign_beyond_capacity_alloc()
{
    test::allocator<int> allocator;
    circular::vector<int, test::allocator<int>> data(4, allocator);
    TRIAL_TEST_EQ(data.capacity(), 4);
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
    data = { 11, 22, 33, 44, 55, 66, 77 };
    TRIAL_TEST_EQ(data.get_allocator().count.allocate, 4);
    TRIAL_TEST_EQ(data.get_allocator().count.deallocate, 0);
}

void run()
{
    ctor_default();
    ctor_default_alloc();
    ctor_capacity();
    ctor_capacity_alloc();
    ctor_copy();
    ctor_copy_alloc();
    ctor_move();
    ctor_move_alloc();
    ctor_initializer_list();
    ctor_initializer_list_alloc();
    ctor_iterator();
    ctor_iterator_alloc();
    assign_beyond_capacity();
    assign_beyond_capacity_alloc();
}

} // namespace allocator_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    api_suite::run();
    assign_suite::run();
    move_suite::run();
    capacity_suite::run();
    allocator_suite::run();

    return boost::report_errors();
}
