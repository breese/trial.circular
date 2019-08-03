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
#include <algorithm>
#include <utility>
#include <trial/detail/lightweight_test.hpp>
#include <trial/circular/span.hpp>

using namespace trial;

// Algorithms that modify the input container via iterators are not supported.

//-----------------------------------------------------------------------------

namespace copy_suite
{

void copy()
{
    int array[4] = {};
    circular::span<int> span(array);
    std::vector<int> input = { 111, 222, 333, 444, 555 };
    std::copy(input.begin(), input.end(), std::back_inserter(span));
    {
        std::vector<int> expect = { 222, 333, 444, 555 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void copy_if()
{
    int array[4] = {};
    circular::span<int> span(array);
    std::vector<int> input = { 111, 222, 333, 444, 555 };
    std::copy_if(input.begin(), input.end(), std::back_inserter(span), [](int value) { return value > 222; });
    {
        std::vector<int> expect = { 333, 444, 555 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void copy_n()
{
    int array[4] = {};
    circular::span<int> span(array);
    std::vector<int> input = { 111, 222, 333, 444, 555 };
    std::copy_n(input.begin(), input.size(), std::back_inserter(span));
    {
        std::vector<int> expect = { 222, 333, 444, 555 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void partition_copy()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55 };
    int yes_copy_array[4];
    circular::span<int> yes_copy(yes_copy_array);
    int no_copy_array[4];
    circular::span<int> no_copy(no_copy_array);
    std::partition_copy(span.begin(), span.end(), std::back_inserter(yes_copy), std::back_inserter(no_copy), [](int value) { return value % 2 == 0; });
    {
        std::vector<int> expect = { 22, 44 };
        TRIAL_TEST_ALL_EQ(yes_copy.begin(), yes_copy.end(),
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 33, 55 };
        TRIAL_TEST_ALL_EQ(no_copy.begin(), no_copy.end(),
                          expect.begin(), expect.end());
    }
}

void remove_copy()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55 };
    int copy_array[4];
    circular::span<int> copy(copy_array);
    std::remove_copy(span.begin(), span.end(), std::back_inserter(copy), 33);
    {
        std::vector<int> expect = { 22, 44, 55 };
        TRIAL_TEST_ALL_EQ(copy.begin(), copy.end(),
                          expect.begin(), expect.end());
    }
}

void reverse_copy()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55 };
    int copy_array[4];
    circular::span<int> copy(copy_array);
    std::reverse_copy(span.begin(), span.end(), std::back_inserter(copy));
    {
        std::vector<int> expect = { 55, 44, 33, 22 };
        TRIAL_TEST_ALL_EQ(copy.begin(), copy.end(),
                          expect.begin(), expect.end());
    }
}

void rotate_copy()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55 };
    int copy_array[4];
    circular::span<int> copy(copy_array);
    std::rotate_copy(span.begin(), ++span.begin(), span.end(), std::back_inserter(copy));
    {
        std::vector<int> expect = { 33, 44, 55, 22 };
        TRIAL_TEST_ALL_EQ(copy.begin(), copy.end(),
                          expect.begin(), expect.end());
    }
}

void unique_copy()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55, 55 };
    int copy_array[4];
    circular::span<int> copy(copy_array);
    std::unique_copy(span.begin(), span.end(), std::back_inserter(copy));
    {
        std::vector<int> expect = { 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(copy.begin(), copy.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    // Only works with back_inserter
    copy();
    copy_if();
    copy_n();
    partition_copy();
    remove_copy();
    reverse_copy();
    rotate_copy();
    unique_copy();
}

} // namespace copy_suite

//-----------------------------------------------------------------------------

namespace count_suite
{

void count_full()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55 };
    TRIAL_TEST_EQ(std::count(span.begin(), span.end(), 11), 0);
    TRIAL_TEST_EQ(std::count(span.begin(), span.end(), 22), 1);
}

void count_if_full()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55 };
    TRIAL_TEST_EQ(std::count_if(span.begin(), span.end(), [](int value) { return value % 2 == 0; }), 2);
}

void run()
{
    count_full();
    count_if_full();
}

} // namespace count_suite

//-----------------------------------------------------------------------------

namespace fill_suite
{

void fill_n_full()
{
    int array[4] = {};
    circular::span<int> span(array);
    std::fill_n(std::back_inserter(span), 2 * span.capacity(), 55);
    TRIAL_TEST_EQ(span.size(), span.capacity());
    {
        std::vector<int> expect = { 55, 55, 55, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    fill_n_full();
}

} // namespace fill_suite

//-----------------------------------------------------------------------------

namespace find_suite
{

void find()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    auto where = std::find(span.begin(), span.end(), 33);
    {
        std::vector<int> expect = { 33, 44 };
        TRIAL_TEST_ALL_EQ(where, span.end(),
                          expect.begin(), expect.end());
    }
}

void find_if()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    auto where = std::find_if(span.begin(), span.end(), [](int value){ return value > 22; });
    {
        std::vector<int> expect = { 33, 44 };
        TRIAL_TEST_ALL_EQ(where, span.end(),
                          expect.begin(), expect.end());
    }
}

void adjacent_find()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 33, 44 };
    auto where = std::adjacent_find(span.begin(), span.end());
    {
        std::vector<int> expect = { 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), where,
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 33, 33, 44 };
        TRIAL_TEST_ALL_EQ(where, span.end(),
                          expect.begin(), expect.end());
    }
}

void adjacent_find_if()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 44, 33, 33, 22, 11 };
    auto where = std::adjacent_find(span.begin(), span.end(), std::greater<int>());
    {
        std::vector<int> expect = { 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), where,
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(where, span.end(),
                          expect.begin(), expect.end());
    }
}

void lower_bound()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    auto lower = std::lower_bound(span.begin(), span.end(), 33);
    {
        std::vector<int> expect = { 33, 44 };
        TRIAL_TEST_ALL_EQ(lower, span.end(),
                          expect.begin(), expect.end());
    }
}

void upper_bound()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    auto upper = std::upper_bound(span.begin(), span.end(), 33);
    {
        std::vector<int> expect = { 44 };
        TRIAL_TEST_ALL_EQ(upper, span.end(),
                          expect.begin(), expect.end());
    }
}

void minmax_element()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    auto where = std::minmax_element(span.begin(), span.end());
    TRIAL_TEST_EQ(*where.first, 11);
    TRIAL_TEST_EQ(*where.second, 44);
}

void run()
{
    find();
    find_if();
    adjacent_find();
    adjacent_find_if();
    lower_bound();
    upper_bound();
    minmax_element();
}

} // namespace find_suite

//-----------------------------------------------------------------------------

namespace predicate_suite
{

void test_all_of()
{
    int array[4] = {};
    circular::span<int> span(array);
    span.push_back(11);
    TRIAL_TEST(std::all_of(span.begin(), span.end(), [] (int current) { return current == 11; }));
    span.push_back(11);
    TRIAL_TEST(std::all_of(span.begin(), span.end(), [] (int current) { return current == 11; }));
    span.push_back(55);
    TRIAL_TEST(!std::all_of(span.begin(), span.end(), [] (int current) { return current == 11; }));
    span.push_back(55);
    span.push_back(55);
    span.push_back(55);
    TRIAL_TEST(std::all_of(span.begin(), span.end(), [] (int current) { return current == 55; }));
}

void test_any_of()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST(std::any_of(span.begin(), span.end(), [] (int current) { return current == 11; }));
    span.push_back(55);
    TRIAL_TEST(!std::any_of(span.begin(), span.end(), [] (int current) { return current == 11; }));
}

void test_none_of()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST(!std::none_of(span.begin(), span.end(), [] (int current) { return current == 11; }));
    span.push_back(55);
    TRIAL_TEST(std::none_of(span.begin(), span.end(), [] (int current) { return current == 11; }));
}

void run()
{
    test_all_of();
    test_any_of();
    test_none_of();
}

} // namespace predicate_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    copy_suite::run();
    count_suite::run();
    fill_suite::run();
    find_suite::run();
    predicate_suite::run();

    return boost::report_errors();
}
