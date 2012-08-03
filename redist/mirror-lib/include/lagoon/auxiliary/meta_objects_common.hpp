/**
 * .file lagoon/auxiliary/meta_objects_common.hpp
 * .brief Common implementation details of the meta-object interfaces
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_AUX_META_OBJECTS_COMMON_1011291729_HPP
#define LAGOON_AUX_META_OBJECTS_COMMON_1011291729_HPP

#include <mirror/meta_object_tags.hpp>
#include <mirror/meta_type.hpp>
#include <mirror/type_traits.hpp>
#include <lagoon/interfaces.hpp>
#include <lagoon/utils.hpp>

#if LAGOON_MT_WITH_MAKE_FACTORY
#include <lagoon/auxiliary/polymorph_factory.hpp>
#endif

LAGOON_NAMESPACE_BEGIN
namespace aux {

template <typename MetaMetaObject, typename Interface>
shared<Interface> get_meta_object(mirror::meta_meta_object_tag);

template <typename MetaType>
class meta_type_base_utils
{
protected:
	typedef typename MetaType::original_type original_type;

	// default-constructibility trait
	typedef typename mirror::is_default_constructible<
		original_type
	>::type _is_default_constructible;

	// copy-constructibility trait
	typedef typename mirror::is_copy_constructible<
		original_type
	>::type _is_copy_constructible;

	// make a new instance if the type has a default constructor
	static raw_ptr make_new(std::true_type)
	{
		original_type* tmp = new original_type();
		return raw_ptr(tmp);
	}

	// implementation of make_new for types without
	// a default constructor
	static inline raw_ptr make_new(std::false_type)
	{
		return raw_ptr();
	}

	// make a new copied instance if the type has a copy constructor
	static raw_ptr make_new_copy(
		raw_ptr source,
		std::true_type
	)
	{
		// check the source
		assert(bool(source));
		// cast and dereference the source pointer
		// and make a new instance using the copy
		// constructor
		original_type* tmp = new original_type(
			*(mirror::raw_cast<
				const original_type*
			>(source))
		);
		return raw_ptr(tmp);
	}

	// implementation of make_new for types without
	// a copy constructor
	static inline raw_ptr make_new_copy(raw_ptr, std::false_type)
	{
		return raw_ptr();
	}

#if LAGOON_MT_WITH_DEFAULT
	// make a new instance if the type has a default constructor
	static inline boost::any make_default(std::true_type)
	{
		return boost::any(original_type());
	}

	// implementation of make_default for types without
	// a default constructor
	static inline boost::any make_default(std::false_type)
	{
		return boost::any();
	}
#endif

#if LAGOON_MT_WITH_DELETE
	static inline void delete_(raw_ptr ptr)
	{
		do_delete(
			ptr,
			typename std::is_void<original_type>::type()
		);
	}

	// implementation of do_delete for non-void*
	static void do_delete(raw_ptr ptr, std::false_type)
	{
		if(bool(ptr))
		{
			delete mirror::raw_cast<
				original_type*
			>(ptr);
		}
	}

	// implementation of do_delete for void*
	static void do_delete(raw_ptr ptr, std::true_type)
	{
		assert(!ptr);
	}
#endif

#if LAGOON_MT_WITH_MAKE_SHARED
	static inline shared_raw_ptr make_shared(raw_ptr ptr)
	{
		return do_make_shared(
			ptr,
			typename std::is_void<original_type>::type()
		);
	}

	// implementation of do_make_shared for non-void*
	static shared_raw_ptr do_make_shared(raw_ptr ptr, std::false_type)
	{
		return shared_raw_ptr(
			std::shared_ptr<original_type>(
				mirror::raw_cast<original_type*>(ptr)
			)
		);
	}

	// implementation of do_make_shared for void*
	static shared_raw_ptr do_make_shared(raw_ptr ptr, std::true_type)
	{
		assert(!ptr);
		return shared_raw_ptr();
	}
#endif

#if LAGOON_MT_WITH_MAKE_UNIQUE
	static inline unique_raw_ptr make_unique(raw_ptr ptr)
	{
		return std::move(do_make_unique(
			ptr,
			typename std::is_void<original_type>::type()
		));
	}

	// implementation of do_make_unique for non-void*
	static unique_raw_ptr do_make_unique(raw_ptr ptr, std::false_type)
	{
		std::unique_ptr<original_type> tmp(
			mirror::raw_cast<original_type*>(ptr)
		);
		return std::move(unique_raw_ptr(std::move(tmp)));
	}

	// implementation of do_make_unique for void*
	static unique_raw_ptr do_make_unique(raw_ptr ptr, std::true_type)
	{
		assert(!ptr);
		return std::move(unique_raw_ptr());
	}
#endif

	template <typename T>
	static shared<meta_type> wrap(void)
	{
		return get_meta_object<
			MIRRORED_TYPE(T),
			meta_type
		>();
	}

	template <template <typename T> class Transform>
	static shared<meta_type> apply_and_wrap(void)
	{
		return wrap<
			typename Transform<
				original_type
			>::type
		>();
	}

#if LAGOON_MT_WITH_MAKE_FACTORY
	std::unique_ptr<polymorph_factory> make_factory(
		polymorph_factory_builder& builder,
		raw_ptr build_data
	)
	{
		return std::unique_ptr<polymorph_factory>(
			new polymorph_factory_<original_type>(
				builder,
				build_data
			)
		);
	}
#endif
};

} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

