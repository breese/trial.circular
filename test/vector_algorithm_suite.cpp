///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <vector>
#include <trial/detail/lightweight_test.hpp>
#include <trial/circular/vector.hpp>

using namespace trial;

//-----------------------------------------------------------------------------

namespace copy_suite
{

void copy_back_inserter()
{
    circular::vector<int> data(4);
    data = { 11, 22, 33 };
    std::vector<int> input = { 111, 222, 333, 444, 555 };
    std::copy(input.begin(), input.end(), std::back_inserter(data));
    {
        std::vector<int> expect = { 222, 333, 444, 555 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(), expect.begin(), expect.end());
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

void count_below_capacity()
{
    circular::vector<int> data(4);
    data = { 11, 22, 33 };
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 11), 1);
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 22), 1);
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 33), 1);
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 44), 0);
}

void count_above_capacity()
{
    circular::vector<int> data(4);
    data = { 11, 22, 33, 44, 55 };
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 11), 0); // Pushed out of span
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 22), 1);
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 33), 1);
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 44), 1);
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 55), 1);
    TRIAL_TEST_EQ(std::count(data.begin(), data.end(), 66), 0);
}

void run()
{
    count_below_capacity();
    count_above_capacity();
}

} // namespace count_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    copy_suite::run();
    count_suite::run();

    return boost::report_errors();
}
