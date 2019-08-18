///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <thread>
#include <functional>
#include <string> // std::stoul
#include <iostream>
#include "queue.hpp"

using concurrent_queue = trial::circular::example::concurrent_queue<std::size_t, 64>;

void producer(concurrent_queue& queue,
              std::size_t amount)
{
    // The producer keeps writing to the queue at high speed, so it may
    // fill up which causes us to lose some of the values at the beginning
    // of the queue, i.e. we lose the oldest values at insertion time.
    //
    // Zero is used to indicate that no further values will be inserted.

    do
    {
        --amount;
        queue.push(amount);
    } while (amount > 0);
}

int main(int argc, char *argv[])
{
    std::size_t amount = (argc > 1) ? std::stoul(argv[1]) : 1;
        
    concurrent_queue queue;

    std::thread thread(std::bind(producer, std::ref(queue), amount));

    // Main thread acts as consumer

    // The consumer reads all values from the queue. If the queue has filled
    // up then the producer will overwrite older values, and we will be unable
    // to read those. Lost values are printed out.

    std::size_t expect = amount - 1;
    for (std::size_t result = queue.pop(); result > 0; result = queue.pop())
    {
        if (result != expect)
        {
            std::cout << "Lost: " << result << " - " << expect << '\n';
            expect = result;
        }
        --expect;
    }

    thread.join();
    return 0;
}
