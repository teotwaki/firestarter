/**
 * @file mirror/auxiliary/class_gen.hpp
 * @brief Implementation of the class generator utility
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUXILIARY_CLASS_GEN_1103092148_HPP
#define MIRROR_AUXILIARY_CLASS_GEN_1103092148_HPP

#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN
namespace aux {

template <
	class MetaScope,
	class MetaVars,
	template <class> class ScopeTransform,
	template <class> class MemVarTransform
>
struct class_gen_hlpr;

template <
	class MetaScope,
	class ... MetaVars,
	template <class> class ScopeTransform,
	template <class> class MemVarTransform
>
class class_gen_hlpr<
	MetaScope,
	mp::range<MetaVars...>,
	ScopeTransform,
	MemVarTransform
>: protected ScopeTransform<MetaScope>::type
 , public MetaVars::template _by_name_val<
	typename MemVarTransform<MetaVars>::type
>::type ...
{
private:
	typename ScopeTransform<MetaScope>::type& base(void)
	{
		return *this;
	}
public:

	template <typename Param>
	class_gen_hlpr(Param param)
	 : ScopeTransform<MetaScope>::type(param)
	 , MetaVars::template _by_name_val<
		typename MemVarTransform<MetaVars>::type
	>::type(base(), param) ...
	{ }
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

