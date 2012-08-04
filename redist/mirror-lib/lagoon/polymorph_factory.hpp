/**
 * @file lagoon/polymorph_factory.hpp
 * @brief Definitions of the interfaces for the polymorphic factories
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_POLYMORPH_FACTORY_1011291729_HPP
#define LAGOON_POLYMORPH_FACTORY_1011291729_HPP

#include <lagoon/lagoon_fwd.hpp>
#include <lagoon/utils.hpp>
#include <lagoon/auxiliary/range_maker.hpp>
#include <mirror/utils/polymorph_factory.hpp>

#if LAGOON_FACT_WITH_CREATE
#include <boost/any.hpp>
#endif

#include <vector>

LAGOON_NAMESPACE_BEGIN

/** @defgroup lagoon_polymorphic_factories Lagoon - Polymorphic factories
 *
 *  Similar to the compile-time factory generator in Mirror, Lagoon
 *  implements a polymorphic framework for generating factory classes
 *  which can get input data from an external representation (XML, JSON,
 *  releational database dataset, a GUI, etc.), pick an appropriate constructor
 *  and create a new instance of a particular type or class.
 */


// TODO: add a compile time switch between this and a real range
// of meta-objects representing the context of the factory
struct polymorph_factory_context
{
private:
	// the meta-object interface used by the context
	typedef meta_named_scoped_object mo_interface;
	// holder of the composite of meta-objects in the context
	struct item_list_holder
	{
		// the composite of context meta-object
		std::vector<shared<mo_interface> > item_list;

		template <class Context>
		item_list_holder(Context)
		{
			mirror::mp::for_each<Context>
			(aux::range_maker<mo_interface>(item_list));
		}
	} holder;
public:
	template <class Context>
	polymorph_factory_context(Context context)
	 : holder(context)
	{ }

	inline range<mo_interface> all(void) const
	{
		return range<mo_interface>(
			holder.item_list.begin(),
			holder.item_list.end()
		);
	}
};

/// Interface for a polymorphic factory manager
/** The manager is responsible for choosing and providing the index
 *  of the constructor that is to be used for the construction
 *  of an instance of product type.
 *
 *  @ingroup lagoon_polymorphic_factories
 */
struct polymorph_factory_manager
{
	virtual ~polymorph_factory_manager(void)
	{ }

	virtual raw_ptr data(void)
	{
		return raw_ptr();
	}

	virtual raw_ptr add_constructor(
		raw_ptr data,
		const shared<meta_constructor>& constructor,
		const polymorph_factory_context& context,
		bool backward_iteration
	)
	{
		return raw_ptr();
	}

	virtual void finish(raw_ptr data)
	{ }

	virtual int index(void) = 0;
};

/// Interface for a polymorphic factory composite or node
/** The composite is responsible for containing other
 *  components of a non-trivial factory.
 *
 *  @ingroup lagoon_polymorphic_factories
 */
struct polymorph_factory_composite
{
	virtual ~polymorph_factory_composite(void)
	{ }

	virtual raw_ptr data(void)
	{
		return raw_ptr();
	}

	virtual void finish(raw_ptr data)
	{ }

	virtual void on_create(void)
	{ }
};

/// Interface for a polymorphic factory element arrayer
/** The arrayer is responsible for creating sequences of
 *  elements when constructing containers.
 *
 *  @ingroup lagoon_polymorphic_factories
 */
struct polymorph_factory_arrayer
{
	virtual ~polymorph_factory_arrayer(void)
	{ }

	virtual raw_ptr data(void)
	{
		return raw_ptr();
	}

	virtual void finish(raw_ptr data)
	{ }

	/// Implementations are responsible for creating and storing elements
	struct element_producer
	{
		virtual ~element_producer(void)
		{ }

		/// Resets the producer, removed previous elements if any
		virtual void reset(void) = 0;

		/// Make and store the next element
		virtual void make_next(void) = 0;
	};

	/// Assigns an element producer for this arrayer
	virtual void assign_producer(element_producer& producer)
	{ }

	/// Creates the elements and puts them into a container
	/** The container used by this function must be
	 *  a std::vector<Element>
	 */
	virtual void create(element_producer& producer) = 0;
};

/// Interface for a polymorphic factory manufacturer or leaf
/** The manufacturer is responsible for creating instances
 *  of product and returning them to the caller which
 *  can use them as the results of the construction or just
 *  as parameters for other construction.
 *
 *  @ingroup lagoon_polymorphic_factories
 */
template <typename Product>
struct polymorph_factory_manufacturer
{
	virtual ~polymorph_factory_manufacturer(void)
	{ }

	virtual void finish(raw_ptr parent_data)
	{ }

	virtual Product create(void) = 0;
};

/// Interface for polymorphic factory suppliers
/** Suppliers are responsible for returning existing instances
 *  of a type that can be uses as the source for copy construction.
 *
 *  @ingroup lagoon_polymorphic_factories
 */
struct polymorph_factory_suppliers
{
	virtual ~polymorph_factory_suppliers(void)
	{ }

	virtual void finish(raw_ptr parent_data)
	{ }

	/// Returns a pointer to an existing object
	/** The pointed-to object is managed by the suppliers.
	 */
	virtual raw_ptr get(void) = 0;
};

/// Interface for a polymorphic factory enumerator leaf node
/** The enumerator is responsible for getting the values
 *  of enum types and returning them to the caller which
 *  can use them as the results of the construction or just
 *  as parameters for other construction.
 *
 *  @ingroup lagoon_polymorphic_factories
 */
struct polymorph_factory_enumerator
{
	virtual ~polymorph_factory_enumerator(void)
	{ }

	virtual void finish(raw_ptr parent_data)
	{ }

	virtual int create(void) = 0;
};

//  Builder's helper subinterface for instantiating manufacturers of a Product
/*  The individual builder units are composed together based on the
 *  native types type range by the means of polymorph_factory_builder_units
 *
 *  @ingroup lagoon_polymorphic_factories
 */
template <typename Product>
class polymorph_factory_builder_unit
{
protected:
	virtual ~polymorph_factory_builder_unit(void)
	{ }
public:
	virtual std::shared_ptr<polymorph_factory_manufacturer<Product> >
	make_manufacturer(
		raw_ptr parent_data,
		const shared<meta_parameter>& ctr_param,
		const polymorph_factory_context& context
	) = 0;
};

//  Forward declaration of the builder unit composer
template <template <class> class Unit, class Range>
struct polymorph_factory_builder_units;

//  Implementation of builder unit composer
template <template <class> class Unit, typename ... P>
class polymorph_factory_builder_units<Unit, mirror::mp::range<P...> >
 : virtual public Unit<P>...
{
private:
	template <class Product>
	Unit<Product>* this_unit(void)
	{
		return this;
	}
public:
	template <typename Product>
	inline std::shared_ptr<polymorph_factory_manufacturer<Product> >
	make_manufacturer(
		raw_ptr parent_data,
		const shared<meta_parameter>& ctr_param,
		const polymorph_factory_context& context,
		mirror::mp::identity<Product>
	)
	{
		return this_unit<Product>()->make_manufacturer(
			parent_data,
			ctr_param,
			context
		);
	}
};

/// Interface for a polymorphic factory builder
/** Builder builds the individual components of a polymorphic
 *  factory (managers, manufacturers, composites and suppliers)
 *  as required by the polymorphic factory framework.
 *
 *  @see polymorph_factory_manager
 *  @see polymorph_factory_composite
 *  @see polymorph_factory_arrayer
 *  @see polymorph_factory_manufacturer
 *  @see polymorph_factory_suppliers
 *
 *  @ingroup lagoon_polymorphic_factories
 */
struct polymorph_factory_builder
 : public polymorph_factory_builder_units<
	polymorph_factory_builder_unit,
	mirror::util_native_types
>
{
	virtual ~polymorph_factory_builder(void)
	{ }

	virtual std::shared_ptr<polymorph_factory_manager>
	make_manager(
		raw_ptr parent_data,
		const polymorph_factory_context& context
	) = 0;

	virtual std::shared_ptr<polymorph_factory_composite>
	make_composite(
		raw_ptr parent_data,
		const shared<meta_parameter>& func_param,
		const polymorph_factory_context& context
	) = 0;

	virtual std::shared_ptr<polymorph_factory_composite>
	make_composite(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	) = 0;

	virtual std::shared_ptr<polymorph_factory_arrayer>
	make_arrayer(
		raw_ptr parent_data,
		const shared<meta_type>& element,
		const polymorph_factory_context& context
	) = 0;

	virtual std::shared_ptr<polymorph_factory_suppliers>
	make_suppliers(
		raw_ptr parent_data,
		const shared<meta_parameter>& ctr_param,
		const polymorph_factory_context& context
	) = 0;

	virtual std::shared_ptr<polymorph_factory_enumerator>
	make_enumerator(
		raw_ptr parent_data,
		const shared<meta_parameter>& ctr_param,
		const polymorph_factory_context& context
	) = 0;
};

template <
	template <class> class ConcreteUnit,
	class Range
> class concrete_polymorph_factory_bldr_hlpr;

template <
	template <class> class ConcreteUnit,
	typename T,
	typename ... P
> class concrete_polymorph_factory_bldr_hlpr<
	ConcreteUnit,
	mirror::mp::range<T, P...>
>: public ConcreteUnit<T>
 , public concrete_polymorph_factory_bldr_hlpr<
	ConcreteUnit,
	mirror::mp::range<P...>
>{ };

template <
	template <class> class ConcreteUnit
> class concrete_polymorph_factory_bldr_hlpr<
	ConcreteUnit,
	mirror::mp::range<>
>: public polymorph_factory_builder
{ };

template <template <class> class BaseUnit>
struct concrete_polymorph_factory_builder_unit
{
public:
	template <class Product>
	class impl
	 : virtual public polymorph_factory_builder_unit<Product>
	 , public BaseUnit<Product>
	{
	private:
		BaseUnit<Product>& base_unit(void)
		{
			return *this;
		}
	public:
		std::shared_ptr<polymorph_factory_manufacturer<Product> >
		make_manufacturer(
			raw_ptr parent_data,
			const shared<meta_parameter>& param,
			const polymorph_factory_context& context
		)
		{
			return std::shared_ptr<
				polymorph_factory_manufacturer<Product>
			>(base_unit().make_manufacturer(
				parent_data,
				param,
				context
			));
		}
	};
};

template <class BaseBuilder, template <class> class BaseUnit>
class concrete_polymorph_factory_builder
 : public BaseBuilder
 , public concrete_polymorph_factory_bldr_hlpr<
	concrete_polymorph_factory_builder_unit<BaseUnit>::template impl,
	mirror::util_native_types
>
{
private:
	BaseBuilder& base_builder(void)
	{
		return *this;
	}
public:
	std::shared_ptr<polymorph_factory_manager>
	make_manager(
		raw_ptr parent_data,
		const polymorph_factory_context& context
	)
	{
		return std::shared_ptr<polymorph_factory_manager>(
			base_builder().make_manager(parent_data, context)
		);
	}

	std::shared_ptr<polymorph_factory_composite>
	make_composite(
		raw_ptr parent_data,
		const shared<meta_type>& product,
		const polymorph_factory_context& context
	)
	{
		return std::shared_ptr<polymorph_factory_composite>(
			base_builder().make_composite(
				parent_data,
				product,
				context
			)
		);
	}

	std::shared_ptr<polymorph_factory_composite>
	make_composite(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	)
	{
		return std::shared_ptr<polymorph_factory_composite>(
			base_builder().make_composite(
				parent_data,
				param,
				context
			)
		);
	}

	std::shared_ptr<polymorph_factory_arrayer>
	make_arrayer(
		raw_ptr parent_data,
		const shared<meta_type>& element,
		const polymorph_factory_context& context
	)
	{
		return std::shared_ptr<polymorph_factory_arrayer>(
			base_builder().make_arrayer(
				parent_data,
				element,
				context
			)
		);
	}

	std::shared_ptr<polymorph_factory_suppliers>
	make_suppliers(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	)
	{
		return std::shared_ptr<polymorph_factory_suppliers>(
			base_builder().make_suppliers(
				parent_data,
				param,
				context
			)
		);
	}

	std::shared_ptr<polymorph_factory_enumerator>
	make_enumerator(
		raw_ptr parent_data,
		const shared<meta_parameter>& param,
		const polymorph_factory_context& context
	)
	{
		return std::shared_ptr<polymorph_factory_enumerator>(
			base_builder().make_enumerator(
				parent_data,
				param,
				context
			)
		);
	}

};

/// Abstract interface for polymorphic factories
/** Implementations of such factories are generated by using
 *  a custom impmlementation of polymorph_factory_builder with
 *  the Mirror's factory generator utility and the polymorphic
 *  factory framework.
 *
 *  @ingroup lagoon_polymorphic_factories
 */
struct polymorph_factory
{
	virtual ~polymorph_factory(void)
	{ }

	/// Creates a new dynamically allocated instance
	/** The created object should be disposed via meta_type::delete_
	 *
	 *  @see create
	 */
	virtual raw_ptr new_(void) = 0;

#if LAGOON_FACT_WITH_CREATE || \
    MIRROR_DOCUMENTATION_ONLY
	/// Creates a new value wrapped in boost::any
	/**
	 *  This member function is available only if the
	 *  #LAGOON_FACT_WITH_CREATE preprocessor symbol
	 *  is set to a nonzero integer value.
	 *
	 *  @see new_
	 */
	virtual boost::any create(void) = 0;
#endif
};

LAGOON_NAMESPACE_END

#endif // include guard

