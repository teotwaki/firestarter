/**
 *  .file lagoon/auxiliary/object_getter.hpp
 *  .brief Switch of the implementations of object_getter
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_AUX_OBJECT_GETTER_1011291729_HPP
#define LAGOON_AUX_OBJECT_GETTER_1011291729_HPP

#include <lagoon/interfaces.hpp>

LAGOON_NAMESPACE_BEGIN
namespace aux {

template <
	typename MetaObject,
	template <class> class Implementation
>
inline Implementation<MetaObject>* inst_tpl_meta_object(void)
{
	static Implementation<MetaObject> mo;
	return &mo;
}

template <
	typename MetaObject,
	class Implementation
>
inline Implementation* inst_meta_object(void)
{
	MetaObject smo;
	static Implementation mo(smo);
	return &mo;
}

template <
	typename MetaObject,
	typename Interface,
	template <class> class Implementation
>
inline shared<Interface> do_get_tpl_meta_object(void)
{
	return shared<Interface>(
		inst_tpl_meta_object<
			MetaObject,
			Implementation
		>()
	);
}

template <
	typename MetaObject,
	typename Interface,
	class Implementation
>
inline shared<Interface> do_get_cls_meta_object(void)
{
	return shared<Interface>(
		inst_meta_object<
			MetaObject,
			Implementation
		>()
	);
}

std::false_type has_wrapper(mirror::meta_object_tag);


#define LAGOON_HLPR_IMPL_GET_META_OBJECT(OBJECT, KIND) \
std::true_type has_wrapper(mirror::meta_## OBJECT ##_tag); \
template <typename MetaObject, typename Interface> \
inline shared<Interface> get_meta_object( \
	mirror::meta_## OBJECT ##_tag \
) \
{ \
	return do_get_## KIND ##_meta_object< \
		MetaObject, \
		Interface, \
		meta_## OBJECT ## _ \
	>(); \
}

LAGOON_HLPR_IMPL_GET_META_OBJECT(namespace, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(global_scope, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(type, tpl)
LAGOON_HLPR_IMPL_GET_META_OBJECT(typedef, tpl)
LAGOON_HLPR_IMPL_GET_META_OBJECT(type_template, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(templated_type, tpl)
LAGOON_HLPR_IMPL_GET_META_OBJECT(class, tpl)
LAGOON_HLPR_IMPL_GET_META_OBJECT(templated_class, tpl)
LAGOON_HLPR_IMPL_GET_META_OBJECT(enum, tpl)
LAGOON_HLPR_IMPL_GET_META_OBJECT(inheritance, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(variable, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(member_variable, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(free_variable, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(plain_free_variable, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(parameter, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(member_function, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(constructor, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(overloaded_functions, cls)
LAGOON_HLPR_IMPL_GET_META_OBJECT(conversion_operator, cls)

#undef LAGOON_HLPR_IMPL_GET_META_OBJECT

std::true_type has_wrapper(mirror::meta_meta_object_tag);
template <typename MetaMetaObject, typename Interface>
inline shared<Interface> get_meta_object(mirror::meta_meta_object_tag)
{
	MetaMetaObject _smmo;
#if LAGOON_POLYMORPIC_MMO
	static meta_meta_object_ mmo(_smmo);
#else
	static meta_meta_object mmo(_smmo);
#endif
	return shared<Interface>(&mmo);
}

// This version of the get function calls the proper overload for MetaObject
template <typename MetaObject, typename Interface>
inline shared<Interface> get_meta_object(void)
{
	// use Mirror to categorize the compile-time meta-object
	// and call the proper overload of the get function
	// to create and wrap an instance of the proper
	// run-time meta-object
	return get_meta_object<
		MetaObject,
		Interface
	>(mirror::categorize_meta_object(MetaObject()));
}

} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

