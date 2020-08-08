///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2020 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <array>
#include <string>
#include <trial/detail/lightweight_test.hpp>
#include <trial/circular/deque.hpp>

using namespace trial;

//-----------------------------------------------------------------------------

namespace api_suite
{

void api_ctor_default()
{
    circular::deque<int> data;
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 0);
}

void api_ctor_capacity()
{
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 64);
}

void api_clear()
{
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    data.clear();
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_emplace_front()
{
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    data.emplace_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.front(), 11);
}

void api_emplace_back()
{
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    data.emplace_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.back(), 11);
}

void api_push_front()
{
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.front(), 11);
}

void api_push_back()
{
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.back(), 11);
}

void api_pop_front()
{
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    data.push_front(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.pop_front(), 11);
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_pop_back()
{
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.pop_back(), 11);
    TRIAL_TEST_EQ(data.size(), 0);
}

void api_reserve()
{
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.reserve(32);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.reserve(128);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 128);
}

void run()
{
    api_ctor_default();
    api_ctor_capacity();
    api_clear();
    api_emplace_front();
    api_emplace_back();
    api_push_front();
    api_push_back();
    api_pop_front();
    api_pop_back();
    api_reserve();
}

} // namespace api_suite

//-----------------------------------------------------------------------------

namespace capacity_suite
{

void push_pop_empty()
{
    circular::deque<int> data;
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 0);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.capacity(), 2);
    TRIAL_TEST_EQ(data.back(), 11);
    data.push_back(22);
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 2);
    TRIAL_TEST_EQ(data.back(), 22);

    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.capacity(), 2);
    TRIAL_TEST_EQ(data.back(), 22);
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 2);
}

void push_pop_small()
{
    circular::deque<int> data(2);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 2);
    data.push_back(11);
    TRIAL_TEST_EQ(data.capacity(), 2);
    {
        std::array<int, 1> expect = { 11 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.push_back(22);
    TRIAL_TEST_EQ(data.capacity(), 2);
    {
        std::array<int, 2> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.push_back(33);
    TRIAL_TEST_EQ(data.capacity(), 3);
    {
        std::array<int, 3> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.push_back(44);
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::array<int, 4> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.push_back(55);
    TRIAL_TEST_EQ(data.capacity(), 6);
    {
        std::array<int, 5> expect = { 11, 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.push_back(66);
    TRIAL_TEST_EQ(data.capacity(), 6);
    {
        std::array<int, 6> expect = { 11, 22, 33, 44, 55, 66 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.push_back(77);
    TRIAL_TEST_EQ(data.capacity(), 9);
    {
        std::array<int, 7> expect = { 11, 22, 33, 44, 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }

    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 9);
    {
        std::array<int, 6> expect = { 22, 33, 44, 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 9);
    {
        std::array<int, 5> expect = { 33, 44, 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 9);
    {
        std::array<int, 4> expect = { 44, 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 6);
    {
        std::array<int, 3> expect = { 55, 66, 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::array<int, 2> expect = { 66, 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 3);
    {
        std::array<int, 1> expect = { 77 };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 2);
}

void push_pop_big()
{
    // Capacity never changes
    circular::deque<int> data(64);
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.push_back(11);
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.push_back(22);
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.push_back(33);
    TRIAL_TEST_EQ(data.size(), 3);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.push_back(44);
    TRIAL_TEST_EQ(data.size(), 4);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.push_back(55);
    TRIAL_TEST_EQ(data.size(), 5);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.push_back(66);
    TRIAL_TEST_EQ(data.size(), 6);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.push_back(77);
    TRIAL_TEST_EQ(data.size(), 7);
    TRIAL_TEST_EQ(data.capacity(), 64);

    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 6);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 5);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 4);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 3);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 2);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 1);
    TRIAL_TEST_EQ(data.capacity(), 64);
    data.pop_front();
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 64);
}

void run()
{
    push_pop_empty();
    push_pop_small();
    push_pop_big();
}

} // namespace capacity_suite

//-----------------------------------------------------------------------------

namespace string_suite
{

void string_ctor_default()
{
    circular::deque<std::string> data;
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 0);
}

void string_push_pop_empty()
{
    circular::deque<std::string> data;
    TRIAL_TEST_EQ(data.size(), 0);
    TRIAL_TEST_EQ(data.capacity(), 0);
    data.push_back("alpha");
    TRIAL_TEST_EQ(data.capacity(), 2);
    {
        std::array<std::string, 1> expect = { "alpha" };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.push_back("bravo");
    TRIAL_TEST_EQ(data.capacity(), 2);
    {
        std::array<std::string, 2> expect = { "alpha", "bravo" };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.push_back("charlie");
    TRIAL_TEST_EQ(data.capacity(), 3);
    {
        std::array<std::string, 3> expect = { "alpha", "bravo", "charlie" };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.push_back("delta");
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::array<std::string, 4> expect = { "alpha", "bravo", "charlie", "delta" };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }

    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::array<std::string, 3> expect = { "bravo", "charlie", "delta" };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 4);
    {
        std::array<std::string, 2> expect = { "charlie", "delta" };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 3);
    {
        std::array<std::string, 1> expect = { "delta" };
        TRIAL_TEST_ALL_EQ(data.begin(), data.end(),
                          expect.begin(), expect.end());
        
    }
    data.pop_front();
    TRIAL_TEST_EQ(data.capacity(), 2);
    TRIAL_TEST_EQ(data.size(), 0);
}

void run()
{
    string_ctor_default();
    string_push_pop_empty();
}

} // namespace string_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    api_suite::run();
    capacity_suite::run();
    string_suite::run();

    return boost::report_errors();
}
