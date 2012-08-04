/**
 * @file mirror/auxiliary/factory.hpp
 * @brief Internal implementation of factory generators
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_AUX_FACTORY_1011291729_HPP
#define MIRROR_AUX_FACTORY_1011291729_HPP

#include <mirror/auxiliary/constructors.hpp>
#include <mirror/meta_prog/make_int_seq.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/constructor_tags.hpp>
#include <tuple>
#include <type_traits>
#include <cassert>

MIRROR_NAMESPACE_BEGIN

namespace aux {

// helper for the picking which template use as the source
// of the parameter value. see the specializations below
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Param,
	class IsSameOrBaseOf,
	class TypeSpecTag
> struct pick_source_helper;

// if the parameter is not the product then use
// the manufacturer template
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Param,
	class TypeSpecTag
>
struct pick_source_helper<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	Param,
	std::false_type,
	TypeSpecTag
>
{
	typedef Manufacturer<Param, SourceTraits> type;
};

template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Param
>
struct pick_source_helper<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	Param,
	std::false_type,
	mirror::spec_enum_tag
>
{
	typedef Enumerator<Param, SourceTraits> type;
};

// if the parameter is the product or is derived from the product
// then use the suppliers because such constructor needs an existing
// instance
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Param,
	class TypeSpecTag
>
struct pick_source_helper<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	Param,
	std::true_type,
	TypeSpecTag
>
{
	typedef Suppliers<Param, SourceTraits> type;
};

// Helper template class for picking the parameter value source type
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Product,
	typename Param
>
struct pick_source : pick_source_helper<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	Param,
	std::integral_constant<
		bool,
		::std::is_same<Param, Product>::value ||
		::std::is_base_of<Param, Product>::value
	>,
	typename mirror::_type::_<Param>::kind
>
{ };

class constructor_utils
{
protected:
	// calls the finish function on sources in the right order
	template <typename Sources, typename SourceParam, int I, int N>
	void finish_sources(
		Sources& sources,
		SourceParam src_param,
		std::integral_constant<int, I>,
		std::integral_constant<int, N>
	)
	{
		std::get<I>(sources).finish(src_param);
		finish_sources(
			sources,
			src_param,
			std::integral_constant<int, I+1>(),
			std::integral_constant<int, N>()
		);
	}

	// stops the finishing of the input sources
	template <typename Sources, typename SourceParam, int N>
	void finish_sources(
		Sources& sources,
		SourceParam src_param,
		std::integral_constant<int, N>,
		std::integral_constant<int, N>
	)
	{ }

};

// Forward declaration of the constructor template class
/* Instantiations of this template provide the function call
 * operator for creating instances of the Product type,
 * using the ConstrIndex-th constructor.
 */
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Product,
	class MetaParamSeq,
	class ParamIdxSeq,
	class ConstrIndex,
	class ParamCount,
	class ConstructorKind
> class constructor;

// helper class used as a parameter for the constructor templates
template <typename Param, typename Context>
struct ctr_param_ctx
{
	Param param;

	inline ctr_param_ctx(Param p)
	 : param(p)
	{ }
};

template <typename Param, class Context>
inline ctr_param_ctx<Param, Context> make_ctr_param_ctx(Param param, Context)
{
	return ctr_param_ctx<Param, Context>(param);
}

// helper implementation of the ConstructionInfo concept
template <
	class MetaFunctions,
	class FuncIndex,
	class ParamIndex,
	class Context
> struct construction_info
{
	typedef Context context;
	typedef MetaFunctions functions;
	typedef FuncIndex function_index;
	typedef ParamIndex parameter_index;

	typedef typename mp::at<
		MetaFunctions,
		FuncIndex
	>::type function;

	typedef typename parameters<function>::type parameters;

	typedef typename mp::at<
		parameters,
		ParamIndex
	>::type parameter;
};

// helper implementation of the ConstructorInfo concept
template <
	class MetaClass,
	class ConstructorIndex,
	class EnumerationInfo,
	class Context
> struct constructor_info
{
	typedef Context context;
	typedef MetaClass product;
	typedef ConstructorIndex constructor_index;

	typedef typename mp::at<
		constructors<product>,
		constructor_index
	>::type constructor;

	EnumerationInfo& enumeration_info;

	inline constructor_info(EnumerationInfo& enum_info)
	 : enumeration_info(enum_info)
	{ }
};

// implementation of constructor for constructors with some parameters
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Product,
	int ConstrIndex,
	int ParamCount,
	typename ... MetaParams,
	typename ... ParamIdcs,
	class ConstructorKind
> class constructor<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	Product,
	mp::range<MetaParams ... >,
	mp::range<ParamIdcs ... >,
	std::integral_constant<int, ConstrIndex>,
	std::integral_constant<int, ParamCount>,
	ConstructorKind
> : public constructor_utils
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

	typedef typename adjust<Product>::type product;

	// this tuple contains the source instances
	// for creating and providing the parameter instances
	// used in the constructor call.
	std::tuple<
		typename pick_source<
			Manufacturer,
			Suppliers,
			Enumerator,
			SourceTraits,
			product,
			typename adjust<
				typename MetaParams::
				type::original_type
			>::type
		>::type ...
	> sources;

	// creates a single parameter value source instance
	template <
		class MetaParam,
		typename ParamIndex,
		typename SourceParam,
		class Context
	>
	static inline typename pick_source<
		Manufacturer,
		Suppliers,
		Enumerator,
		SourceTraits,
		product,
		typename adjust<
			typename MetaParam::
			type::original_type
		>::type
	>::type make_src(ParamIndex, SourceParam src_param, Context)
	{
		typedef typename pick_source<
			Manufacturer,
			Suppliers,
			Enumerator,
			SourceTraits,
			product,
			typename adjust<
				typename MetaParam::
				type::original_type
			>::type
		>::type src;
		construction_info<
			constructors<meta_class<product> >,
			std::integral_constant<int, ConstrIndex>,
			ParamIndex,
			typename mp::push_back<Context, MetaParam>::type
		> ctr_info;
		return src(src_param, ctr_info);
	}

	inline product create(mirror::struct_initializer_tag)
	{
		product x = {
			std::get<
				ParamIdcs::value
			>(sources)()...
		};
		return x;
	}

	inline product create(mirror::initializer_list_constructor_tag)
	{
		static_assert(
			mp::size<mp::range<ParamIdcs...> >::value == 1,
			"Initializer list constructors must have 1 argument"
		);
		auto init_list = std::get<0>(sources)();
		// TODO: some tricks can be employed here to do
		// some optimizations
		return product(
			init_list.begin(),
			init_list.end()
		);
	}

	template <typename CtrKind>
	inline product create(CtrKind)
	{
		return product(
			std::get<
				ParamIdcs::value
			>(sources)()...
		);
	}

	inline product* make_new(mirror::struct_initializer_tag kind)
	{
		return new product(create(kind));
	}

	inline product* make_new(mirror::initializer_list_constructor_tag)
	{
		static_assert(
			mp::size<mp::range<ParamIdcs...> >::value == 1,
			"Initializer list constructors must have 1 argument"
		);
		auto init_list = std::get<0>(sources)();
		// TODO: some tricks can be employed here to do
		// some optimizations
		return new product(
			init_list.begin(),
			init_list.end()
		);
	}

	template <typename CtrKind>
	inline product* make_new(CtrKind)
	{
		return new product(
			std::get<
				ParamIdcs::value
			>(sources)()...
		);
	}
public:
	// initializing constructor passing its parameter
	// to all the sources
	template <typename Param, class Context>
	constructor(const ctr_param_ctx<Param, Context>& ctx_param)
	 : sources(
		make_src<MetaParams>(
			ParamIdcs(),
			ctx_param.param,
			Context()
		) ...
	)
	{
		finish_sources(
			sources,
			ctx_param.param,
			std::integral_constant<int, 0>(),
			std::integral_constant<int, sizeof...(ParamIdcs)>()
		);
	}

	// function call operator which uses the sources
	// to get the parameters and passes them to the constructor
	product operator()(void)
	{
		return create(ConstructorKind());
	}

	// function which uses the sources to get
	// the parameters and passes them to the
	// operator new / constructor
	product* new_(void)
	{
		return make_new(ConstructorKind());
	}
};

// implementation of the constructor template for default constructors
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Product,
	int ConstrIndex,
	class ConstructorKind
> class constructor<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	Product,
	mp::range<>,
	mp::range<>,
	std::integral_constant<int, ConstrIndex>,
	std::integral_constant<int, 0>,
	ConstructorKind
>
{
private:
	// adjust the product type
	typedef typename std::remove_cv<
		typename std::remove_reference<
			Product
		>::type
	>::type product;
public:
	// initializing constructor which passes its parameter
	// to the suppliers
	template <typename Param, class Context>
	constructor(const ctr_param_ctx<Param, Context>&)
	{ }

	product operator()(void)
	{
		// use the default constructor to create the product
		return product();
	}

	product* new_(void)
	{
		return new product();
	}
};

// forward declaration of the base_factory template
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Product,
	class MetaConstrSeq,
	class ConstrIdxSeq,
	class IsSpecialized
> class base_factory;

// helper class providing information about the enumeration
// of constructors when calling the add_constructor(...) member function
// the implementations of the Manager can use this information
// to put the constructors into correct order
class constructor_enum_info
{
private:
	int prev_index;
	int current_index;
public:
	constructor_enum_info(void)
	 : prev_index(-1)
	 , current_index(-1)
	{ }

	enum direction_type {unknown, forward, backward};

	constructor_enum_info& update(int index)
	{
		prev_index = current_index;
		current_index = index;
		return *this;
	}

	direction_type direction(void) const
	{
		if(prev_index >= 0)
		{
			if(prev_index < current_index)
				return direction_type::forward;
			else if(prev_index > current_index)
				return direction_type::backward;
		}
		return direction_type::unknown;
	}

	bool going_backward(void) const
	{
		return direction() == direction_type::backward;
	}

	bool going_forward(void) const
	{
		return direction() == direction_type::forward;
	}
};

// implementation of the base factory template
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Product,
	typename ... MetaConstrs,
	typename ... ConstrIdcs
> class base_factory<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	Product,
	mp::range<MetaConstrs ... >,
	mp::range<ConstrIdcs ... >,
	std::false_type
> : public constructor_utils
{
private:
	// This manager is responsible for picking the index
	// of the constructor that is to be used to create
	// an instance in the new_ member function and
	// in the function call operator
	Manufacturer<void, SourceTraits> manager;

	constructor_enum_info enum_info;

	// this tuple contains the constructor instances
	// one of which is used in the factory call.
	std::tuple<
		constructor<
			Manufacturer,
			Suppliers,
			Enumerator,
			SourceTraits,
			Product,
			typename parameters<MetaConstrs>::type,
			typename mp::make_index_seq<
				typename mp::size<
					parameters<MetaConstrs>
				>::type
			>::type,
			ConstrIdcs,
			typename mp::size<
				parameters<MetaConstrs>
			>::type,
			typename MetaConstrs::kind
		> ...
	> constructors;

	// adaptor used in the pick_and_create function to
	// call the proper function on the picked constructor.
	// this version calls the function call operator
	// which in turn creates a new instance
	struct construct
	{
		typedef Product result_type;

		template <typename Constructor>
		inline Product operator()(Constructor& ctr) const
		{
			return ctr();
		}

		static inline const Product& dummy_result(void)
		{
			return *((Product*)nullptr);
		}
	};

	// adaptor used in the pick_and_create function to
	// call the proper function on the picked constructor.
	// this version calls the member function called new_
	// which in turn uses the new operator to dynamically
	// create a new instance
	struct make_new
	{
		typedef Product* result_type;

		template <typename Constructor>
		inline Product* operator()(Constructor& ctr) const
		{
			return ctr.new_();
		}

		static inline Product* dummy_result(void)
		{
			return nullptr;
		}
	};

	// This function uses binary search algorithm to find
	// the constructor with the proper index and uses
	// the adaptor to do the proper thing with the picked
	// constructor
	template <class Adaptor, int Min, int Max>
	inline typename Adaptor::result_type pick_and_create(
		const Adaptor& adapt,
		std::integral_constant<int, Min>,
		std::integral_constant<int, Max>,
		int index
	)
	{
		// find the middle of the searched range
		typedef std::integral_constant<int, (Max + Min) / 2> I;
		// if the middle of the range is the index we are
		// looking for, then we are done, get the constructor
		// and use the adaptor to call the proper function
		if(index == I::value)
			return adapt(std::get<I::value>(constructors));
		// if the index we are looking for is smaller
		// than the middle of the range then do the same
		// thing with the left range
		if(index <  I::value)
			return pick_and_create(
				adapt,
				std::integral_constant<int, Min>(),
				std::integral_constant<int, I::value-1>(),
				index
			);
		// if the index we are looking for is larger
		// than the middle of the range then do the same
		// thing with the right range
		if(index >  I::value)
			return pick_and_create(
				adapt,
				std::integral_constant<int, I::value+1>(),
				std::integral_constant<int, Max>(),
				index
			);
		// this should never happen
		assert(!"Never should get here!");
		return adapt.dummy_result();

	}

	// This is a helper for easier calling of the real
	// implementation of the pick_and_create function
	// This overload chooses the proper range on which
	// to do the initial call
	template <class Adaptor, int N>
	inline typename Adaptor::result_type pick_and_create(
		const Adaptor& adaptor,
		std::integral_constant<int, N>,
		int index
	)
	{
		return pick_and_create(
			adaptor,
			std::integral_constant<int, 0>(),
			std::integral_constant<int, N - 1>(),
			index
		);
	}

	template <class Adaptor>
	inline typename Adaptor::result_type pick_and_create(
		const Adaptor& adapt,
		std::integral_constant<int, 1>,
		int index
	)
	{
		if(index == 0) return adapt(std::get<0>(constructors));
		assert(!"Never should get here!");
		return adapt.dummy_result();
	}

	template <class Adaptor>
	inline typename Adaptor::result_type pick_and_create(
		const Adaptor& adapt,
		std::integral_constant<int, 2>,
		int index
	)
	{
		if(index == 0) return adapt(std::get<0>(constructors));
		if(index == 1) return adapt(std::get<1>(constructors));
		assert(!"Never should get here!");
		return adapt.dummy_result();
	}

	template <class Adaptor>
	inline typename Adaptor::result_type pick_and_create(
		const Adaptor& adapt,
		std::integral_constant<int, 3>,
		int index
	)
	{
		if(index == 0) return adapt(std::get<0>(constructors));
		if(index == 1) return adapt(std::get<1>(constructors));
		if(index == 2) return adapt(std::get<2>(constructors));
		assert(!"Never should get here!");
		return adapt.dummy_result();
	}

	template <class Adaptor>
	inline typename Adaptor::result_type pick_and_create(
		const Adaptor& adapt,
		std::integral_constant<int, 4>,
		int index
	)
	{
		if(index == 0) return adapt(std::get<0>(constructors));
		if(index == 1) return adapt(std::get<1>(constructors));
		if(index == 2) return adapt(std::get<2>(constructors));
		if(index == 3) return adapt(std::get<3>(constructors));
		assert(!"Never should get here!");
		return adapt.dummy_result();
	}
public:
	// initializing constructor passing its parameter
	// to all the sources
	template <typename Param, class Context>
	base_factory(Param param, Context context)
	 : manager(param, context)
	 , constructors(
		make_ctr_param_ctx(
			manager.add_constructor(
				param,
				constructor_info<
					meta_class<Product>,
					ConstrIdcs,
					constructor_enum_info,
					Context
				>(enum_info.update(ConstrIdcs::value))
			),
			context
		) ...
	)
	{
		manager.finish(param);
	}

	Product operator()(void)
	{
		// get the index of the constructor to be used
		// from the manager, pick the constructor and
		// let it create an instance
		construct adaptor;
		return pick_and_create(
			adaptor,
			typename mp::size<
				mp::range<MetaConstrs ... >
			>::type(),
			manager.index()
		);
	}

	Product* new_(void)
	{
		// get the index of the constructor to be used
		// from the manager, pick the constructor and
		// let it create an instance
		make_new adaptor;
		return pick_and_create(
			adaptor,
			typename mp::size<
				mp::range<MetaConstrs ... >
			>::type(),
			manager.index()
		);
	}
};

// implementation of the base factory template
// for types with specialized manufacturers
// typically native types like; int, bool. string, etc.
template <
	template <class, class> class Manufacturer,
	template <class, class> class Suppliers,
	template <class, class> class Enumerator,
	class SourceTraits,
	typename Product,
	typename ... MetaConstrs,
	typename ... ConstrIdcs
> class base_factory<
	Manufacturer,
	Suppliers,
	Enumerator,
	SourceTraits,
	Product,
	mp::range<MetaConstrs ... >,
	mp::range<ConstrIdcs ... >,
	std::true_type
> : public constructor_utils
{
private:
	constructor_enum_info enum_info;

	typename pick_source_helper<
		Manufacturer,
		Suppliers,
		Enumerator,
		SourceTraits,
		Product,
		std::false_type,
		typename mirror::_type::_<Product>::kind
	>::type get;

	template <class Context>
	struct _ctrn_info
	{
		typedef Context context;

		typedef meta_initializer<Product> function;


		typedef typename parameters<function>::type parameters;

		typedef std::integral_constant<int, 0> parameter_index;
		typedef typename mp::at<
			parameters,
			parameter_index
		>::type parameter;
	};
public:
	// initializing constructor passing its parameter
	// to all the sources
	template <typename Param, class Context>
	base_factory(Param param, Context)
	 : get(param, _ctrn_info<Context>())
	{
		get.finish(param);
	}

	Product operator()(void)
	{
		return get();
	}

	Product* new_(void)
	{
		return new Product(get());
	}
};

} // namespace aux
MIRROR_NAMESPACE_END

#endif //include guard

