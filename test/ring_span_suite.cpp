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
#include <trial/circular/ring_span.hpp>

using namespace trial;

//-----------------------------------------------------------------------------

namespace api_null_suite
{

template <typename T>
using ring_span = circular::ring_span<T, circular::null_popper<T>>;

void api_pop_front()
{
    std::array<int, 4> array = { 11, 22, 33 };
    ring_span<int> span(array.begin(), array.end(), array.begin(), 3);
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.pop_front();
    {
        std::vector<int> expect = { 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    void api_pop_front();
}

} // namespace api_null_suite

//-----------------------------------------------------------------------------

namespace api_default_suite
{

template <typename T>
using ring_span = circular::ring_span<T, circular::default_popper<T>>;

void api_ctor_iterator()
{
    std::array<int, 4> array;
    ring_span<int> span(array.begin(), array.end());
    TRIAL_TEST(span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 0);
    TRIAL_TEST_EQ(span.capacity(), 4);
}

void api_ctor_iterator_init()
{
    std::array<int, 4> array = { 11, 22, 33 };
    ring_span<int> span(array.begin(), array.end(), array.begin(), 3);
    TRIAL_TEST(!span.empty());
    TRIAL_TEST(!span.full());
    TRIAL_TEST_EQ(span.size(), 3);
    TRIAL_TEST_EQ(span.capacity(), 4);
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_emplace_back()
{
    // FIXME
}

void api_pop_front()
{
    std::array<int, 4> array = { 11, 22, 33 };
    ring_span<int> span(array.begin(), array.end(), array.begin(), 3);
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    TRIAL_TEST_EQ(span.pop_front(), 11);
    {
        std::vector<int> expect = { 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void api_swap()
{
    std::array<int, 4> array1 = { 11, 22, 33 };
    ring_span<int> span1(array1.begin(), array1.end(), array1.begin(), 3);

    std::array<int, 4> array2 = { 55, 66, 77 };
    ring_span<int> span2(array2.begin(), array2.end(), array2.begin(), 3);

    span1.swap(span2);
    {
        std::vector<int> expect = { 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(span1.begin(), span1.end(),
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span2.begin(), span2.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    api_ctor_iterator();
    api_ctor_iterator_init();
    api_emplace_back();
    api_pop_front();
    api_swap();
}

} // namespace api_default_suite

//-----------------------------------------------------------------------------

namespace api_copy_suite
{

template <typename T>
using ring_span = circular::ring_span<T, circular::copy_popper<T>>;

void api_pop_front()
{
    std::array<int, 4> array = { 11, 22, 33, 44 };
    ring_span<int> span(array.begin(), array.end(), array.begin(), 4, circular::copy_popper<int>(77));
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    TRIAL_TEST_EQ(span.pop_front(), 11);
    {
        std::vector<int> expect = { 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 77, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(array.begin(), array.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    api_pop_front();
}

} // namespace api_copy_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    api_default_suite::run();
    api_null_suite::run();
    api_copy_suite::run();

    return boost::report_errors();
}
