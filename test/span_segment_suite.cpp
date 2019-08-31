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

void api_data()
{
    int array[4] = {};
    circular::span<int> span(array);
    {
        auto first = span.first_segment();
        TRIAL_TEST(first.data() == nullptr);
        const auto cfirst = span.first_segment();
        TRIAL_TEST(cfirst.data() == nullptr);
        auto last = span.last_segment();
        TRIAL_TEST(last.data() == nullptr);
        const auto clast = span.last_segment();
        TRIAL_TEST(clast.data() == nullptr);
    }
    span = { 11 };
    {
        auto first = span.first_segment();
        TRIAL_TEST(first.data() == &*span.begin());
        const auto cfirst = span.first_segment();
        TRIAL_TEST(cfirst.data() == &*span.begin());
        auto last = span.last_segment();
        TRIAL_TEST(last.data() != &*span.end());
        const auto clast = span.last_segment();
        TRIAL_TEST(clast.data() != &*span.end());
    }
    // 55 22 33 44
    // -> <-------
    span = { 11, 22, 33, 44, 55 };
    {
        auto first = span.first_segment();
        TRIAL_TEST(first.data() == &*span.begin());
        const auto cfirst = span.first_segment();
        TRIAL_TEST(cfirst.data() == &*span.begin());
        auto last = span.last_segment();
        TRIAL_TEST(last.data() != &*span.begin());
        const auto clast = span.last_segment();
        TRIAL_TEST(clast.data() != &*span.begin());
    }
}

void api_data_const()
{
    int array[4] = {};
    circular::span<int> original(array);
    {
        const circular::span<int> span(original);
        auto first = span.first_segment();
        TRIAL_TEST(first.data() == nullptr);
        const auto cfirst = span.first_segment();
        TRIAL_TEST(cfirst.data() == nullptr);
        auto last = span.last_segment();
        TRIAL_TEST(last.data() == nullptr);
        const auto clast = span.last_segment();
        TRIAL_TEST(clast.data() == nullptr);
    }
    original = { 11 };
    {
        const circular::span<int> span(original);
        auto first = span.first_segment();
        TRIAL_TEST(first.data() == &*span.begin());
        const auto cfirst = span.first_segment();
        TRIAL_TEST(cfirst.data() == &*span.begin());
        auto last = span.last_segment();
        TRIAL_TEST(last.data() != &*span.end());
        const auto clast = span.last_segment();
        TRIAL_TEST(clast.data() != &*span.end());
    }
    // 55 22 33 44
    // -> <-------
    original = { 11, 22, 33, 44, 55 };
    {
        const circular::span<int> span(original);
        auto first = span.first_segment();
        TRIAL_TEST(first.data() == &*span.begin());
        const auto cfirst = span.first_segment();
        TRIAL_TEST(cfirst.data() == &*span.begin());
        auto last = span.last_segment();
        TRIAL_TEST(last.data() != &*span.begin());
        const auto clast = span.last_segment();
        TRIAL_TEST(clast.data() != &*span.begin());
    }
}

void api_size()
{
    int array[4] = {};
    circular::span<int> span(array);
    {
        auto first = span.first_segment();
        TRIAL_TEST_EQ(first.size(), 0);
        auto last = span.last_segment();
        TRIAL_TEST_EQ(last.size(), 0);
    }
    span = { 11 };
    {
        auto first = span.first_segment();
        TRIAL_TEST_EQ(first.size(), 1);
        auto last = span.last_segment();
        TRIAL_TEST_EQ(last.size(), 0);
    }
    // 55 22 33 44
    // -> <-------
    span = { 11, 22, 33, 44, 55 };
    {
        auto first = span.first_segment();
        TRIAL_TEST_EQ(first.size(), 3);
        auto last = span.last_segment();
        TRIAL_TEST_EQ(last.size(), 1);
    }
}

void run()
{
    api_data();
    api_data_const();
    api_size();
}

} // namespace api_suite

//-----------------------------------------------------------------------------

namespace content_suite
{

void segment_empty()
{
    int array[4] = {};
    circular::span<int> span(array);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 0, 0, 0, 0 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
}

void segment_partial()
{
    int array[4] = {};
    circular::span<int> span(array);
    // 11 X X X
    span = { 11 };
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 0, 0, 0 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 11 22 X X
    span.push_back(22);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 0, 0 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 11 22 33 X
    span.push_back(33);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 0 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 11 22 33 44
    span.push_back(44);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
}

void segment_overfull()
{
    int array[4] = {};
    circular::span<int> span(array);
    // 55 22 33 44
    span = { 11, 22, 33, 44, 55 };
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { 55 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 55 66 33 44
    span.push_back(66);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 33, 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { 55, 66 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 55 66 77 44
    span.push_back(77);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 55 66 77 88
    span.push_back(88);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 99 66 77 88
    span.push_back(99);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { 99 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
}

void segment_overfull_1()
{
    int array[4] = {};
    circular::span<int> span(array);
    // 55 X 33 44
    span = { 11, 22, 33, 44, 55 };
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 33, 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { 55 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 22 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 55 66 X 44
    span.push_back(66);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { 55, 66 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 33 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 55 66 77 X
    span.push_back(77);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // X 66 77 88
    span.push_back(88);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 55 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 99 X 77 88
    span.push_back(99);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 77, 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { 99 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 66 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
}

void segment_overfull_2()
{
    int array[4] = {};
    circular::span<int> span(array);
    // 55 X X 44
    span = { 11, 22, 33, 44, 55 };
    span.remove_front(2);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { 55 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 22, 33 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 55 66 X X
    span.push_back(66);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 55, 66 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 33, 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // X 66 77 X
    span.push_back(77);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 66, 77 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { 55 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // X X 77 88
    span.push_back(88);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 77, 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 55, 66 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 99 X X 88
    span.push_back(99);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { 99 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 66, 77 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
}

void segment_overfull_3()
{
    int array[4] = {};
    circular::span<int> span(array);
    // 55 X X X
    span = { 11, 22, 33, 44, 55 };
    span.remove_front(3);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 55 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // X 66 X X
    span.push_back(66);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 66 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 33, 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { 55 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // X X 77 X
    span.push_back(77);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 77 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { 55, 66 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // X X X 88
    span.push_back(88);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // 99 X X X
    span.push_back(99);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { 99 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
}

void segment_overfull_4()
{
    int array[4] = {};
    circular::span<int> span(array);
    // Z X X X
    span = { 11, 22, 33, 44, 55 };
    span.remove_front(4);
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { 55 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // X Z X X
    span.push_back(66);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 33, 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { 55, 66 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // X X Z X
    span.push_back(77);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 44 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // X X X Z
    span.push_back(88);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 55, 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    // Z X X X
    span.push_back(99);
    span.remove_front();
    {
        auto segment = span.first_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_segment();

        std::vector<int> expect = { };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.first_unused_segment();

        std::vector<int> expect = { 66, 77, 88 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
    {
        auto segment = span.last_unused_segment();

        std::vector<int> expect = { 99 };
        TRIAL_TEST_ALL_EQ(segment.data(), segment.data() + segment.size(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    segment_empty();
    segment_partial();
    segment_overfull();
    segment_overfull_1();
    segment_overfull_2();
    segment_overfull_3();
    segment_overfull_4();
}

} // namespace content_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    api_suite::run();
    content_suite::run();
 
    return boost::report_errors();
}
