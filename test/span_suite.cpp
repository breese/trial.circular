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
#include <vector>
#include <trial/detail/lightweight_test.hpp>
#include <trial/circular/span.hpp>

using namespace trial;

//-----------------------------------------------------------------------------

namespace api_suite
{

void api_ctor_default()
{
    circular::span<int> span;
    TRIAL_TEST(span.empty());
    TRIAL_TEST(span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 0);
}

void api_ctor_copy()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
    circular::span<int> clone(span);
    TRIAL_TEST(clone.empty());
    TRIAL_TEST(!clone.full());
    TRIAL_TEST_EQ(clone.size(), 0);
    TRIAL_TEST_EQ(clone.capacity(), 4);
}

void api_ctor_copy_assign()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
    circular::span<int> clone;
    TRIAL_TEST(clone.empty());
    TRIAL_TEST(clone.full());
    TRIAL_TEST_EQ(clone.size(), 0);
    TRIAL_TEST_EQ(clone.capacity(), 0);
    clone = span;
    TRIAL_TEST(clone.empty());
    TRIAL_TEST(!clone.full());
    TRIAL_TEST_EQ(clone.size(), 0);
    TRIAL_TEST_EQ(clone.capacity(), 4);
}

void api_ctor_move()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
    circular::span<int> clone(std::move(span));
    TRIAL_TEST(clone.empty());
    TRIAL_TEST(!clone.full());
    TRIAL_TEST_EQ(clone.size(), 0);
    TRIAL_TEST_EQ(clone.capacity(), 4);
}

void api_ctor_move_assign()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
    circular::span<int> clone;
    TRIAL_TEST(clone.empty());
    TRIAL_TEST(clone.full());
    TRIAL_TEST_EQ(clone.size(), 0);
    TRIAL_TEST_EQ(clone.capacity(), 0);
    clone = std::move(span);
    TRIAL_TEST(clone.empty());
    TRIAL_TEST(!clone.full());
    TRIAL_TEST_EQ(clone.size(), 0);
    TRIAL_TEST_EQ(clone.capacity(), 4);
}

void api_ctor_array()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void api_ctor_iterator()
{
    std::array<int, 4> array;
    circular::span<int> span(array.begin(), array.end());
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void api_ctor_iterator_init()
{
    std::array<int, 4> array = { 1, 2, 3, 4 };
    circular::span<int> span(array.begin(), array.end(), array.begin(), array.size());
    TRIAL_TEST(!span.empty());
    TRIAL_TEST(span.full());
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void api_empty()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST(span.empty());
    span.push_back(11);
    TRIAL_TEST(!span.empty());
}

void api_full()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST(!span.full());
    span = {11, 22, 33, 44};
    TRIAL_TEST(span.full());
}

void api_capacity()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void api_size()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST_EQ(span.size(), 0);
    span = {11, 22, 33, 44};
    TRIAL_TEST_EQ(span.size(), 4);
    span.push_back(55);
    TRIAL_TEST_EQ(span.size(), 4);
}

void api_front()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33};
    TRIAL_TEST_EQ(span.front(), 11);
    span.push_back(44);
    TRIAL_TEST_EQ(span.front(), 11);
    span.push_back(55);
    TRIAL_TEST_EQ(span.front(), 22);
}

void api_front_const()
{
    std::array<int, 4> array = {11, 22, 33, 44};
    const circular::span<int> span(array.begin(), array.end(), array.begin(), array.size());
    TRIAL_TEST_EQ(span.front(), 11);
}

void api_back()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33};
    TRIAL_TEST_EQ(span.back(), 33);
    span.push_front(44);
    TRIAL_TEST_EQ(span.back(), 33);
    span.push_front(55);
    TRIAL_TEST_EQ(span.back(), 22);
}

void api_back_const()
{
    std::array<int, 4> array = {11, 22, 33, 44};
    const circular::span<int> span(array.begin(), array.end(), array.begin(), array.size());
    TRIAL_TEST_EQ(span.back(), 44);
}

void api_operator_index()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33, 44};
    TRIAL_TEST_EQ(span[0], 11);
    TRIAL_TEST_EQ(span[1], 22);
    TRIAL_TEST_EQ(span[2], 33);
    TRIAL_TEST_EQ(span[3], 44);
    span.push_back(55);
    TRIAL_TEST_EQ(span[0], 22);
    TRIAL_TEST_EQ(span[1], 33);
    TRIAL_TEST_EQ(span[2], 44);
    TRIAL_TEST_EQ(span[3], 55);
}

void api_operator_index_const()
{
    std::array<int, 4> array = {11, 22, 33, 44};
    const circular::span<int> span(array.begin(), array.end(), array.begin(), array.size());
    TRIAL_TEST_EQ(span[0], 11);
    TRIAL_TEST_EQ(span[1], 22);
    TRIAL_TEST_EQ(span[2], 33);
    TRIAL_TEST_EQ(span[3], 44);
}

void api_clear()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33, 44};
    TRIAL_TEST_EQ(span.size(), 4);
    span.clear();
    TRIAL_TEST_EQ(span.size(), 0);
}

void api_assign_iterator()
{
    int array[4];
    circular::span<int> span(array);
    std::vector<int> input = {11, 22, 33, 44};
    span.assign(input.begin(), input.end());
    {
        std::vector<int> expect = {11, 22, 33, 44};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_assign_initializer_list()
{
    int array[4];
    circular::span<int> span(array);
    span.assign({11, 22, 33, 44});
    {
        std::vector<int> expect = {11, 22, 33, 44};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_push_front()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33};
    span.push_front(44);
    {
        std::vector<int> expect = {44, 11, 22, 33};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_push_back()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33};
    span.push_back(44);
    {
        std::vector<int> expect = {11, 22, 33, 44};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_pop_front()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33};
    span.pop_front();
    {
        std::vector<int> expect = {22, 33};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_pop_back()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33};
    span.pop_back();
    {
        std::vector<int> expect = {11, 22};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_move_front()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33};
    TRIAL_TEST_EQ(span.move_front(), 11);
    {
        std::vector<int> expect = {22, 33};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_move_back()
{
    int array[4];
    circular::span<int> span(array);
    span = {11, 22, 33};
    TRIAL_TEST_EQ(span.move_back(), 33);
    {
        std::vector<int> expect = {11, 22};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_advance_left()
{
    int array[4];
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    span.advance_left(1);
    {
        std::vector<int> expect = { 22, 33, 44, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_advance_right()
{
    int array[4];
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    span.advance_right(1);
    {
        std::vector<int> expect = { 44, 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    api_ctor_default();
    api_ctor_copy();
    api_ctor_copy_assign();
    api_ctor_move();
    api_ctor_move_assign();
    api_ctor_array();
    api_ctor_iterator();
    api_ctor_iterator_init();
    api_empty();
    api_full();
    api_capacity();
    api_size();
    api_front();
    api_front_const();
    api_back();
    api_back_const();
    api_operator_index();
    api_operator_index_const();
    api_clear();
    api_assign_iterator();
    api_assign_initializer_list();
    api_push_front();
    api_push_back();
    api_pop_front();
    api_pop_back();
    api_move_front();
    api_move_back();
    api_advance_left();
    api_advance_right();
}

} // namespace api_suite

//-----------------------------------------------------------------------------

namespace plain_array_suite
{

void test_ctor()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void test_push_front()
{
    int array[4];
    circular::span<int> span(array);
    span.push_front(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_front(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_front(3);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 3);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_front(4);
    TRIAL_TEST(span.full());
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 4);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_front(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 5);
    TRIAL_TEST_EQ(span.back(), 2);
    span.push_front(6);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 6);
    TRIAL_TEST_EQ(span.back(), 3);
    span.push_front(7);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 7);
    TRIAL_TEST_EQ(span.back(), 4);
    span.push_front(8);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 8);
    TRIAL_TEST_EQ(span.back(), 5);
    span.push_front(9);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 9);
    TRIAL_TEST_EQ(span.back(), 6);
}

void test_push_back()
{
    int array[4];
    circular::span<int> span(array);
    span.push_back(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_back(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 2);
    span.push_back(3);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 3);
    span.push_back(4);
    TRIAL_TEST(span.full());
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 4);
    span.push_back(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 5);
    span.push_back(6);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 3);
    TRIAL_TEST_EQ(span.back(), 6);
    span.push_back(7);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 4);
    TRIAL_TEST_EQ(span.back(), 7);
    span.push_back(8);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 5);
    TRIAL_TEST_EQ(span.back(), 8);
    span.push_back(9);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 6);
    TRIAL_TEST_EQ(span.back(), 9);
}

void test_push_alternating()
{
    int array[4];
    circular::span<int> span(array);
    span.push_back(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_front(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_back(3);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 3);
    span.push_front(4);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 4);
    TRIAL_TEST_EQ(span.back(), 3);
    span.push_back(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 2); // 4 was pushed out
    TRIAL_TEST_EQ(span.back(), 5);
    span.push_front(6);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 6);
    TRIAL_TEST_EQ(span.back(), 3); // 5 was pushed out
}

void test_pop_front()
{
    int array[4];
    circular::span<int> span(array);
    span.push_back(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.pop_front();
    TRIAL_TEST_EQ(span.size(), 0);
    span.push_back(1);
    span.push_back(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 2);
    span.pop_front();
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 2);
    span.push_back(3);
    span.push_back(4);
    span.push_back(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 5);
    span.pop_front();
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 3);
    TRIAL_TEST_EQ(span.back(), 5);
}

void test_pop_back()
{
    int array[4];
    circular::span<int> span(array);
    span.push_back(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.pop_back();
    TRIAL_TEST_EQ(span.size(), 0);
    span.push_back(1);
    span.push_back(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 2);
    span.pop_back();
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_back(3);
    span.push_back(4);
    span.push_back(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 5);
    span.pop_back();
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 4);
}

void test_move_front()
{
    int array[4];
    circular::span<int> span(array);
    span.push_back(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    TRIAL_TEST_EQ(span.move_front(), 1);
    TRIAL_TEST_EQ(span.size(), 0);
    span.push_back(1);
    span.push_back(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 2);
    TRIAL_TEST_EQ(span.move_front(), 1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 2);
    span.push_back(3);
    span.push_back(4);
    span.push_back(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 5);
    TRIAL_TEST_EQ(span.move_front(), 2);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 3);
    TRIAL_TEST_EQ(span.back(), 5);
}

void test_move_back()
{
    int array[4];
    circular::span<int> span(array);
    span.push_back(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    TRIAL_TEST_EQ(span.move_back(), 1);
    TRIAL_TEST_EQ(span.size(), 0);
    span.push_back(1);
    span.push_back(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 2);
    TRIAL_TEST_EQ(span.move_back(), 2);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_back(3);
    span.push_back(4);
    span.push_back(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 5);
    TRIAL_TEST_EQ(span.move_back(), 5);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 4);
}

void test_assign_operator()
{
    int array[4];
    circular::span<int> span(array);
    span = { 11 };
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 11);
    TRIAL_TEST_EQ(span.back(), 11);
    span = { 21, 22 };
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 21);
    TRIAL_TEST_EQ(span.back(), 22);
    span = { 31, 32, 33 };
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 31);
    TRIAL_TEST_EQ(span.back(), 33);
    span = { 41, 42, 43, 44 };
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 41);
    TRIAL_TEST_EQ(span.back(), 44);
    span = { 51, 52, 53, 54, 55 };
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 52);
    TRIAL_TEST_EQ(span.back(), 55);
}

void test_assign_iterator()
{
    int array[4];
    circular::span<int> span(array);
    {
        std::vector<int> input = { 11 };
        span.assign(input.begin(), input.end());
    }
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 11);
    TRIAL_TEST_EQ(span.back(), 11);
    {
        std::vector<int> input = { 21, 22 };
        span.assign(input.begin(), input.end());
    }
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 21);
    TRIAL_TEST_EQ(span.back(), 22);
    {
        std::vector<int> input = { 31, 32, 33 };
        span.assign(input.begin(), input.end());
    }
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 31);
    TRIAL_TEST_EQ(span.back(), 33);
    {
        std::vector<int> input = { 41, 42, 43, 44 };
        span.assign(input.begin(), input.end());
    }
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 41);
    TRIAL_TEST_EQ(span.back(), 44);
    {
        std::vector<int> input = { 51, 52, 53, 54, 55 };
        span.assign(input.begin(), input.end());
    }
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 52);
    TRIAL_TEST_EQ(span.back(), 55);
}

void test_assign_initializer_list()
{
    int array[4];
    circular::span<int> span(array);
    span.assign({ 11 });
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 11);
    TRIAL_TEST_EQ(span.back(), 11);
    span.assign({ 21, 22 });
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 21);
    TRIAL_TEST_EQ(span.back(), 22);
    span.assign({ 31, 32, 33 });
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 31);
    TRIAL_TEST_EQ(span.back(), 33);
    span.assign({ 41, 42, 43, 44 });
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 41);
    TRIAL_TEST_EQ(span.back(), 44);
    span.assign({ 51, 52, 53, 54, 55 });
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 52);
    TRIAL_TEST_EQ(span.back(), 55);
}

void run()
{
    test_ctor();
    test_push_front();
    test_push_back();
    test_push_alternating();
    test_pop_front();
    test_pop_back();
    test_move_front();
    test_move_back();
    test_assign_operator();
    test_assign_iterator();
    test_assign_initializer_list();
}

} // namespace plain_array_suite

//-----------------------------------------------------------------------------

namespace const_array_suite
{

void test_ctor()
{
    int array[4];
    circular::span<const int> span(array);
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void test_pop_front()
{
    int array[4] = { 1, 2, 3 };
    circular::span<const int> span(&array[0], &array[3],
                                   &array[0], 3);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 3);
    span.pop_front();
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 3);
}

void test_move_front()
{
    int array[4] = { 1, 2, 3 };
    circular::span<const int> span(&array[0], &array[3],
                                   &array[0], 3);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 3);
    TRIAL_TEST_EQ(span.move_front(), 1);
}

void run()
{
    // Cannot push or assign via const span
    test_ctor();
    test_pop_front();
    test_move_front();
}

} // namespace const_array_suite

//-----------------------------------------------------------------------------

namespace std_array_suite
{

void test_ctor()
{
    std::array<int, 4> array;
    circular::span<int> span(array.begin(), array.end());
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void test_ctor_init()
{
    std::array<int, 4> array = { 1, 2, 3, 4 };
    circular::span<int> span(array.begin(), array.end(), array.begin(), array.size());
    TRIAL_TEST(!span.empty());
    TRIAL_TEST(span.full());
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void test_push_front()
{
    std::array<int, 4> array;
    circular::span<int> span(array.begin(), array.end());
    span.push_front(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_front(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_front(3);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 3);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_front(4);
    TRIAL_TEST(span.full());
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 4);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_front(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 5);
    TRIAL_TEST_EQ(span.back(), 2);
    span.push_front(6);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 6);
    TRIAL_TEST_EQ(span.back(), 3);
}

void test_push_back()
{
    std::array<int, 4> array;
    circular::span<int> span(array.begin(), array.end());
    span.push_back(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_back(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 2);
    span.push_back(3);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 3);
    span.push_back(4);
    TRIAL_TEST(span.full());
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 4);
    span.push_back(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 5);
    span.push_back(6);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 3);
    TRIAL_TEST_EQ(span.back(), 6);
}

void run()
{
    test_ctor();
    test_ctor_init();
    test_push_front();
    test_push_back();
}

} // namespace std_array_suite

//-----------------------------------------------------------------------------

namespace std_vector_suite
{

void test_ctor_empty()
{
    std::vector<int> array;
    circular::span<int> span(array.begin(), array.end());
    TRIAL_TEST(span.empty());
    TRIAL_TEST(span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 0);
}

void test_ctor()
{
    std::vector<int> array(4);
    circular::span<int> span(array.begin(), array.end());
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void test_push_back()
{
    std::vector<int> array(4);
    circular::span<int> span(array.begin(), array.end());
    span.push_back(1);
    TRIAL_TEST_EQ(span.size(), 1);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 1);
    span.push_back(2);
    TRIAL_TEST_EQ(span.size(), 2);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 2);
    span.push_back(3);
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 3);
    span.push_back(4);
    TRIAL_TEST(span.full());
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 1);
    TRIAL_TEST_EQ(span.back(), 4);
    span.push_back(5);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 2);
    TRIAL_TEST_EQ(span.back(), 5);
    span.push_back(6);
    TRIAL_TEST_EQ(span.size(), 4);
    TRIAL_TEST_EQ(span.front(), 3);
    TRIAL_TEST_EQ(span.back(), 6);
}

void run()
{
    test_ctor_empty();
    test_ctor();
    test_push_back();
}

} // namespace std_vector_suite

//-----------------------------------------------------------------------------

namespace initialization_suite
{

void init_zero()
{
    std::array<int, 4> array = {11, 22, 33, 44};
    circular::span<int> span(array.begin(), array.end(), array.begin(), 0);
    {
        std::vector<int> expect = {};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void init_one()
{
    std::array<int, 4> array = {11, 22, 33, 44};
    circular::span<int> span(array.begin(), array.end(), array.begin(), 1);
    {
        std::vector<int> expect = {11};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void init_two()
{
    std::array<int, 4> array = {11, 22, 33, 44};
    circular::span<int> span(array.begin(), array.end(), array.begin(), 2);
    {
        std::vector<int> expect = {11, 22};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void init_three()
{
    std::array<int, 4> array = {11, 22, 33, 44};
    circular::span<int> span(array.begin(), array.end(), array.begin(), 3);
    {
        std::vector<int> expect = {11, 22, 33};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void init_four()
{
    std::array<int, 4> array = {11, 22, 33, 44};
    circular::span<int> span(array.begin(), array.end(), array.begin(), array.size());
    {
        std::vector<int> expect = {11, 22, 33, 44};
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    init_zero();
    init_one();
    init_two();
    init_three();
    init_four();
}

} // namespace initialization_suite

//-----------------------------------------------------------------------------

namespace clear_suite
{

void clear_empty()
{
    int array[4];
    circular::span<int> span(array);
    TRIAL_TEST_EQ(span.capacity(), 4);
    TRIAL_TEST_EQ(span.size(), 0);
    span.clear();
    TRIAL_TEST_EQ(span.capacity(), 4);
    TRIAL_TEST_EQ(span.size(), 0);
}

void clear_partial()
{
    int array[4];
    circular::span<int> span(array);
    span.push_back(1);
    span.push_back(2);
    span.push_back(3);
    TRIAL_TEST_EQ(span.capacity(), 4);
    TRIAL_TEST_EQ(span.size(), 3);
    span.clear();
    TRIAL_TEST_EQ(span.capacity(), 4);
    TRIAL_TEST_EQ(span.size(), 0);
}

void clear_full()
{
    int array[4];
    circular::span<int> span(array);
    span.push_back(1);
    span.push_back(2);
    span.push_back(3);
    span.push_back(4);
    TRIAL_TEST_EQ(span.capacity(), 4);
    TRIAL_TEST_EQ(span.size(), 4);
    span.clear();
    TRIAL_TEST_EQ(span.capacity(), 4);
    TRIAL_TEST_EQ(span.size(), 0);
}

void clear_overfull()
{
    int array[4];
    circular::span<int> span(array);
    span.push_back(1);
    span.push_back(2);
    span.push_back(3);
    span.push_back(4);
    span.push_back(5);
    TRIAL_TEST_EQ(span.capacity(), 4);
    TRIAL_TEST_EQ(span.size(), 4);
    span.clear();
    TRIAL_TEST_EQ(span.capacity(), 4);
    TRIAL_TEST_EQ(span.size(), 0);
}

void run()
{
    clear_empty();
    clear_partial();
    clear_full();
    clear_overfull();
}

} // namespace clear_suite

//-----------------------------------------------------------------------------

namespace window_size_suite
{

void window_1()
{
    std::array<int, 1> array;
    circular::span<int> span(array.begin(), array.end());
    span.push_back(1);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 1);
        std::vector<int> expect = { 1 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(2);
    {
        TRIAL_TEST_EQ(span.front(), 2);
        TRIAL_TEST_EQ(span.back(), 2);
        std::vector<int> expect = { 2 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(3);
    {
        TRIAL_TEST_EQ(span.front(), 3);
        TRIAL_TEST_EQ(span.back(), 3);
        std::vector<int> expect = { 3 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(4);
    {
        TRIAL_TEST_EQ(span.front(), 4);
        TRIAL_TEST_EQ(span.back(), 4);
        std::vector<int> expect = { 4 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(5);
    {
        TRIAL_TEST_EQ(span.front(), 5);
        TRIAL_TEST_EQ(span.back(), 5);
        std::vector<int> expect = { 5 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(6);
    {
        TRIAL_TEST_EQ(span.front(), 6);
        TRIAL_TEST_EQ(span.back(), 6);
        std::vector<int> expect = { 6 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void window_2()
{
    std::array<int, 2> array;
    circular::span<int> span(array.begin(), array.end());
    span.push_back(1);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 1);
        std::vector<int> expect = { 1 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(2);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 2);
        std::vector<int> expect = { 1, 2 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(3);
    {
        TRIAL_TEST_EQ(span.front(), 2);
        TRIAL_TEST_EQ(span.back(), 3);
        std::vector<int> expect = { 2, 3 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(4);
    {
        TRIAL_TEST_EQ(span.front(), 3);
        TRIAL_TEST_EQ(span.back(), 4);
        std::vector<int> expect = { 3, 4 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(5);
    {
        TRIAL_TEST_EQ(span.front(), 4);
        TRIAL_TEST_EQ(span.back(), 5);
        std::vector<int> expect = { 4, 5 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(6);
    {
        TRIAL_TEST_EQ(span.front(), 5);
        TRIAL_TEST_EQ(span.back(), 6);
        std::vector<int> expect = { 5, 6 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void window_3()
{
    std::array<int, 3> array;
    circular::span<int> span(array.begin(), array.end());
    span.push_back(1);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 1);
        std::vector<int> expect = { 1 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(2);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 2);
        std::vector<int> expect = { 1, 2 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(3);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 3);
        std::vector<int> expect = { 1, 2, 3 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(4);
    {
        TRIAL_TEST_EQ(span.front(), 2);
        TRIAL_TEST_EQ(span.back(), 4);
        std::vector<int> expect = { 2, 3, 4 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(5);
    {
        TRIAL_TEST_EQ(span.front(), 3);
        TRIAL_TEST_EQ(span.back(), 5);
        std::vector<int> expect = { 3, 4, 5 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(6);
    {
        TRIAL_TEST_EQ(span.front(), 4);
        TRIAL_TEST_EQ(span.back(), 6);
        std::vector<int> expect = { 4, 5, 6 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(7);
    {
        TRIAL_TEST_EQ(span.front(), 5);
        TRIAL_TEST_EQ(span.back(), 7);
        std::vector<int> expect = { 5, 6, 7 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void window_4()
{
    std::array<int, 4> array;
    circular::span<int> span(array.begin(), array.end());
    span.push_back(1);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 1);
        std::vector<int> expect = { 1 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(2);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 2);
        std::vector<int> expect = { 1, 2 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(3);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 3);
        std::vector<int> expect = { 1, 2, 3 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(4);
    {
        TRIAL_TEST_EQ(span.front(), 1);
        TRIAL_TEST_EQ(span.back(), 4);
        std::vector<int> expect = { 1, 2, 3, 4 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(5);
    {
        TRIAL_TEST_EQ(span.front(), 2);
        TRIAL_TEST_EQ(span.back(), 5);
        std::vector<int> expect = { 2, 3, 4, 5 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(6);
    {
        TRIAL_TEST_EQ(span.front(), 3);
        TRIAL_TEST_EQ(span.back(), 6);
        std::vector<int> expect = { 3, 4, 5, 6 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(7);
    {
        TRIAL_TEST_EQ(span.front(), 4);
        TRIAL_TEST_EQ(span.back(), 7);
        std::vector<int> expect = { 4, 5, 6, 7 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(8);
    {
        TRIAL_TEST_EQ(span.front(), 5);
        TRIAL_TEST_EQ(span.back(), 8);
        std::vector<int> expect = { 5, 6, 7, 8 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(9);
    {
        TRIAL_TEST_EQ(span.front(), 6);
        TRIAL_TEST_EQ(span.back(), 9);
        std::vector<int> expect = { 6, 7, 8, 9 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    window_1();
    window_2();
    window_3();
    window_4();
}

} // namespace window_size_suite

//-----------------------------------------------------------------------------

namespace advance_suite
{

void advance_empty()
{
    int array[4] = {};
    circular::span<int> span(array);
    TRIAL_TEST(span.empty());
    span.advance_left(1);
    TRIAL_TEST(span.empty());
    span.advance_right(1);
    TRIAL_TEST(span.empty());
}

void advance_0()
{
    // Advance 0 is a no-operation
    int array[4] = {};
    circular::span<int> span(array);
    span.push_back(11);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(0);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(0);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span = { 11, 22, 33, 44 };
    span.advance_left(0);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(0);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void advance_back_left_1()
{
    int array[4] = {};
    circular::span<int> span(array);
    span.push_back(11);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(22);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(33);
    {
        std::vector<int> expect = { 22, 11, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 11, 33, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(44);
    {
        std::vector<int> expect = { 11, 33, 22, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 33, 22, 44, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(55);
    {
        std::vector<int> expect = { 22, 44, 11, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 44, 11, 55, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(66);
    {
        std::vector<int> expect = { 11, 55, 22, 66 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 55, 22, 66, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void advance_back_right_1()
{
    int array[4] = {};
    circular::span<int> span(array);
    span.push_back(11);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(22);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(33);
    {
        std::vector<int> expect = { 22, 11, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(44);
    {
        std::vector<int> expect = { 33, 22, 11, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 44, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(55);
    {
        std::vector<int> expect = { 33, 22, 11, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 55, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(66);
    {
        std::vector<int> expect = { 33, 22, 11, 66 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 66, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void advance_front_left_1()
{
    int array[4] = {};
    circular::span<int> span(array);
    span.push_front(11);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(22);
    {
        std::vector<int> expect = { 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(33);
    {
        std::vector<int> expect = { 33, 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(44);
    {
        std::vector<int> expect = { 44, 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(55);
    {
        std::vector<int> expect = { 55, 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 11, 22, 33, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(66);
    {
        std::vector<int> expect = { 66, 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 11, 22, 33, 66 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void advance_front_right_1()
{
    int array[4] = {};
    circular::span<int> span(array);
    span.push_front(11);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(22);
    {
        std::vector<int> expect = { 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(33);
    {
        std::vector<int> expect = { 33, 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 22, 33, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(44);
    {
        std::vector<int> expect = { 44, 22, 33, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 11, 44, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(55);
    {
        std::vector<int> expect = { 55, 11, 44, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 22, 55, 11, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(66);
    {
        std::vector<int> expect = { 66, 22, 55, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 11, 66, 22, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void advance_left_n()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    span.advance_left(1);
    {
        std::vector<int> expect = { 22, 33, 44, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 33, 44, 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 44, 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 22, 33, 44, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void advance_right_n()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    span.advance_right(1);
    {
        std::vector<int> expect = { 44, 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 33, 44, 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 22, 33, 44, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 44, 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void advance_alternating_n()
{
    int array[8] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55, 66, 77, 88 };
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(0);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(1);
    {
        std::vector<int> expect = { 22, 33, 44, 55, 66, 77, 88, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(1);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(2);
    {
        std::vector<int> expect = { 33, 44, 55, 66, 77, 88, 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(2);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(3);
    {
        std::vector<int> expect = { 44, 55, 66, 77, 88, 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(3);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(4);
    {
        std::vector<int> expect = { 55, 66, 77, 88, 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(4);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(5);
    {
        std::vector<int> expect = { 66, 77, 88, 11, 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(5);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(6);
    {
        std::vector<int> expect = { 77, 88, 11, 22, 33, 44, 55, 66 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(6);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(7);
    {
        std::vector<int> expect = { 88, 11, 22, 33, 44, 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(7);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_left(8);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.advance_right(8);
    {
        std::vector<int> expect = { 11, 22, 33, 44, 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    advance_empty();
    advance_0();
    advance_back_left_1();
    advance_back_right_1();
    advance_front_left_1();
    advance_front_right_1();
    advance_left_n();
    advance_right_n();
    advance_alternating_n();
}

} // namespace advance_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    api_suite::run();
    plain_array_suite::run();
    const_array_suite::run();
    std_array_suite::run();
    std_vector_suite::run();
    initialization_suite::run();
    clear_suite::run();
    window_size_suite::run();
    advance_suite::run();

    return boost::report_errors();
}
