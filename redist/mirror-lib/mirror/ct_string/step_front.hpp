/**
 * @file mirror/ct_string/step_front.hpp
 * @brief Implementation of the step_front meta-function
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CT_STRING_STEP_FRONT_1105240825_HPP
#define MIRROR_CT_STRING_STEP_FRONT_1105240825_HPP

#include <mirror/ct_string/default.hpp>

MIRROR_NAMESPACE_BEGIN
namespace cts {

#ifndef MIRROR_DOCUMENTATION_ONLY

template <typename Char, Char C, Char ... Cn>
struct step_front<basic_string<Char, C, Cn...> >
{
	typedef basic_string<Char, Cn...> type;
};

template <typename Char>
struct step_front<basic_string<Char> >
{
	typedef basic_string<Char> type;
};

#endif

} // namespace cts
MIRROR_NAMESPACE_END

#endif //include guard

