/**
 * .file mirror/utils/lex_match/std_tm.hpp
 * .brief default implementation of the lexical matcher
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_LEX_MATCH_STD_TM_1011291729_HPP
#define MIRROR_UTILS_LEX_MATCH_STD_TM_1011291729_HPP

MIRROR_NAMESPACE_BEGIN

namespace aux {

template <>
struct _lex_match<std::tm>
{
	template <typename Iterator>
	double match(Iterator& begin, Iterator end) const
	{
		// TODO:
		begin = end;
		return 1.0;
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

