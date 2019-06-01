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
        return static_cast<pointer>(::operator new(size));
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

void ctor_default()
{
    circular::vector<int> data;
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 0);
}

void ctor_default_alloc()
{
    std::allocator<int> allocator;
    circular::vector<int> data(allocator);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 0);
}

void ctor_copy()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(data);
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void ctor_copy_alloc()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(data, allocator);
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void ctor_move()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data));
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void ctor_move_alloc()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
    circular::vector<int> copy(std::move(data), allocator);
    TRIAL_TEST_EQ(copy.size(), 0);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void ctor_copy_assign()
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

void ctor_move_assign()
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

void ctor_capacity()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void ctor_capacity_alloc()
{
    std::allocator<int> allocator;
    circular::vector<int> data(4, allocator);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void ctor_initializer_list()
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

void ctor_initializer_list_alloc()
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

void ctor_iterator()
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

void ctor_iterator_alloc()
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

void assign_initializer_list()
{
    circular::vector<int> data(4);
    data.assign({ 11, 22 });
    TRIAL_TEST_EQ(data.size(), 2);
}

void assign_iterator()
{
    circular::vector<int> data(4);
    std::vector<int> input = { 11, 22 };
    data.assign(input.begin(), input.end());
    TRIAL_TEST_EQ(data.size(), 2);
}

void empty()
{
    circular::vector<int> data(4);
    TRIAL_TEST(data.empty());
}

void capacity()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void size()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.size(), 0);
}

void max_size()
{
    circular::vector<int> data(4);
    TRIAL_TEST_EQ(data.max_size(), 4);
}

void full()
{
    circular::vector<int> data(4);
    TRIAL_TEST(!data.full());
}

void front()
{
    circular::vector<int> data(4);
    data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data.front(), 11);
    data.front() = 22;
    TRIAL_TEST_EQ(data.front(), 22);
}

void front_const()
{
    const circular::vector<int> data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data.front(), 11);
}

void back()
{
    circular::vector<int> data(4);
    data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data.back(), 44);
    data.back() = 55;
    TRIAL_TEST_EQ(data.back(), 55);
}

void back_const()
{
    const circular::vector<int> data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data.back(), 44);
}

void operator_index()
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

void operator_index_const()
{
    const circular::vector<int> data = {11, 22, 33, 44};
    TRIAL_TEST_EQ(data[0], 11);
    TRIAL_TEST_EQ(data[1], 22);
    TRIAL_TEST_EQ(data[2], 33);
    TRIAL_TEST_EQ(data[3], 44);
}

void clear()
{
    circular::vector<int> data(4);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.clear();
    TRIAL_TEST_EQ(data.size(), 0);
}

void push_front()
{
    circular::vector<int> data(4);
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void push_back()
{
    circular::vector<int> data(4);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void pop_front()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 1);
}

void pop_back()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    data.pop_back();
    TRIAL_TEST_EQ(data.size(), 1);
}

void move_front()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.move_front(), 11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void move_back()
{
    circular::vector<int> data = { 11, 22 };
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.move_back(), 22);
    TRIAL_TEST_EQ(data.size(), 1);
}

void run()
{
    ctor_default();
    ctor_default_alloc();
    ctor_copy();
    ctor_copy_alloc();
    ctor_move();
    ctor_move_alloc();
    ctor_copy_assign();
    ctor_move_assign();
    ctor_capacity();
    ctor_capacity_alloc();
    ctor_initializer_list();
    ctor_initializer_list_alloc();
    ctor_iterator();
    ctor_iterator_alloc();
    assign_initializer_list();
    assign_iterator();
    empty();
    capacity();
    size();
    max_size();
    full();
    front();
    front_const();
    back();
    back_const();
    operator_index();
    operator_index_const();
    clear();
    push_front();
    push_back();
    pop_front();
    pop_back();
    move_front();
    move_back();
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

void run()
{
    assign_below_capacity();
    assign_at_capacity();
    assign_above_capacity();
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
    allocator_suite::run();

    return boost::report_errors();
}
