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

void api_ctor_default()
{
    circular::array<int, 4> data;
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void api_ctor_copy()
{
    circular::array<int, 4> data;
    data.push_back(11);
    circular::array<int, 4> copy(data);
    TRIAL_TEST_EQ(copy.size(), 1);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_copy_assign()
{
    circular::array<int, 4> data;
    data.push_back(11);
    circular::array<int, 4> copy;
    copy = data;
    TRIAL_TEST_EQ(copy.size(), 1);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_move()
{
    circular::array<int, 4> data;
    data.push_back(11);
    circular::array<int, 4> copy(std::move(data));
    TRIAL_TEST_EQ(copy.size(), 1);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_move_assign()
{
    circular::array<int, 4> data;
    data.push_back(11);
    circular::array<int, 4> copy;
    copy = std::move(data);
    TRIAL_TEST_EQ(copy.size(), 1);
    TRIAL_TEST_EQ(copy.capacity(), 4);
}

void api_ctor_initializer_list()
{
    circular::array<int, 4> data({11, 22});
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void api_operator_assign_initializer_list()
{
    circular::array<int, 4> data;
    data = {11, 22};
    TRIAL_TEST_EQ(data.size(), 2);
}

void api_assign_initializer_list()
{
    circular::array<int, 4> data;
    data.assign({11, 22});
    TRIAL_TEST_EQ(data.size(), 2);
}

void api_assign_iterator()
{
    circular::array<int, 4> data;
    std::vector<int> input = {11, 22};
    data.assign(input.begin(), input.end());
    TRIAL_TEST_EQ(data.size(), 2);
}

void api_empty()
{
    circular::array<int, 4> data;
    TRIAL_TEST(data.empty());
}

void api_capacity()
{
    circular::array<int, 4> data;
    TRIAL_TEST_EQ(data.capacity(), 4);
}

void api_size()
{
    circular::array<int, 4> data;
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_max_size()
{
    circular::array<int, 4> data;
    TRIAL_TEST_EQ(data.max_size(), 4);
}

void api_full()
{
    circular::array<int, 4> data;
    TRIAL_TEST(!data.full());
}

void api_front()
{
    circular::array<int, 4> data;
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.front(), 11);
}

void api_back()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.back(), 11);
}

void api_first_segment()
{
    circular::array<int, 4> data = { 11, 22, 33 };
    auto segment = data.first_segment();
    TRIAL_TEST_EQ(segment.size(), 3);
}

void api_last_segment()
{
    circular::array<int, 4> data = { 11, 22, 33 };
    auto segment = data.last_segment();
    TRIAL_TEST_EQ(segment.size(), 0);
}

void api_operator_index()
{
    circular::array<int, 4> data = {11, 22, 33};
    TRIAL_TEST_EQ(data[0], 11);
    TRIAL_TEST_EQ(data[1], 22);
    TRIAL_TEST_EQ(data[2], 33);
}

void api_clear()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.clear();
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_push_front()
{
    circular::array<int, 4> data;
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void api_push_front_iterator()
{
    circular::array<int, 4> data;
    std::vector<int> input = { 11, 22 };
    data.push_front(input.begin(), input.end());
    TRIAL_TEST_EQ(data.size(), 2);
}

void api_push_back()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
}

void api_push_back_iterator()
{
    circular::array<int, 4> data;
    std::vector<int> input = { 11, 22 };
    data.push_back(input.begin(), input.end());
    TRIAL_TEST_EQ(data.size(), 2);
}

void api_pop_front()
{
    circular::array<int, 4> data;
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.pop_front(), 11);
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_pop_back()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.pop_back(), 11);
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_expand_front()
{
    circular::array<int, 4> data;
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.expand_front();
    TRIAL_TEST_EQ(data.size(), 2);
}

void api_expand_front_n()
{
    circular::array<int, 4> data;
    data.push_front(11);
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 2);
    data.expand_front(2);
    TRIAL_TEST_EQ(data.size(), 4);
}

void api_remove_front()
{
    circular::array<int, 4> data;
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.remove_front();
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_remove_front_n()
{
    circular::array<int, 4> data;
    data.push_front(11);
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 2);
    data.remove_front(2);
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_expand_back()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.expand_back();
    TRIAL_TEST_EQ(data.size(), 2);
}

void api_expand_back_n()
{
    circular::array<int, 4> data;
    data.push_back(11);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 2);
    data.expand_back(2);
    TRIAL_TEST_EQ(data.size(), 4);
}

void api_remove_back()
{
    circular::array<int, 4> data;
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.remove_back();
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_remove_back_n()
{
    circular::array<int, 4> data;
    data.push_back(11);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 2);
    data.remove_back(2);
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_begin_end()
{
    circular::array<int, 4> data;
    data = { 11, 22, 33 };
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
    }
}

void api_cbegin_cend()
{
    circular::array<int, 4> data;
    data = { 11, 22, 33 };
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(data.cbegin(), data.cend(),
                          expect.begin(), expect.end());
    }
}

void api_rbegin_rend()
{
    circular::array<int, 4> data;
    data = { 11, 22, 33 };
    {
        std::vector<int> expect = { 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(data.rbegin(), data.rend(),
                          expect.begin(), expect.end());
    }
}

void api_crbegin_crend()
{
    circular::array<int, 4> data;
    data = { 11, 22, 33 };
    {
        std::vector<int> expect = { 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(data.crbegin(), data.crend(),
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
    api_ctor_initializer_list();

    api_operator_assign_initializer_list();
    api_assign_initializer_list();
    api_assign_iterator();

    api_empty();
    api_capacity();
    api_size();
    api_max_size();
    api_full();
    api_front();
    api_back();
    api_first_segment();
    api_last_segment();
    api_operator_index();
    api_clear();
    api_push_front();
    api_push_front_iterator();
    api_push_back();
    api_push_back_iterator();
    api_pop_front();
    api_pop_back();
    api_expand_front();
    api_expand_front_n();
    api_remove_front();
    api_remove_front_n();
    api_expand_back();
    api_expand_back_n();
    api_remove_back();
    api_remove_back_n();

    api_begin_end();
    api_cbegin_cend();
    api_rbegin_rend();
    api_crbegin_crend();
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
