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
#include <boost/version.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace trial
{
namespace detail
{

void test_success()
{
#if BOOST_VERSION >= 106800
    ::boost::detail::test_results();
#else
    ::boost::detail::report_errors_remind();
#endif
}

void test_failure()
{
#if BOOST_VERSION >= 106800
    ::boost::detail::test_results().errors();
#else
    ++::boost::detail::test_errors();
#endif
}

// Cannot use boost::detail::test_with_impl (introduced in 1.68) for
// custom-defined predicates
template <class T, template <class> class BinaryPredicate>
void test_with_impl(char const * expr1,
                    char const * expr2,
                    char const * predicate_text,
                    char const * file, int line, char const * function,
                    const T& lhs, const T& rhs, BinaryPredicate<T> predicate)
{
    if (predicate(lhs, rhs))
    {
        test_success();
    }
    else
    {
        BOOST_LIGHTWEIGHT_TEST_OSTREAM
            << file << "(" << line << "): test '" << expr1 << " == " << expr2
            << "' failed in function '" << function << "': "
            << "'" << lhs << "' != '" << rhs << "' (" << predicate_text << ")" << std::endl;
        test_failure();
    }
}
               
#if !defined(BOOSST_TEST_ALL_EQ)
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
        test_success();
    }
    else
    {
        output << std::endl;
        test_failure();
    }
}
#endif

#if !defined(BOOST_TEST_ALL_WITH)
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
        test_success();
    }
    else
    {
        output << std::endl;
        test_failure();
    }
}
#endif

} // namespace detail
} // namespace trial

#define TRIAL_TEST BOOST_TEST
#define TRIAL_TEST_EQ BOOST_TEST_EQ

#define TRIAL_TEST_WITH(expr1,expr2,predicate) ( ::trial::detail::test_with_impl(#expr1, #expr2, #predicate, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, expr1, expr2, predicate) )

#if !defined(BOOST_TEST_ALL_WITH)
# define TRIAL_TEST_ALL_WITH(FIRST_BEGIN, FIRST_END, SECOND_BEGIN, SECOND_END, PREDICATE) ::trial::detail::test_all_with_impl(BOOST_LIGHTWEIGHT_TEST_OSTREAM, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, FIRST_BEGIN, FIRST_END, SECOND_BEGIN, SECOND_END, PREDICATE)
#else
# define TRIAL_TEST_ALL_WITH BOOST_TEST_ALL_WITH
#endif

#if !defined(BOOST_TEST_ALL_EQ)
# define TRIAL_TEST_ALL_EQ(FIRST_BEGIN, FIRST_END, SECOND_BEGIN, SECOND_END) ::trial::detail::test_all_eq_impl(BOOST_LIGHTWEIGHT_TEST_OSTREAM, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, FIRST_BEGIN, FIRST_END, SECOND_BEGIN, SECOND_END)
#else
# define TRIAL_TEST_ALL_EQ BOOST_TEST_ALL_EQ
#endif

#endif // TRIAL_DETAIL_LIGHTWEIGHT_TEST_HPP
