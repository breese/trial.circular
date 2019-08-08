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
#include "average.hpp"

int main()
{
    trial::circular::example::average<double, 4> filter;

    filter.push(11.0);
    assert(filter.mean() == 11.0);

    filter.push(22.0);
    assert(filter.mean() == (11.0 + 22.0) / 2.0);

    filter.push(33.0);
    assert(filter.mean() == (11.0 + 22.0 + 33.0) / 3.0);

    filter.push(44.0);
    assert(filter.mean() == (11.0 + 22.0 + 33.0 + 44.0) / 4.0);

    filter.push(55.0);
    assert(filter.mean() == (22.0 + 33.0 + 44.0 + 55.0) / 4.0);

    return 0;
}
