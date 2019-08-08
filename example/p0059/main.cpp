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
#include "ring_span.hpp"

int main()
{
    using namespace trial::circular::example;

    // The copy_popper overwrites popped values in the underlying storage

    std::array<int, 4> array = { 11, 22, 33 };
    ring_span<int, copy_popper<int>> span(array.begin(), array.end(), array.begin(), 3, copy_popper<int>(99));
    assert(span.size() == 3);
    assert(span.front() == 11);
    assert(span.back() == 33);

    span.pop_front();
    assert(span.size() == 2);
    assert(span.front() == 22);
    assert(span.back() == 33);

    return 0;
}
