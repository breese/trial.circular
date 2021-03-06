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
#include <algorithm>
#include <iterator>
#include <trial/detail/lightweight_test.hpp>
#include <trial/circular/span.hpp>

using namespace trial;

//-----------------------------------------------------------------------------

namespace concept_suite
{

void iterator_move_constructible()
{
    static_assert(std::is_move_constructible<circular::span<int>::iterator>::value, "is_move_constructible");
}

void iterator_copy_constructible()
{
    static_assert(std::is_copy_constructible<circular::span<int>::iterator>::value, "is_copy_constructible");
}

void iterator_copy_assignable()
{
    static_assert(std::is_copy_assignable<circular::span<int>::iterator>::value, "is_copy_assignable");
}

void iterator_destructible()
{
    static_assert(std::is_destructible<circular::span<int>::iterator>::value, "is_destructible");
}

void iterator_swappable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    circular::span<int>::iterator b = span.end();
    using std::swap;
    swap(a, b);
    TRIAL_TEST(a == span.end());
    TRIAL_TEST(b == span.begin());
}

void iterator_traits()
{
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::iterator>::value_type, circular::span<int>::iterator::value_type>::value, "value_type");
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::iterator>::difference_type, circular::span<int>::iterator::difference_type>::value, "difference_type");
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::iterator>::reference, circular::span<int>::iterator::reference>::value, "reference");
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::iterator>::pointer, circular::span<int>::iterator::pointer>::value, "pointer");
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::iterator>::iterator_category, circular::span<int>::iterator::iterator_category>::value, "iterator_category");
}

void iterator_dereferenceable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    int result = *a;
    TRIAL_TEST_EQ(result, 11);
}

void iterator_incrementable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    circular::span<int>::iterator& b = ++a;
    TRIAL_TEST(b == a);
    TRIAL_TEST(b != span.begin());
}

void input_iterator_equality_comparable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    circular::span<int>::iterator b = span.begin();
    circular::span<int>::iterator c = span.begin();
    TRIAL_TEST(a == a);
    TRIAL_TEST(a == b);
    TRIAL_TEST(b == a);
    TRIAL_TEST(b == c);
    TRIAL_TEST(a == c);

    circular::span<int>::iterator d = span.end();
    TRIAL_TEST(a != d);
}

void input_iterator_dereferenceable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    TRIAL_TEST_EQ(*a, 11);
}

void input_iterator_post_incrementable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    circular::span<int>::iterator b = a++;
    TRIAL_TEST(a != span.begin());
    TRIAL_TEST(b == span.begin());
}

void forward_iterator_default_constructible()
{
    // Only valid operations on default constructed iterator are assignment and
    // destruction (see [iterator.requirements.general] / 7)

    static_assert(std::is_default_constructible<circular::span<int>::iterator>::value, "default constructible");

    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    circular::span<int>::iterator b; // Singular value not associated with span
    b = span.begin();
    TRIAL_TEST(a == b);
    TRIAL_TEST(b == a);
}

void forward_iterator_multipass()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    circular::span<int>::iterator b = a;
    TRIAL_TEST(a == b);
    ++a;
    ++b;
    TRIAL_TEST(a == b);
    TRIAL_TEST(*a == *b);
}

void bidirectional_iterator_decrementable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    ++a;
    circular::span<int>::iterator& b = --a;
    TRIAL_TEST(b == a);
    TRIAL_TEST(b == span.begin());
}

void bidirectional_iterator_post_decrementable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    ++a;
    circular::span<int>::iterator b = a--;
    TRIAL_TEST(a == span.begin());
    TRIAL_TEST(b != span.begin());
}

void random_access_iterator_addition_assignment()
{
    // r += n
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin();
    a += 2;
    TRIAL_TEST_EQ(std::distance(span.begin(), a), 2);
}

void random_access_iterator_addition()
{
    // a + n
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.begin() + 2;
    TRIAL_TEST_EQ(std::distance(span.begin(), a), 2);
    // n + a
    circular::span<int>::iterator b = 2 + span.begin();
    TRIAL_TEST_EQ(std::distance(span.begin(), b), 2);
}

void random_access_iterator_subtraction_assignment()
{
    // r -= n
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.end();
    a -= 2;
    TRIAL_TEST_EQ(std::distance(span.begin(), a), 2);
}

void random_access_iterator_subtraction()
{
    // a - n
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::iterator a = span.end() - 2;
    TRIAL_TEST_EQ(std::distance(span.begin(), a), 2);
}

void random_access_iterator_difference()
{
    // b - a
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> alpha(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int> bravo(data.begin(), data.end(), std::next(data.begin()), data.size() - 1);
    TRIAL_TEST_EQ(alpha.begin() - bravo.begin(), -1);
    TRIAL_TEST_EQ(bravo.begin() - alpha.begin(), 1);
}

void random_access_iterator_index()
{
    // a[n]
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST_EQ(span.begin()[2], 33);
    TRIAL_TEST_EQ(span.cbegin()[2], 33);
}

void random_access_iterator_less()
{
    // a < b
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST(!(span.begin() < span.begin()));
    TRIAL_TEST(span.begin() < span.end());
    TRIAL_TEST(!(span.end() < span.begin()));
    TRIAL_TEST(!(span.end() < span.end()));
}

void random_access_iterator_less_equal()
{
    // a <= b
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST(span.begin() <= span.begin());
    TRIAL_TEST(span.begin() <= span.end());
    TRIAL_TEST(!(span.end() <= span.begin()));
    TRIAL_TEST(span.end() <= span.end());
}

void random_access_iterator_greater()
{
    // a > b
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST(!(span.begin() > span.begin()));
    TRIAL_TEST(!(span.begin() > span.end()));
    TRIAL_TEST(span.end() > span.begin());
    TRIAL_TEST(!(span.end() > span.end()));
}

void random_access_iterator_greater_equal()
{
    // a >= b
    std::vector<int> data = { 11, 22, 33, 44 };
    circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST(span.begin() >= span.begin());
    TRIAL_TEST(!(span.begin() >= span.end()));
    TRIAL_TEST(span.end() >= span.begin());
    TRIAL_TEST(span.end() >= span.end());
}

void run()
{
    // [iterator.iterators]
    iterator_move_constructible();
    iterator_copy_constructible();
    iterator_copy_assignable();
    iterator_destructible();
    iterator_swappable();
    iterator_traits();
    iterator_dereferenceable();
    iterator_incrementable();

    // [input.iterators]
    input_iterator_equality_comparable();
    input_iterator_dereferenceable();
    input_iterator_post_incrementable();

    // [forward.iterators]
    forward_iterator_default_constructible();
    forward_iterator_multipass();

    // [bidirectional.iterators]
    bidirectional_iterator_decrementable();
    bidirectional_iterator_post_decrementable();

    // [random.access.iterators]
    random_access_iterator_addition_assignment();
    random_access_iterator_addition();
    random_access_iterator_subtraction_assignment();
    random_access_iterator_subtraction();
    random_access_iterator_difference();
    random_access_iterator_index();
    random_access_iterator_less();
    random_access_iterator_less_equal();
    random_access_iterator_greater();
    random_access_iterator_greater_equal();
}

} // namespace concept_suite

//-----------------------------------------------------------------------------

namespace concept_const_suite
{

void iterator_move_constructible()
{
    static_assert(std::is_move_constructible<circular::span<int>::const_iterator>::value, "is_move_constructible");
}

void iterator_copy_constructible()
{
    static_assert(std::is_copy_constructible<circular::span<int>::const_iterator>::value, "is_copy_constructible");
}

void iterator_copy_assignable()
{
    static_assert(std::is_copy_assignable<circular::span<int>::const_iterator>::value, "is_copy_assignable");
}

void iterator_destructible()
{
    static_assert(std::is_destructible<circular::span<int>::const_iterator>::value, "is_destructible");
}

void iterator_swappable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    circular::span<int>::const_iterator b = span.end();
    using std::swap;
    swap(a, b);
    TRIAL_TEST(a == span.end());
    TRIAL_TEST(b == span.begin());
}

void iterator_traits()
{
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::const_iterator>::value_type, circular::span<int>::const_iterator::value_type>::value, "value_type");
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::const_iterator>::difference_type, circular::span<int>::const_iterator::difference_type>::value, "difference_type");
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::const_iterator>::reference, circular::span<int>::const_iterator::reference>::value, "reference");
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::const_iterator>::pointer, circular::span<int>::const_iterator::pointer>::value, "pointer");
    static_assert(std::is_same<std::iterator_traits<circular::span<int>::const_iterator>::iterator_category, circular::span<int>::const_iterator::iterator_category>::value, "iterator_category");
}

void iterator_dereferenceable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    int result = *a;
    TRIAL_TEST_EQ(result, 11);
}

void iterator_incrementable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    circular::span<int>::const_iterator& b = ++a;
    TRIAL_TEST(b == a);
    TRIAL_TEST(b != span.begin());
}

void input_iterator_equality_comparable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    circular::span<int>::const_iterator b = span.begin();
    circular::span<int>::const_iterator c = span.begin();
    TRIAL_TEST(a == a);
    TRIAL_TEST(a == b);
    TRIAL_TEST(b == a);
    TRIAL_TEST(b == c);
    TRIAL_TEST(a == c);

    circular::span<int>::const_iterator d = span.end();
    TRIAL_TEST(a != d);
}

void input_iterator_dereferenceable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    TRIAL_TEST_EQ(*a, 11);
}

void input_iterator_post_incrementable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    circular::span<int>::const_iterator b = a++;
    TRIAL_TEST(a != span.begin());
    TRIAL_TEST(b == span.begin());
}

void forward_iterator_default_constructible()
{
    // Only valid operations on default constructed iterator are assignment and
    // destruction (see [iterator.requirements.general] / 7)

    static_assert(std::is_default_constructible<circular::span<int>::const_iterator>::value, "default constructible");

    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    circular::span<int>::const_iterator b; // Singular value not associated with span
    b = span.begin();
    TRIAL_TEST(a == b);
    TRIAL_TEST(b == a);
}

void forward_iterator_multipass()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    circular::span<int>::const_iterator b = a;
    TRIAL_TEST(a == b);
    ++a;
    ++b;
    TRIAL_TEST(a == b);
    TRIAL_TEST(*a == *b);
}

void bidirectional_iterator_decrementable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    ++a;
    circular::span<int>::const_iterator& b = --a;
    TRIAL_TEST(b == a);
    TRIAL_TEST(b == span.begin());
}

void bidirectional_iterator_post_decrementable()
{
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    ++a;
    circular::span<int>::const_iterator b = a--;
    TRIAL_TEST(a == span.begin());
    TRIAL_TEST(b != span.begin());
}

void random_access_iterator_addition_assignment()
{
    // r += n
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin();
    a += 2;
    TRIAL_TEST_EQ(std::distance(span.begin(), a), 2);
}

void random_access_iterator_addition()
{
    // a + n
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.begin() + 2;
    TRIAL_TEST_EQ(std::distance(span.begin(), a), 2);
    // n + a
    circular::span<int>::const_iterator b = 2 + span.begin();
    TRIAL_TEST_EQ(std::distance(span.begin(), b), 2);
}

void random_access_iterator_subtraction_assignment()
{
    // r -= n
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.end();
    a -= 2;
    TRIAL_TEST_EQ(std::distance(span.begin(), a), 2);
}

void random_access_iterator_subtraction()
{
    // a - n
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    circular::span<int>::const_iterator a = span.end() - 2;
    TRIAL_TEST_EQ(std::distance(span.begin(), a), 2);
}

void random_access_iterator_difference()
{
    // b - a
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> alpha(data.begin(), data.end(), data.begin(), data.size());
    const circular::span<int> bravo(data.begin(), data.end(), std::next(data.begin()), data.size() - 1);
    TRIAL_TEST_EQ(alpha.begin() - bravo.begin(), -1);
    TRIAL_TEST_EQ(bravo.begin() - alpha.begin(), 1);
}

void random_access_iterator_index()
{
    // a[n]
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST_EQ(span.begin()[2], 33);
    TRIAL_TEST_EQ(span.cbegin()[2], 33);
}

void random_access_iterator_less()
{
    // a < b
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST(!(span.begin() < span.begin()));
    TRIAL_TEST(span.begin() < span.end());
    TRIAL_TEST(!(span.end() < span.begin()));
    TRIAL_TEST(!(span.end() < span.end()));
}

void random_access_iterator_less_equal()
{
    // a <= b
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST(span.begin() <= span.begin());
    TRIAL_TEST(span.begin() <= span.end());
    TRIAL_TEST(!(span.end() <= span.begin()));
    TRIAL_TEST(span.end() <= span.end());
}

void random_access_iterator_greater()
{
    // a > b
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST(!(span.begin() > span.begin()));
    TRIAL_TEST(!(span.begin() > span.end()));
    TRIAL_TEST(span.end() > span.begin());
    TRIAL_TEST(!(span.end() > span.end()));
}

void random_access_iterator_greater_equal()
{
    // a >= b
    std::vector<int> data = { 11, 22, 33, 44 };
    const circular::span<int> span(data.begin(), data.end(), data.begin(), data.size());
    TRIAL_TEST(span.begin() >= span.begin());
    TRIAL_TEST(!(span.begin() >= span.end()));
    TRIAL_TEST(span.end() >= span.begin());
    TRIAL_TEST(span.end() >= span.end());
}

void run()
{
    // [iterator.iterators]
    iterator_move_constructible();
    iterator_copy_constructible();
    iterator_copy_assignable();
    iterator_destructible();
    iterator_swappable();
    iterator_traits();
    iterator_dereferenceable();
    iterator_incrementable();

    // [input.iterators]
    input_iterator_equality_comparable();
    input_iterator_dereferenceable();
    input_iterator_post_incrementable();

    // [forward.iterators]
    forward_iterator_default_constructible();
    forward_iterator_multipass();

    // [bidirectional.iterators]
    bidirectional_iterator_decrementable();
    bidirectional_iterator_post_decrementable();

    // [random.access.iterators]
    random_access_iterator_addition_assignment();
    random_access_iterator_addition();
    random_access_iterator_subtraction_assignment();
    random_access_iterator_subtraction();
    random_access_iterator_difference();
    random_access_iterator_index();
    random_access_iterator_less();
    random_access_iterator_less_equal();
    random_access_iterator_greater();
    random_access_iterator_greater_equal();
}

} // namespace concept_suite

//-----------------------------------------------------------------------------

namespace iterator_suite
{

void test_empty()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    TRIAL_TEST(span.begin() == span.end());
}

void test_iterator()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span.push_back(11);
    TRIAL_TEST(span.begin() != span.end());
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void test_iterator_constructible()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span.push_back(11);
    decltype(span)::iterator alpha; // DefaultConstructible
    alpha = span.begin();
    TRIAL_TEST(alpha == span.begin());
    decltype(span)::iterator bravo(alpha); // CopyConstructible
    TRIAL_TEST(bravo == span.begin());
}

void test_const_iterator()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span.push_back(11);
    TRIAL_TEST(span.cbegin() != span.cend());
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.cbegin(), span.cend(),
                          expect.begin(), expect.end());
    }
}

void test_const_iterator_constructible()
{
    // Also checks implicit conversion from iterator to const_iterator
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span.push_back(11);
    decltype(span)::const_iterator alpha; // DefaultConstructible
    alpha = span.begin();
    TRIAL_TEST(alpha == span.begin());
    decltype(span)::const_iterator bravo(alpha); // CopyConstructible
    TRIAL_TEST(bravo == span.begin());
}

void test_push_back()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span.push_back(11);
    TRIAL_TEST(span.begin() != span.end());
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(22);
    {
        std::vector<int> expect = { 11, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(33);
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(44);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(55);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(66);
    {
        std::vector<int> expect = { 33, 44, 55, 66 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void test_push_front()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span.push_front(11);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(22);
    {
        std::vector<int> expect = { 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(33);
    {
        std::vector<int> expect = { 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(44);
    {
        std::vector<int> expect = { 44, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(55);
    {
        std::vector<int> expect = { 55, 44, 33, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(66);
    {
        std::vector<int> expect = { 66, 55, 44, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void test_push_alternating()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span.push_back(11);
    {
        std::vector<int> expect = { 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(22);
    {
        std::vector<int> expect = { 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(33);
    {
        std::vector<int> expect = { 22, 11, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(44);
    {
        std::vector<int> expect = { 44, 22, 11, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_back(55);
    {
        std::vector<int> expect = { 22, 11, 33, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    span.push_front(66);
    {
        std::vector<int> expect = { 66, 22, 11, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    test_empty();
    test_iterator();
    test_iterator_constructible();
    test_const_iterator();
    test_const_iterator_constructible();
    test_push_front();
    test_push_back();
    test_push_alternating();
}

} // namespace iterator_suite

//-----------------------------------------------------------------------------

namespace operator_suite
{

void addition_assignment()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    {
        auto where = span.begin();
        where += 0;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin();
        where += 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin();
        where += 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin();
        where += 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
    span.push_back(55);
    {
        auto where = span.begin();
        where += 0;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin();
        where += 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin();
        where += 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin();
        where += 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
    span.push_back(66);
    {
        auto where = span.begin();
        where += 0;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin();
        where += 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin();
        where += 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin();
        where += 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
    span.push_back(77);
    {
        auto where = span.begin();
        where += 0;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin();
        where += 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin();
        where += 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin();
        where += 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
    span.push_back(88);
    {
        auto where = span.begin();
        where += 0;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin();
        where += 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin();
        where += 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin();
        where += 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
}

void addition()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    {
        auto where = span.begin();
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin() + 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin() + 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin() + 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
    span.push_back(55);
    {
        auto where = span.begin();
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin() + 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin() + 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin() + 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
    span.push_back(66);
    {
        auto where = span.begin();
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin() + 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin() + 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin() + 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
    span.push_back(77);
    {
        auto where = span.begin();
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin() + 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin() + 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin() + 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
    span.push_back(88);
    {
        auto where = span.begin();
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 0);
        where = span.begin() + 1;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 1);
        where = span.begin() + 2;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 2);
        where = span.begin() + 3;
        TRIAL_TEST_EQ(std::distance(span.begin(), where), 3);
    }
}

void subtraction_assignment()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    {
        auto where = span.end();
        where -= 0;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end();
        where -= 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end();
        where -= 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end();
        where -= 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
    span.push_back(55);
    {
        auto where = span.end();
        where -= 0;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end();
        where -= 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end();
        where -= 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end();
        where -= 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
    span.push_back(66);
    {
        auto where = span.end();
        where -= 0;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end();
        where -= 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end();
        where -= 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end();
        where -= 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
    span.push_back(77);
    {
        auto where = span.end();
        where -= 0;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end();
        where -= 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end();
        where -= 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end();
        where -= 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
    span.push_back(88);
    {
        auto where = span.end();
        where -= 0;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end();
        where -= 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end();
        where -= 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end();
        where -= 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
}

void subtraction()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    {
        auto where = span.end();
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end() - 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end() - 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end() - 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
    span.push_back(55);
    {
        auto where = span.end();
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end() - 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end() - 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end() - 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
    span.push_back(66);
    {
        auto where = span.end();
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end() - 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end() - 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end() - 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
    span.push_back(77);
    {
        auto where = span.end();
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end() - 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end() - 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end() - 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
    span.push_back(88);
    {
        auto where = span.end();
        TRIAL_TEST_EQ(std::distance(where, span.end()), 0);
        where = span.end() - 1;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 1);
        where = span.end() - 2;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 2);
        where = span.end() - 3;
        TRIAL_TEST_EQ(std::distance(where, span.end()), 3);
    }
}

void difference_partial()
{
    int array[4] = {};
    circular::span<int> span(array);
    // X X X X
    TRIAL_TEST_EQ(span.end() - span.begin(), 0);
    // 11 X X X
    span.push_back(11);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 11 22 X X
    span.push_back(22);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 11 22 33 X
    span.push_back(33);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 11 22 33 44
    span.push_back(44);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(std::next(span.begin(), 3) - span.begin(), 3);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
}

void difference_0()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55 };
    // 55 22 33 44
    span.push_back(55);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(std::next(span.begin(), 3) - span.begin(), 3);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 55 66 33 44
    span.push_back(66);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(std::next(span.begin(), 3) - span.begin(), 3);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 55 66 77 44
    span.push_back(77);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(std::next(span.begin(), 3) - span.begin(), 3);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 55 66 77 88
    span.push_back(88);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(std::next(span.begin(), 3) - span.begin(), 3);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
}

void difference_1()
{
    int array[4] = {};
    circular::span<int> span(array);
    // X 22 33 44
    span = { 11, 22, 33, 44 };
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 55 X 33 44
    span.push_back(55);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 55 66 X 44
    span.push_back(66);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 55 66 77 X
    span.push_back(77);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // X 66 77 88
    span.push_back(88);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(std::next(span.begin(), 2) - span.begin(), 2);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
}

void difference_2()
{
    int array[4] = {};
    circular::span<int> span(array);
    // X X 33 44
    span = { 11, 22, 33, 44 };
    span.remove_front(2);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 55 X X 44
    span.push_back(55);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 55 66 X X
    span.push_back(66);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // X 66 77 X
    span.push_back(77);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // X X 77 88
    span.push_back(88);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(std::next(span.begin(), 1) - span.begin(), 1);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
}

void difference_3()
{
    int array[4] = {};
    circular::span<int> span(array);
    // X X X 44
    span = { 11, 22, 33, 44 };
    span.remove_front(3);
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // 55 X X X
    span.push_back(55);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // X 66 X X
    span.push_back(66);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // X X 77 X
    span.push_back(77);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
    // X X X 88
    span.push_back(88);
    span.remove_front();
    TRIAL_TEST_EQ(span.begin() - span.begin(), 0);
    TRIAL_TEST_EQ(span.end() - span.begin(), span.size());
}

void index()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST_EQ(span.begin()[0], 11);
    TRIAL_TEST_EQ(span.begin()[1], 22);
    TRIAL_TEST_EQ(span.begin()[2], 33);
    TRIAL_TEST_EQ(span.begin()[3], 44);

    span.push_back(55);
    TRIAL_TEST_EQ(span.begin()[0], 22);
    TRIAL_TEST_EQ(span.begin()[1], 33);
    TRIAL_TEST_EQ(span.begin()[2], 44);
    TRIAL_TEST_EQ(span.begin()[3], 55);

    span.push_back(66);
    TRIAL_TEST_EQ(span.begin()[0], 33);
    TRIAL_TEST_EQ(span.begin()[1], 44);
    TRIAL_TEST_EQ(span.begin()[2], 55);
    TRIAL_TEST_EQ(span.begin()[3], 66);

    span.push_back(77);
    TRIAL_TEST_EQ(span.begin()[0], 44);
    TRIAL_TEST_EQ(span.begin()[1], 55);
    TRIAL_TEST_EQ(span.begin()[2], 66);
    TRIAL_TEST_EQ(span.begin()[3], 77);

    span.push_back(88);
    TRIAL_TEST_EQ(span.begin()[0], 55);
    TRIAL_TEST_EQ(span.begin()[1], 66);
    TRIAL_TEST_EQ(span.begin()[2], 77);
    TRIAL_TEST_EQ(span.begin()[3], 88);
}

void less()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST(!(span.begin() < span.begin()));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() < span.begin()));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 4)));

    span.push_back(55);
    TRIAL_TEST(!(span.begin() < span.begin()));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() < span.begin()));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 4)));

    span.push_back(66);
    TRIAL_TEST(!(span.begin() < span.begin()));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() < span.begin()));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 4)));

    span.push_back(77);
    TRIAL_TEST(!(span.begin() < span.begin()));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() < span.begin()));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 4)));

    span.push_back(88);
    TRIAL_TEST(!(span.begin() < span.begin()));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() < std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() < span.begin()));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.end() < std::next(span.begin(), 4)));
}

void less_equal()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST(span.begin() <= span.begin());
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() <= span.begin()));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 3)));
    TRIAL_TEST(span.end() <= std::next(span.begin(), 4));

    span.push_back(55);
    TRIAL_TEST(span.begin() <= span.begin());
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() <= span.begin()));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 3)));
    TRIAL_TEST(span.end() <= std::next(span.begin(), 4));

    span.push_back(66);
    TRIAL_TEST(span.begin() <= span.begin());
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() <= span.begin()));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 3)));
    TRIAL_TEST(span.end() <= std::next(span.begin(), 4));

    span.push_back(77);
    TRIAL_TEST(span.begin() <= span.begin());
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() <= span.begin()));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 3)));
    TRIAL_TEST(span.end() <= std::next(span.begin(), 4));

    span.push_back(88);
    TRIAL_TEST(span.begin() <= span.begin());
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 1));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 2));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 3));
    TRIAL_TEST(span.begin() <= std::next(span.begin(), 4));

    TRIAL_TEST(!(span.end() <= span.begin()));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.end() <= std::next(span.begin(), 3)));
    TRIAL_TEST(span.end() <= std::next(span.begin(), 4));
}

void greater()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST(!(span.begin() > span.begin()));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() > span.begin());
    TRIAL_TEST(span.end() > std::next(span.begin(), 1));
    TRIAL_TEST(span.end() > std::next(span.begin(), 2));
    TRIAL_TEST(span.end() > std::next(span.begin(), 3));
    TRIAL_TEST(!(span.end() > std::next(span.begin(), 4)));

    span.push_back(55);
    TRIAL_TEST(!(span.begin() > span.begin()));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() > span.begin());
    TRIAL_TEST(span.end() > std::next(span.begin(), 1));
    TRIAL_TEST(span.end() > std::next(span.begin(), 2));
    TRIAL_TEST(span.end() > std::next(span.begin(), 3));
    TRIAL_TEST(!(span.end() > std::next(span.begin(), 4)));

    span.push_back(66);
    TRIAL_TEST(!(span.begin() > span.begin()));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() > span.begin());
    TRIAL_TEST(span.end() > std::next(span.begin(), 1));
    TRIAL_TEST(span.end() > std::next(span.begin(), 2));
    TRIAL_TEST(span.end() > std::next(span.begin(), 3));
    TRIAL_TEST(!(span.end() > std::next(span.begin(), 4)));

    span.push_back(77);
    TRIAL_TEST(!(span.begin() > span.begin()));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() > span.begin());
    TRIAL_TEST(span.end() > std::next(span.begin(), 1));
    TRIAL_TEST(span.end() > std::next(span.begin(), 2));
    TRIAL_TEST(span.end() > std::next(span.begin(), 3));
    TRIAL_TEST(!(span.end() > std::next(span.begin(), 4)));

    span.push_back(88);
    TRIAL_TEST(!(span.begin() > span.begin()));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() > std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() > span.begin());
    TRIAL_TEST(span.end() > std::next(span.begin(), 1));
    TRIAL_TEST(span.end() > std::next(span.begin(), 2));
    TRIAL_TEST(span.end() > std::next(span.begin(), 3));
    TRIAL_TEST(!(span.end() > std::next(span.begin(), 4)));
}

void greater_equal()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    TRIAL_TEST(span.begin() >= span.begin());
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() >= span.begin());
    TRIAL_TEST(span.end() >= std::next(span.begin(), 1));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 2));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 3));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 4));

    span.push_back(55);
    TRIAL_TEST(span.begin() >= span.begin());
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() >= span.begin());
    TRIAL_TEST(span.end() >= std::next(span.begin(), 1));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 2));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 3));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 4));

    span.push_back(66);
    TRIAL_TEST(span.begin() >= span.begin());
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() >= span.begin());
    TRIAL_TEST(span.end() >= std::next(span.begin(), 1));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 2));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 3));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 4));

    span.push_back(77);
    TRIAL_TEST(span.begin() >= span.begin());
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() >= span.begin());
    TRIAL_TEST(span.end() >= std::next(span.begin(), 1));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 2));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 3));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 4));

    span.push_back(88);
    TRIAL_TEST(span.begin() >= span.begin());
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 1)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 2)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 3)));
    TRIAL_TEST(!(span.begin() >= std::next(span.begin(), 4)));

    TRIAL_TEST(span.end() >= span.begin());
    TRIAL_TEST(span.end() >= std::next(span.begin(), 1));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 2));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 3));
    TRIAL_TEST(span.end() >= std::next(span.begin(), 4));
}

void run()
{
    addition_assignment();
    addition();
    subtraction_assignment();
    subtraction();
    difference_partial();
    difference_0();
    difference_1();
    difference_2();
    difference_3();
    index();
    less();
    less_equal();
    greater();
    greater_equal();
}

} // namespace operator_suite

//-----------------------------------------------------------------------------

namespace reverse_suite
{

void test_empty()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    TRIAL_TEST(span.rbegin() == span.rend());
}

void test_reverse()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span = { 11, 22, 33 };
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.rbegin(), span.rend(),
                          expect.begin(), expect.end());
    }
    span.push_back(44);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 44, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.rbegin(), span.rend(),
                          expect.begin(), expect.end());
    }
    span.push_back(55);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 55, 44, 33, 22 };
        TRIAL_TEST_ALL_EQ(span.rbegin(), span.rend(),
                          expect.begin(), expect.end());
    }
}

void test_const_reverse()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span = { 11, 22, 33 };
    {
        std::vector<int> expect = { 11, 22, 33 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.crbegin(), span.crend(),
                          expect.begin(), expect.end());
    }
    span.push_back(44);
    {
        std::vector<int> expect = { 11, 22, 33, 44 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 44, 33, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.crbegin(), span.crend(),
                          expect.begin(), expect.end());
    }
    span.push_back(55);
    {
        std::vector<int> expect = { 22, 33, 44, 55 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
    {
        std::vector<int> expect = { 55, 44, 33, 22 };
        TRIAL_TEST_ALL_EQ(span.crbegin(), span.crend(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    test_empty();
    test_reverse();
    test_const_reverse();
}

} // namespace reverse_suite

//-----------------------------------------------------------------------------

namespace inserter_suite
{

void test_back_inserter()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span = { 11 };
    std::fill_n(std::back_inserter(span), 3, 22);
    {
        std::vector<int> expect = { 11, 22, 22, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }

    span = { 11 };
    std::fill_n(std::back_inserter(span), 4, 22);
    {
        std::vector<int> expect = { 22, 22, 22, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void test_front_inserter()
{
    std::array<int, 4> array = {};
    circular::span<int> span(array.begin(), array.end());
    span = { 11 };
    std::fill_n(std::front_inserter(span), 3, 22);
    {
        std::vector<int> expect = { 22, 22, 22, 11 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }

    span = { 11 };
    std::fill_n(std::front_inserter(span), 4, 22);
    {
        std::vector<int> expect = { 22, 22, 22, 22 };
        TRIAL_TEST_ALL_EQ(span.begin(), span.end(),
                          expect.begin(), expect.end());
    }
}

void run()
{
    test_back_inserter();
    test_front_inserter();
}

} // namespace inserter_suite

//-----------------------------------------------------------------------------

namespace range_for_suite
{

void empty()
{
    int array[4] = {};
    circular::span<int> span(array);
    int sum = 0;
    for (auto&& element : span)
    {
        sum += element;
    }
    TRIAL_TEST_EQ(sum, 0);
}

void full()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    int sum = 0;
    for (auto&& element : span)
    {
        sum += element;
    }
    TRIAL_TEST_EQ(sum, 11 + 22 + 33 + 44);
}

void overfull()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44, 55 };
    int sum = 0;
    for (auto&& element : span)
    {
        sum += element;
    }
    TRIAL_TEST_EQ(sum, 22 + 33 + 44 + 55);
}

void segment_empty()
{
    int array[4] = {};
    circular::span<int> span(array);
    int sum = 0;
    for (auto&& element : span.first_segment())
    {
        sum += element;
    }
    TRIAL_TEST_EQ(sum, 0);
    for (auto&& element : span.last_segment())
    {
        sum += element;
    }
    TRIAL_TEST_EQ(sum, 0);
}

void segment_full()
{
    int array[4] = {};
    circular::span<int> span(array);
    span = { 11, 22, 33, 44 };
    int sum = 0;
    for (auto&& element : span.first_segment())
    {
        sum += element;
    }
    TRIAL_TEST_EQ(sum, 11 + 22 + 33 + 44);
    for (auto&& element : span.last_segment())
    {
        sum += element;
    }
    TRIAL_TEST_EQ(sum, 11 + 22 + 33 + 44);
}

void segment_overfull()
{
    int array[4] = {};
    circular::span<int> span(array);
    // 55 22 33 44
    // -> <-------
    span = { 11, 22, 33, 44, 55 };
    int sum = 0;
    for (auto&& element : span.first_segment())
    {
        sum += element;
    }
    TRIAL_TEST_EQ(sum, 22 + 33 + 44);
    for (auto&& element : span.last_segment())
    {
        sum += element;
    }
    TRIAL_TEST_EQ(sum, 22 + 33 + 44 + 55);
}

void run()
{
    empty();
    full();
    overfull();
    segment_empty();
    segment_full();
    segment_overfull();
}

} // namespace range_for_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    concept_suite::run();
    concept_const_suite::run();
    iterator_suite::run();
    operator_suite::run();
    reverse_suite::run();
    inserter_suite::run();
    range_for_suite::run();

    return boost::report_errors();
}
