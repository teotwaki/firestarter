/**
 * @file mirror/auxiliary/func_call.hpp
 * @brief Internal implementation of function caller similar to factory generator
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_FUNC_CALL_1011291729_HPP
#define MIRROR_AUX_FUNC_CALL_1011291729_HPP

#include <mirror/factory.hpp>
#include <tuple>
#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace aux {

// forward declaration of the base_fn_caller template
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	class MetaFunction,
	class MetaParamSeq,
	class ParamIndexSeq
> class base_fn_caller;

template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	class MetaFunction,
	typename ... MetaParams,
	typename ... ParamIndices
> class base_fn_caller<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	MetaFunction,
	mp::range<MetaParams ... >,
	mp::range<ParamIndices ... >
>
{
private:
	// adjusts the type before passing it as the parameter
	// to the Menufacturer template
	template <typename Type>
	struct adjust
	{
		typedef typename std::remove_cv<
			typename std::remove_reference<
				Type
			>::type
		>::type type;
	};

	std::tuple<
		factory<
			Manufacturer,
			Suppliers,
			Enumerator,
			SourceTraits,
			typename adjust<
				typename MetaParams::type::original_type
			>::type
		> ...
	> factories;
public:
	// initializing constructor passing its parameter
	// to all the factories
	template <typename Param>
	base_fn_caller(Param param)
	 : factories(
		factory<
			Manufacturer,
			Suppliers,
			Enumerator,
			SourceTraits,
			typename adjust<
				typename MetaParams::type::original_type
			>::type
		>(param, mp::range<MetaFunction, MetaParams>()) ...
	){ }

	template <class Class>
	typename MetaFunction::result_type::original_type call_on(Class& inst)
	{
		return MetaFunction::wrap(inst)(
			std::get<ParamIndices::value>(factories)()...
		);
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

