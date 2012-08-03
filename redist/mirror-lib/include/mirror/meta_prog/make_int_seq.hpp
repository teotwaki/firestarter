/**
 * @file mirror/meta_prog/make_int_seq.hpp
 * @brief meta-function template class returning a sequence
 * of integral constant types from 0 to N-1.
 *
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_MAKE_INT_SEQ_1011291729_HPP
#define MIRROR_META_PROG_MAKE_INT_SEQ_1011291729_HPP

#include <mirror/meta_prog/apply_on_seq_pack.hpp>
#include <mirror/meta_prog/range.hpp>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace mp {

/// Meta-function returning a range of integral constants from 0 to N-1.
/**
 *  @tparam Count the count of elements in the range
 *  @ingroup meta_programming
 */
template <typename Int, typename Count>
struct make_int_seq
{
private:
	struct helper
	{
		template <unsigned ... P>
		struct apply
		{
			typedef range<
				std::integral_constant<Int, P>...
			> type;
		};
	};
public:
	typedef typename apply_on_seq_pack<
		helper,
		Count
	>::type type;
};

template <typename Count>
struct make_index_seq
 : make_int_seq<int, Count>
{ };

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

