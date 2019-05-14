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
#include <numeric>
#include <trial/detail/lightweight_test.hpp>
#include <trial/circular/span.hpp>

using namespace trial;

//-----------------------------------------------------------------------------


namespace accumulate_suite
{

void test_overfull()
{
    std::array<int, 4> array;
    circular::span<int> span(array.begin(), array.end());
    span = { 11, 22, 33, 44 };
    TRIAL_TEST_EQ(std::accumulate(span.begin(), span.end(), 0), 11 + 22 + 33 + 44);
    span.push_back(55);
    TRIAL_TEST_EQ(std::accumulate(span.begin(), span.end(), 0), 22 + 33 + 44 + 55);
}

void run()
{
    test_overfull();
}

} // namespace accumulate_suite

//-----------------------------------------------------------------------------

namespace inner_product_suite
{

void test_overfull()
{
    std::array<int, 4> array;
    circular::span<int> span(array.begin(), array.end());
    span = { 11, 22, 33, 44 };
    TRIAL_TEST_EQ(std::inner_product(span.begin(), span.end(), array.begin(), 0),
                  11 * 11 + 22 * 22 + 33 * 33 + 44 * 44);
    span = { 11, 22, 33, 44, 55 };
    TRIAL_TEST_EQ(std::inner_product(span.begin(), span.end(), array.begin(), 0),
                  22 * 55 + 33 * 22 + 44 * 33 + 55 * 44);
}

void run()
{
    test_overfull();
}

} // namespace inner_product_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    accumulate_suite::run();
    inner_product_suite::run();

    return boost::report_errors();
}
