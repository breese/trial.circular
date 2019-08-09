///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "impulse.hpp"

using namespace trial::circular::example;

int main()
{
    impulse<double, 2> filter(0.75, 0.25);

    filter.push(11.0);
    assert(filter.value() == 11.0 * 0.75);

    filter.push(22.0);
    assert(filter.value() == 22.0 * 0.75 + 11.0 * 0.25);

    filter.push(33.0);
    assert(filter.value() == 33.0 * 0.75 + 22.0 * 0.25);

    return 0;
}
