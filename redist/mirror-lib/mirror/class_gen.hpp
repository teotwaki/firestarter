/**
 * @file mirror/class_gen.hpp
 * @brief Implementation of the class generator utility
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_CLASS_GEN_1103092151_HPP
#define MIRROR_CLASS_GEN_1103092151_HPP

#include <mirror/mirror_fwd.hpp>
#include <mirror/auxiliary/class_gen.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_class.hpp>

MIRROR_NAMESPACE_BEGIN

template <
	class MetaScope,
	template <class> class VarRangeGetter,
	template <class> class ScopeTransform,
	template <class> class VarTransform
>
struct scope_class_generator
{
	typedef aux::class_gen_hlpr<
		MetaScope,
		typename VarRangeGetter<MetaScope>::type,
		ScopeTransform,
		VarTransform
	> type;
};

template <
	class Class,
	template <class> class ClassTransform,
	template <class> class MemVarTransform
>
struct class_generator
{
	typedef aux::class_gen_hlpr<
		meta_class<Class>,
		typename all_member_variables<meta_class<Class> >::type,
		ClassTransform,
		MemVarTransform
	> type;
};

template <
	template <class> class ClassTransform,
	template <class> class MemVarTransform
>
struct generator_maker
{
	template <class Class>
	struct generator
	{
		typedef typename class_generator<
			Class,
			ClassTransform,
			MemVarTransform
		>::type type;
	};
};

MIRROR_NAMESPACE_END

#endif //include guard

