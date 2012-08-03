/**
 * @file mirror/pre_registered/class/boost/optional.hpp
 * @brief Pre-registration of the boost::optional template class with Mirror
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_PRE_REGISTERED_CLASS_BOOST_OPTIONAL_1101110333_HPP
#define MIRROR_PRE_REGISTERED_CLASS_BOOST_OPTIONAL_1101110333_HPP

#include <mirror/meta_type_template.hpp>
#include <mirror/pre_registered/namespace/boost.hpp>
#include <mirror/meta_prog/range.hpp>
#include <boost/optional/optional.hpp>

MIRROR_REG_BEGIN

MIRROR_REG_CLASS_TEMPLATE_BEGIN(
	(typename Type),
	struct, boost, optional, (Type)
)

MIRROR_REG_CONSTRUCTORS_BEGIN
	MIRROR_REG_CONSTRUCTOR_BEGIN(public, 0)
		MIRROR_REG_CONSTRUCTOR_PARAM(Type, value)
	MIRROR_REG_CONSTRUCTOR_END(0)
	MIRROR_REG_DEFAULT_CONSTRUCTOR(public)
	MIRROR_REG_COPY_CONSTRUCTOR(public)
MIRROR_REG_CONSTRUCTORS_END
MIRROR_REG_CLASS_END

namespace aux {

template <typename Optional, typename Type>
class boost_opt_locator
{
private:
	Optional& opt;
public:
	boost_opt_locator(Optional& _opt)
	 : opt(_opt)
	{ }

	typedef std::false_type safe;

	inline bool dereferencable(void) const
	{
		return bool(opt);
	}

	operator bool (void) const
	{
		return bool(opt);
	}

	bool operator !(void) const
	{
		return !opt;
	}

	Type& get(void) const
	{
		assert(dereferencable());
		return opt.get();
	}
};

template <typename Optional, typename Type>
class boost_opt_trvrsl
{
private:
	Optional& opt;
	bool eoc;
public:
	boost_opt_trvrsl(Optional& _opt)
	 : opt(_opt)
	 , eoc(false)
	{ }

	bool done(void) const
	{
		return eoc || !opt;
	}

	bool empty(void) const
	{
		return done();
	}

	void step_front(void)
	{
		assert(!empty());
		eoc = true;
	}

	boost_opt_locator<Optional, Type> front(void) const
	{
		assert(!empty());
		return boost_opt_locator<Optional, Type>(opt);
	}
};

template <typename Optional>
struct meta_boost_opt_trav;

template <typename Type>
struct meta_boost_opt_trav<boost::optional<Type> >
{
	typedef boost::optional<Type> Optional;
	typedef meta_traversal_tag category;
	typedef mirror::mp::empty_range signature;

	static inline std::string base_name(void)
	{
		return std::string();
	}

	typedef boost_opt_trvrsl<Optional, Type>
		traversal_type;

	static traversal_type start(Optional& opt)
	{
		return traversal_type(opt);
	}

	typedef boost_opt_trvrsl<const Optional, const Type>
		const_traversal_type;

	static const_traversal_type start(const Optional& opt)
	{
		return const_traversal_type(opt);
	}
};

template <class Optional>
struct meta_boost_opt_assign_inserter;

template <class T>
struct meta_boost_opt_assign_inserter<boost::optional<T> >
{
	typedef meta_inserter_tag category;

	static std::string base_name(void)
	{
		return std::string(
			"assign",
			sizeof("assign") - 1
		);
	}

	typedef mirror::mp::range<
		mirror::container_op_value_param
	> signature;

	static void insert(
		boost::optional<T>& opt,
		const T& value
	)
	{
		opt = value;
	}
};

template <class Optional>
struct meta_boost_opt_reset_eraser
{
	typedef meta_eraser_tag category;

	static std::string base_name(void)
	{
		return std::string(
			"reset",
			sizeof("reset") - 1
		);
	}

	typedef mirror::mp::empty_range signature;

	static void erase(Optional& opt)
	{
		opt = boost::none_t();
	}
};


} // namespace aux

namespace _container {

template <class Type>
struct _<boost::optional<Type> >
{
	struct cntnr
	{
		typedef boost::optional<Type> _optional;

		typedef Type elem_type;

		static bool empty(const _optional& opt)
		{
			return !opt;
		}

		static size_t size_hint(const _optional& opt)
		{
			return bool(opt) ? 1 : 0;
		}

		typedef mirror::aux::meta_boost_opt_trav<
			_optional
		> default_traversal;

		typedef mirror::mp::range<
			default_traversal
		> traversals;

		typedef mirror::mp::empty_range locators;

		typedef mirror::mp::range<
			mirror::aux::meta_boost_opt_assign_inserter<
				_optional
			>
		> inserters;

		typedef mirror::mp::range<
			mirror::aux::meta_boost_opt_reset_eraser<
				_optional
			>
		> erasers;
	};
	static cntnr _cntnr(std::integral_constant<int, 0>);
	typedef std::integral_constant<int, 1> cntnr_count;
	typedef optional_container_tag kind;
};

} // namespace container

MIRROR_REG_END

#endif //include guard

