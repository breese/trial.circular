#ifndef TRIAL_CIRCULAR_EXAMPLE_CONCURRENT_QUEUE_HPP
#define TRIAL_CIRCULAR_EXAMPLE_CONCURRENT_QUEUE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <mutex>
#include <condition_variable>
#include <trial/circular/array.hpp>

namespace trial
{
namespace circular
{
namespace example
{

template <typename T, std::size_t N>
class concurrent_queue
{
public:
    using value_type = T;
    using size_type = std::size_t;

    concurrent_queue() = default;

    //! @brief Inserts element at end.

    void push(value_type input)
    {
        std::lock_guard<std::mutex> guard(mutex);
        data.push_back(std::move(input));
        notifier.notify_all();
    }

    //! @brief Removes and returns first element.

    value_type pop()
    {
        std::unique_lock<std::mutex> guard(mutex);
        if (data.empty())
            notifier.wait(guard, [this] { return !data.empty(); });
        return data.pop_front();
    }

private:
    std::mutex mutex;
    std::condition_variable notifier;
    circular::array<T, N> data;
};

} // namespace example
} // namespace circular
} // namespace trial

#endif // TRIAL_CIRCULAR_EXAMPLE_CONCURRENT_QUEUE_HPP
