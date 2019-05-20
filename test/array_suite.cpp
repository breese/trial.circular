///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <numeric>
#include <vector>
#include <trial/detail/lightweight_test.hpp>
#include <trial/circular/array.hpp>

using namespace trial;

//-----------------------------------------------------------------------------

namespace api_suite
{

void test_ctor_default()
{
    circular::array<int, 4> data;
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void test_ctor_move()
{
    circular::array<int, 4> data;
    data.push_back(11);
    circular::array<int, 4> copy(std::move(data));
    TRIAL_TEST_EQ(copy.size(), 1);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void test_ctor_move_assign()
{
    circular::array<int, 4> data;
    data.push_back(11);
    circular::array<int, 4> copy;
    copy = std::move(data);
    TRIAL_TEST_EQ(copy.size(), 1);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void test_ctor_initializer_list()
{
    circular::array<int, 4> data({11, 22});
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void test_ctor_iterator()
{
    std::vector<int> input = {11, 22};
    circular::array<int, 4> data(input.begin(), input.end());
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void test_operator_assign_initializer_list()
{
    circular::array<int, 4> data;
    data = {11, 22};
    TRIAL_TEST_EQ(data.size(), 2);
}

void test_assign_initializer_list()
{
    circular::array<int, 4> data;
    data.assign({11, 22});
    TRIAL_TEST_EQ(data.size(), 2);
}

void test_assign_iterator()
{
    circular::array<int, 4> data;
    std::vector<int> input = {11, 22};
    data.assign(input.begin(), input.end());
    TRIAL_TEST_EQ(data.size(), 2);
}

void test_empty()
{
    circular::array<int, 4> data;
    TRIAL_TEST(data.empty());
}

void test_capacity()
{
    circular::array<int, 4> data;
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void test_size()
{
    circular::array<int, 4> data;
    TRIAL_TEST_EQ(data.size(), 0);
}

void test_max_size()
{
    circular::array<int, 4> data;
    TRIAL_TEST_EQ(data.max_size(), 4);
}

void test_full()
{
    circular::array<int, 4> data;
    TRIAL_TEST(!data.full());
}

void test_front()
{
    circular::array<int, 4> data;
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.front(), 11);
}

void test_back()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.back(), 11);
}

void test_operator_index()
{
    circular::array<int, 4> data = {11, 22, 33};
    TRIAL_TEST_EQ(data[0], 11);
    TRIAL_TEST_EQ(data[1], 22);
    TRIAL_TEST_EQ(data[2], 33);
}

void test_clear()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.clear();
    TRIAL_TEST_EQ(data.size(), 0);
}

void test_push_front()
{
    circular::array<int, 4> data;
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void test_push_back()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void test_pop_front()
{
    circular::array<int, 4> data;
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 0);
}

void test_pop_back()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.pop_back();
    TRIAL_TEST_EQ(data.size(), 0);
}

void test_move_front()
{
    circular::array<int, 4> data;
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.move_front(), 11);
    TRIAL_TEST_EQ(data.size(), 0);
}

void test_move_back()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.move_back(), 11);
    TRIAL_TEST_EQ(data.size(), 0);
}

void run()
{
    test_ctor_default();
    test_ctor_move();
    test_ctor_move_assign();
    test_ctor_initializer_list();
    test_ctor_iterator();

    test_operator_assign_initializer_list();
    test_assign_initializer_list();
    test_assign_iterator();

    test_empty();
    test_capacity();
    test_size();
    test_max_size();
    test_full();
    test_front();
    test_back();
    test_operator_index();
    test_clear();
    test_push_front();
    test_push_back();
    test_pop_front();
    test_pop_back();
    test_move_front();
    test_move_back();
}

} // namespace api_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    api_suite::run();

    return boost::report_errors();
}
