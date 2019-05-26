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
#include <trial/detail/lightweight_test.hpp>
#include <trial/circular/span.hpp>

using namespace trial;

// Algorithms that modify the input container via iterators are not supported.

//-----------------------------------------------------------------------------

namespace copy_suite
{

void copy_back_inserter()
{
    int array[4];
    circular::span<int> span(array);
    span = { 11, 22, 33 };
    std::vector<int> input = { 111, 222, 333, 444, 555 };
    std::copy(input.begin(), input.end(), std::back_inserter(span));
    {
        std::vector<int> expect = { 222, 333, 444, 555 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    copy_back_inserter();
}

} // namespace copy_suite

//-----------------------------------------------------------------------------

namespace count_suite
{

void count_full()
{
    int array[4];
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST_EQ(std::count(span.begin(), span.end(), 22), 1);
}

void count_if_full()
{
    int array[4];
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST_EQ(std::count_if(span.begin(), span.end(), [] (int value) { return value % 2 == 0; }), 2);
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
    int array[4];
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

void find_lower_bound_3()
{
    int array[4];
    circular::span<int> span(array);
    span = { 1, 2, 3, 4 };
    auto lower = std::lower_bound(span.begin(), span.end(), 3);
    {
        std::vector<int> expect = { 3, 4 };
        TRIAL_TEST_ALL_EQ(lower, span.end(),
                          expect.begin(), expect.end());
    }
}

void find_minmax()
{
    int array[4];
    circular::span<int> span(array);
    span = { 1, 2, 3, 4 };
    auto where = std::minmax_element(span.begin(), span.end());
    TRIAL_TEST_EQ(*where.first, 1);
    TRIAL_TEST_EQ(*where.second, 4);
}

void run()
{
    find_lower_bound_3();
    find_minmax();
}

} // namespace find_suite

//-----------------------------------------------------------------------------

namespace predicate_suite
{

void test_all_of()
{
    int array[4];
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
    int array[4];
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST(std::any_of(span.begin(), span.end(), [] (int current) { return current == 11; }));
    span.push_back(55);
    TRIAL_TEST(!std::any_of(span.begin(), span.end(), [] (int current) { return current == 11; }));
}

void test_none_of()
{
    int array[4];
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

namespace rotate_suite
{

void rotate_copy()
{
    int array[4];
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55 };
    int resarray[4];
    circular::span<int> result(resarray);
    std::rotate_copy(span.begin(), ++span.begin(), span.end(), std::back_inserter(result));
    {
        std::vector<int> expect = { 33, 44, 55, 22 };
        TRIAL_TEST_ALL_EQ(result.begin(), result.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    rotate_copy();
}

} // namespace rotate_suite

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
    rotate_suite::run();

    return boost::report_errors();
}
