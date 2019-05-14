#ifndef TRIAL_DETAIL_LIGHTWEIGHT_TEST_HPP
#define TRIAL_DETAIL_LIGHTWEIGHT_TEST_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2016 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <algorithm>
#include <sstream>
#include <boost/detail/lightweight_test.hpp>

namespace trial
{
namespace detail
{

template <class T, template <class> class BinaryPredicate>
void test_with_impl(char const * expr1,
                    char const * expr2,
                    char const * predicate_text,
                    char const * file, int line, char const * function,
                    const T& lhs, const T& rhs, BinaryPredicate<T> predicate)
{
    if (predicate(lhs, rhs))
    {
        boost::detail::report_errors_remind();
    }
    else
    {
        BOOST_LIGHTWEIGHT_TEST_OSTREAM
            << file << "(" << line << "): test '" << expr1 << " == " << expr2
            << "' failed in function '" << function << "': "
            << "'" << lhs << "' != '" << rhs << "' (" << predicate_text << ")" << std::endl;
        ++boost::detail::test_errors();
    }
}
               
template<class T, class U>
inline void test_close_impl(char const * expr1,
                            char const * expr2,
                            char const * file, int line, char const * function,
                            const T& lhs, const U& rhs, double tolerance)
{
    if (std::fabs(lhs - rhs) <= tolerance)
    {
        boost::detail::report_errors_remind();
    }
    else
    {
        BOOST_LIGHTWEIGHT_TEST_OSTREAM
            << file << "(" << line << "): test '" << expr1 << " == " << expr2
            << "' failed in function '" << function << "': "
            << "'" << lhs << "' != '" << rhs << "'" << std::endl;
        ++boost::detail::test_errors();
    }
}

template<class FormattedOutputFunction, class InputIterator1, class InputIterator2>
void test_all_eq_impl(FormattedOutputFunction& output,
                      char const * file, int line, char const * function,
                      InputIterator1 first_begin, InputIterator1 first_end,
                      InputIterator2 second_begin, InputIterator2 second_end)
{
    InputIterator1 first_it = first_begin;
    InputIterator2 second_it = second_begin;
    typename std::iterator_traits<InputIterator1>::difference_type first_index = 0;
    typename std::iterator_traits<InputIterator2>::difference_type second_index = 0;
    std::size_t error_count = 0;
    const std::size_t max_count = 8;
    do
    {
        while ((first_it != first_end) && (second_it != second_end) && (*first_it == *second_it))
        {
            ++first_it;
            ++second_it;
            ++first_index;
            ++second_index;
        }
        if ((first_it == first_end) || (second_it == second_end))
        {
            break; // do-while
        }
        if (error_count == 0)
        {
            output << file << "(" << line << "): Container contents differ in function '" << function << "':";
        }
        else if (error_count >= max_count)
        {
            output << " ...";
            break;
        }
        output << " [" << first_index << "] '" << *first_it << "' != '" << *second_it << "'";
        ++first_it;
        ++second_it;
        ++first_index;
        ++second_index;
        ++error_count;
    } while (first_it != first_end);

    first_index += std::distance(first_it, first_end);
    second_index += std::distance(second_it, second_end);
    if (first_index != second_index)
    {
        if (error_count == 0)
        {
            output << file << "(" << line << "): Container sizes differ in function '" << function << "': size(" << first_index << ") != size(" << second_index << ")";
        }
        else
        {
            output << " [*] size(" << first_index << ") != size(" << second_index << ")";
        }
        ++error_count;
    }

    if (error_count == 0)
    {
        boost::detail::report_errors_remind();
    }
    else
    {
        output << std::endl;
        ++boost::detail::test_errors();
    }
}

template<class FormattedOutputFunction, class InputIterator1, class InputIterator2, typename Predicate>
void test_all_with_impl(FormattedOutputFunction& output,
                        char const * file, int line, char const * function,
                        InputIterator1 first_begin, InputIterator1 first_end,
                        InputIterator2 second_begin, InputIterator2 second_end,
                        Predicate predicate)
{
    InputIterator1 first_it = first_begin;
    InputIterator2 second_it = second_begin;
    typename std::iterator_traits<InputIterator1>::difference_type first_index = 0;
    typename std::iterator_traits<InputIterator2>::difference_type second_index = 0;
    std::size_t error_count = 0;
    const std::size_t max_count = 8;
    do
    {
        while ((first_it != first_end) && (second_it != second_end) && predicate(*first_it, *second_it))
        {
            ++first_it;
            ++second_it;
            ++first_index;
            ++second_index;
        }
        if ((first_it == first_end) || (second_it == second_end))
        {
            break; // do-while
        }
        if (error_count == 0)
        {
            output << file << "(" << line << "): Container contents differ in function '" << function << "':";
        }
        else if (error_count >= max_count)
        {
            output << " ...";
            break;
        }
        output << " [" << first_index << "]";
        ++first_it;
        ++second_it;
        ++first_index;
        ++second_index;
        ++error_count;
    } while (first_it != first_end);

    first_index += std::distance(first_it, first_end);
    second_index += std::distance(second_it, second_end);
    if (first_index != second_index)
    {
        if (error_count == 0)
        {
            output << file << "(" << line << "): Container sizes differ in function '" << function << "': size(" << first_index << ") != size(" << second_index << ")";
        }
        else
        {
            output << " [*] size(" << first_index << ") != size(" << second_index << ")";
        }
        ++error_count;
    }

    if (error_count == 0)
    {
        boost::detail::report_errors_remind();
    }
    else
    {
        output << std::endl;
        ++boost::detail::test_errors();
    }
}

} // namespace detail
} // namespace trial

#define TRIAL_TEST BOOST_TEST
#define TRIAL_TEST_EQ BOOST_TEST_EQ

#define TRIAL_TEST_WITH(expr1,expr2,predicate) ( ::trial::detail::test_with_impl(#expr1, #expr2, #predicate, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, expr1, expr2, predicate) )

#define TRIAL_TEST_CLOSE(LHS, RHS, TOLERANCE) ::trial::detail::test_close_impl(#LHS, #RHS, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, LHS, RHS, TOLERANCE)

#define TRIAL_TEST_ALL_WITH(FIRST_BEGIN, FIRST_END, SECOND_BEGIN, SECOND_END, PREDICATE) ::trial::detail::test_all_with_impl(BOOST_LIGHTWEIGHT_TEST_OSTREAM, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, FIRST_BEGIN, FIRST_END, SECOND_BEGIN, SECOND_END, PREDICATE)

#define TRIAL_TEST_ALL_EQ(FIRST_BEGIN, FIRST_END, SECOND_BEGIN, SECOND_END) ::trial::detail::test_all_eq_impl(BOOST_LIGHTWEIGHT_TEST_OSTREAM, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, FIRST_BEGIN, FIRST_END, SECOND_BEGIN, SECOND_END)

#endif // TRIAL_DETAIL_LIGHTWEIGHT_TEST_HPP
