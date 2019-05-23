#ifndef TRIAL_CIRCULAR_DETAIL_CONFIG_HPP
#define TRIAL_CIRCULAR_DETAIL_CONFIG_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2019 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if __cplusplus >= 201402L
# define TRIAL_CIRCULAR_CXX14(x) x
#else
# define TRIAL_CIRCULAR_CXX14(x)
#endif

#define TRIAL_CXX14_CONSTEXPR TRIAL_CIRCULAR_CXX14(constexpr)

#endif // TRIAL_CIRCULAR_DETAIL_CONFIG_HPP
