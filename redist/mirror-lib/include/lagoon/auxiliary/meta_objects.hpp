/**
 * .file lagoon/auxiliary/meta_objects.hpp
 * .brief Implementation of the meta-object interfaces
 *
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_AUX_META_OBJECTS_1011291729_HPP
#define LAGOON_AUX_META_OBJECTS_1011291729_HPP

#include <lagoon/interfaces.hpp>
#include <lagoon/auxiliary/meta_objects_common.hpp>
#include <lagoon/auxiliary/object_getter.hpp>
#include <lagoon/auxiliary/specifier_getter.hpp>
#include <lagoon/auxiliary/range_maker.hpp>
#include <lagoon/auxiliary/sequence_holder.hpp>

#include <mirror/meta_prog.hpp>
#include <mirror/meta_meta_object.hpp>
#include <mirror/meta_namespace.hpp>
#include <mirror/meta_type.hpp>


LAGOON_NAMESPACE_BEGIN

//  Base implementation of the meta_object interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_object_
 : virtual public meta_object
{
private:
	shared<meta_meta_object> _self;
public:
	template <typename MetaObject>
	inline meta_object_(MetaObject)
	 : _self(
		aux::get_meta_object<
			MIRRORED_META_OBJECT(MetaObject),
			meta_meta_object
		>()
	){ }

	// Returns a meta-meta-object reflecting this meta-object
	shared<meta_meta_object> self(void) const
	{
		return _self;
	}
};

#if LAGOON_POLYMORPHIC_MMO
//  Base implementation of the meta_meta_object interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_meta_object_
 : public meta_meta_object
{
private:
#if LAGOON_MMO_WITH_GENERALIZATIONS
	range<meta_meta_object>(*_get_generalizations)(void);
#endif
public:
	template <typename MetaMetaObject>
	inline meta_meta_object_(MetaMetaObject mmo)
	 : meta_meta_object(mmo)
#if LAGOON_MMO_WITH_GENERALIZATIONS
	 , _get_generalizations(
		&aux::sequence_holder<
			MetaMetaObject,
			meta_meta_object,
			mirror::generalizations
		>::all
	)
#endif
	{ }

#if LAGOON_MMO_WITH_GENERALIZATIONS
	range<meta_meta_object> generalizations(void) const
	{
		return _get_generalizations();
	}
#endif
};
#endif

//  Implementation of the meta_named_object interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_named_object_
 : virtual public meta_named_object
{
private:
	const std::string _base_name;
public:
	template <typename MetaNamedObject>
	inline meta_named_object_(MetaNamedObject)
	 : _base_name(MetaNamedObject::base_name())
	{ }

	// Returns the base name of the named-object
	std::string base_name(void)
	{
		return _base_name;
	}
};

//  Implementation of the meta_named_scoped_object interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_named_scoped_object_
 : virtual public meta_named_scoped_object
{
private:
	const std::string _base_name;
	const std::string _full_name;
#if LAGOON_MNSO_WITH_LOCAL_NAME
	std::string(*_get_local_name)(void);
#endif
	shared<meta_scope>(*_get_scope)(void);
public:
	template <typename MetaNamedScopedObject>
	inline meta_named_scoped_object_(MetaNamedScopedObject)
	 : _base_name(MetaNamedScopedObject::base_name())
	 , _full_name(MetaNamedScopedObject::full_name())
#if LAGOON_MNSO_WITH_LOCAL_NAME
	 , _get_local_name(&MetaNamedScopedObject::local_name)
#endif
	 , _get_scope(&aux::get_meta_object<
		typename MetaNamedScopedObject::scope,
		meta_scope
	>)
	{ }

	// Returns the base name of the named-scoped object
	std::string base_name(void)
	{
		return _base_name;
	}

	// Returns the full name of the named-scoped object
	std::string full_name(void)
	{
		return _full_name;
	}

#if LAGOON_MNSO_WITH_LOCAL_NAME
	std::string local_name(void)
	{
		assert(_get_local_name != nullptr);
		return _get_local_name();
	}
#endif

	// Returns a pointer to a meta_scope reflecting the scope
	shared<meta_scope> scope(void)
	{
		assert(_get_scope != nullptr);
		return _get_scope();
	}
};


//  Implementation of the meta_scope interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_scope_
 : public meta_named_scoped_object_
 , virtual public meta_scope
{
private:
	template <typename MetaScope>
	static range<meta_named_scoped_object> do_get_members(void)
	{
		// use the internal holder to hold the sequence
		// of the members of the scope
		typedef aux::sequence_holder<
			MetaScope,
			meta_named_scoped_object,
			mirror::members
		> sequence;
#if LAGOON_NO_NAMESPACE_MEMBERS
		return sequence::make(
			typename mirror::mp::is_not_a<
				MetaScope,
				mirror::meta_namespace_tag
			>::type()
		);
#else
		return sequence::all();
#endif
	}

	range<meta_named_scoped_object> (*_get_members)(void);
public:
	template <typename MetaScope>
	inline meta_scope_(MetaScope)
	 : meta_named_scoped_object_(MetaScope())
	 , _get_members(&meta_scope_::template do_get_members<MetaScope>)
	{ }

	// returns a range of meta-objects reflecting the members
	range<meta_named_scoped_object> members(void)
	{
		assert(_get_members != nullptr);
		return _get_members();
	}
};

//  Base implementation of the meta_type interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_type_base_base_
 : public meta_object_
 , virtual public meta_type
{
private:
	range<meta_constructor>(*_get_constructors)(void);

	bool _is_default_constructible;
	bool _is_copy_constructible;

	std::size_t _size_of;

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_ALIGNMENT_OF
	std::size_t _alignment_of;
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_ARRAY
	bool _is_array;
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_CONST
	bool _is_const;
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_POINTER
	bool _is_pointer;
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_REFERENCE
	bool _is_reference;
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_VOLATILE
	bool _is_volatile;
#endif

	template <typename MetaType>
	static std::size_t get_size_of(std::true_type)
	{
		return 0;
	}

	template <typename MetaType>
	static std::size_t get_size_of(std::false_type)
	{
		return sizeof(typename MetaType::original_type);
	}

	template <typename MetaType, template <typename T> class Transform>
	static bool apply_and_get(void)
	{
		typedef typename MetaType::original_type original_type;
		return Transform< original_type >::value;
	}
public:
	template <typename MetaType>
	inline meta_type_base_base_(MetaType)
	 : meta_object_(MetaType())
	 , _get_constructors(
		&aux::sequence_holder<
			MetaType,
			meta_constructor,
			mirror::constructors
		>::all
	), _is_default_constructible(
		mirror::is_default_constructible<
			typename MetaType::original_type
		>::value
	), _is_copy_constructible(
		mirror::is_copy_constructible<
			typename MetaType::original_type
		>::value
	), _size_of(
		get_size_of<MetaType>(
			typename std::is_void<
				typename MetaType::original_type
			>::type()
		)
#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_ALIGNMENT_OF
	), _alignment_of(
		std::alignment_of<
			typename MetaType::original_type
		>::value
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_ARRAY
	), _is_array(
		apply_and_get<MetaType, std::is_array>()
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_CONST
	), _is_const(
		apply_and_get<MetaType, std::is_const>()
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_POINTER
	), _is_pointer(
		apply_and_get<MetaType, std::is_pointer>()
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_REFERENCE
	), _is_reference(
		apply_and_get<MetaType, std::is_reference>()
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_VOLATILE
	), _is_volatile(
		apply_and_get<MetaType, std::is_volatile>()
#endif
	)
	{ }

	// returns the range of meta-constructors
	range<meta_constructor> constructors(void)
	{
		assert(_get_constructors != nullptr);
		return _get_constructors();
	}

	// Returns true if the type is default constructible
	bool is_default_constructible(void)
	{
		return _is_default_constructible;
	}

	// Returns true if the type is copy constructible
	bool is_copy_constructible(void)
	{
		return _is_copy_constructible;
	}

	// Returns the size of this type
	std::size_t size_of(void)
	{
		return _size_of;
	}

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_ALIGNMENT_OF
	// Returns the alignment of this type
	std::size_t alignment_of(void)
	{
		return _alignment_of;
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_ARRAY
	bool is_array(void)
	{
		return _is_array;
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_CONST
	bool is_const(void)
	{
		return _is_const;
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_POINTER
	bool is_pointer(void)
	{
		return _is_pointer;
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_REFERENCE
	bool is_reference(void)
	{
		return _is_reference;
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_IS_VOLATILE
	bool is_volatile(void)
	{
		return _is_volatile;
	}
#endif
};

template <typename MetaType>
class meta_type_base_
 : public aux::meta_type_base_utils<MetaType>
 , public meta_type_base_base_
{
private:
	typedef aux::meta_type_base_utils<MetaType> utils;

	typedef typename MetaType::original_type original_type;

public:
	inline meta_type_base_(void)
	 : meta_type_base_base_(MetaType())
	{ }

	// Allocates and constructs a new instance of the reflected type
	raw_ptr new_(void)
	{
		return utils::make_new(
			typename utils::_is_default_constructible()
		);
	}

	// Allocates and copy-constructs a new instance of the reflected type
	raw_ptr new_copy(raw_ptr source)
	{
		return utils::make_new_copy(
			source,
			typename utils::_is_copy_constructible()
		);
	}

#if LAGOON_MT_WITH_DEFAULT
	// constructs a new default instance of the reflected type
	boost::any default_(void)
	{
		return utils::make_default(
			typename utils::_is_default_constructible()
		);
	}
#endif

#if LAGOON_MT_WITH_DELETE
	void delete_(raw_ptr ptr)
	{
		utils::delete_(ptr);
	}
#endif

#if LAGOON_MT_WITH_MAKE_SHARED
	shared_raw_ptr make_shared(raw_ptr ptr)
	{
		return utils::make_shared(ptr);
	}
#endif

#if LAGOON_MT_WITH_MAKE_UNIQUE
	unique_raw_ptr make_unique(raw_ptr ptr)
	{
		return std::move(utils::make_unique(ptr));
	}
#endif

#if LAGOON_MT_WITH_MAKE_FACTORY
	std::unique_ptr<polymorph_factory> make_factory(
		polymorph_factory_builder& builder,
		raw_ptr build_data
	)
	{
		return utils::make_factory(builder, build_data);
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_ADD_CONST
	shared<meta_type> add_const(void)
	{
		return apply_and_wrap<std::add_const>();
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_ADD_CV
	shared<meta_type> add_cv(void)
	{
		return apply_and_wrap<std::add_cv>();
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_ADD_VOLATILE
	shared<meta_type> add_volatile(void)
	{
		return apply_and_wrap<std::add_volatile>();
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_REMOVE_CONST
	shared<meta_type> remove_const(void)
	{
		return apply_and_wrap<std::remove_const>();
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_REMOVE_CV
	shared<meta_type> remove_cv(void)
	{
		return apply_and_wrap<std::remove_cv>();
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_REMOVE_POINTER
	shared<meta_type> remove_pointer(void)
	{
		return apply_and_wrap<std::remove_pointer>();
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_REMOVE_REFERENCE
	shared<meta_type> remove_reference(void)
	{
		return apply_and_wrap<std::remove_reference>();
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_REMOVE_VOLATILE
	shared<meta_type> remove_volatile(void)
	{
		return apply_and_wrap<std::remove_volatile>();
	}
#endif

#if LAGOON_ALL_TYPE_TRAITS || LAGOON_MT_WITH_REMOVE_EXTENT
	shared<meta_type> remove_extent(void)
	{
		return apply_and_wrap<std::remove_extent>();
	}
#endif

};

//  Implementation of the meta_type interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
template <typename MetaType>
class meta_type_
 : public meta_type_base_<MetaType>
 , public meta_named_scoped_object_
 , virtual public meta_type
{
public:
	inline meta_type_(void)
	 : meta_named_scoped_object_(MetaType())
	{ }
};

// Base implementation of the meta_class interface
class meta_class_base_
 : virtual public meta_class
{
private:
	shared<elaborated_type_specifier> _elaborated_type;

	range<meta_inheritance>(*_get_base_classes)(void);
	range<meta_class>(*_get_class_layout)(void);
	range<meta_member_variable>(*_get_member_variables)(void);
#if LAGOON_MC_WITH_ALL_MEMBER_VARIABLES
	range<meta_member_variable>(*_get_all_member_variables)(void);
#endif
	range<meta_overloaded_functions>(*_get_member_functions)(void);
	range<meta_conversion_operator>(*_get_conversions)(void);
public:
	template <typename MetaClass>
	inline meta_class_base_(MetaClass)
	 : _elaborated_type(
		aux::get_elaborated_type_spec<
			typename MetaClass::elaborated_type,
			elaborated_type_specifier
		>()
	), _get_base_classes(
		&aux::sequence_holder<
			MetaClass,
			meta_inheritance,
			mirror::base_classes
		>::all
	), _get_class_layout(
		&aux::sequence_holder<
			MetaClass,
			meta_class,
			mirror::class_layout
		>::all
	), _get_member_variables(
		&aux::sequence_holder<
			MetaClass,
			meta_member_variable,
			mirror::member_variables
		>::all
#if LAGOON_MC_WITH_ALL_MEMBER_VARIABLES
	), _get_all_member_variables(
		&aux::sequence_holder<
			MetaClass,
			meta_member_variable,
			mirror::all_member_variables
		>::all
#endif
	), _get_member_functions(
		&aux::sequence_holder<
			MetaClass,
			meta_overloaded_functions,
			mirror::member_functions
		>::all
	), _get_conversions(
		&aux::sequence_holder<
			MetaClass,
			meta_conversion_operator,
			mirror::conversions
		>::all
	)
	{ }

	shared<elaborated_type_specifier> elaborated_type(void)
	{
		return _elaborated_type;
	}

	range<meta_inheritance> base_classes(void)
	{
		assert(_get_base_classes != nullptr);
		return _get_base_classes();
	}

	range<meta_class> class_layout(void)
	{
		assert(_get_class_layout != nullptr);
		return _get_class_layout();
	}

	range<meta_member_variable> member_variables(void)
	{
		assert(_get_member_variables != nullptr);
		return _get_member_variables();
	}

#if LAGOON_MC_WITH_ALL_MEMBER_VARIABLES
	range<meta_member_variable> all_member_variables(void)
	{
		assert(_get_all_member_variables != nullptr);
		return _get_all_member_variables();
	}
#endif

	range<meta_overloaded_functions> member_functions(void)
	{
		assert(_get_member_functions != nullptr);
		return _get_member_functions();
	}

	range<meta_conversion_operator> conversions(void)
	{
		assert(_get_conversions != nullptr);
		return _get_conversions();
	}
};

//  Implementation of the meta_class interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros and functions.
 */
template <typename MetaClass>
class meta_class_
 : public meta_scope_
 , public meta_type_base_<MetaClass>
 , public meta_class_base_
{
public:
	inline meta_class_(void)
	 : meta_scope_(MetaClass())
	 , meta_class_base_(MetaClass())
	{ }
};

//  Implementation of the meta_type_template interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_type_template_
 : public meta_object_
 , public meta_named_scoped_object_
 , virtual public meta_type_template
{
public:
	template <typename MetaTypeTemplate>
	inline meta_type_template_(MetaTypeTemplate)
	 : meta_object_(MetaTypeTemplate())
	 , meta_named_scoped_object_(MetaTypeTemplate())
	{ }
};


template <typename BaseInterface>
class meta_templated_type_base_
 : virtual BaseInterface
{
private:
	shared<meta_type_template>(*_get_type_template)(void);
	range<meta_type>(*_get_template_parameters)(void);
public:
	template <typename MetaTemplatedType>
	inline meta_templated_type_base_(MetaTemplatedType)
	 : _get_type_template(
		&aux::get_meta_object<
			typename MetaTemplatedType::type_template,
			meta_type_template
		>
	), _get_template_parameters(
		&aux::sequence_holder<
			MetaTemplatedType,
			meta_type,
			mirror::template_parameters
		>::all
	)
	{ }

	shared<meta_type_template> type_template(void)
	{
		assert(_get_type_template != nullptr);
		return _get_type_template();
	}

	range<meta_type> template_parameters(void)
	{
		assert(_get_template_parameters != nullptr);
		return _get_template_parameters();
	}
};

//  Implementation of the meta_templated_type interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
template <typename MetaTemplatedType>
class meta_templated_type_
 : public meta_type_<MetaTemplatedType>
 , public meta_templated_type_base_<meta_templated_type>
{
public:
	inline meta_templated_type_(void)
	 : meta_templated_type_base_<meta_templated_type>(MetaTemplatedType())
	{ }
};

//  Implementation of the meta_templated_class interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
template <typename MetaTemplatedClass>
class meta_templated_class_
 : public meta_class_<MetaTemplatedClass>
 , public meta_templated_type_base_<meta_templated_class>
{
public:
	inline meta_templated_class_(void)
	 : meta_templated_type_base_<meta_templated_class>(MetaTemplatedClass())
	{ }
};

class meta_enum_base_
 : virtual public meta_enum
{
private:
	int item_count;
	// NOTE: this refers to the name cache hidden inside
	// of Mirror's meta_enum(s)
	const std::vector<const char*>& name_cache;

	std::vector<int> value_cache;

	template <typename MetaEnum>
	static range<meta_named_scoped_object> do_get_members(void)
	{
		typedef aux::sequence_holder<
			MetaEnum,
			meta_named_scoped_object,
			mirror::members
		> sequence;
#if LAGOON_NO_ENUM_MEMBERS
		return sequence::make(std::false_type());
#else
		//TODO: define and implement meta_enum_value
		//return sequence::make(std::false_type());
		return sequence::all();
#endif
	}

	range<meta_named_scoped_object> (*_get_members)(void);
public:
	template <typename MetaEnum>
	inline meta_enum_base_(MetaEnum me)
	 : item_count(MetaEnum::size::value)
	 , name_cache(MetaEnum::_name_cache())
	 , value_cache(MetaEnum::size::value, 0)
	 , _get_members(&meta_enum_base_::template do_get_members<MetaEnum>)
	{
		for(int i=0; i!=MetaEnum::size::value; ++i)
			value_cache[i] = int(MetaEnum::value(i));
	}

	range<meta_named_scoped_object> members(void)
	{
		assert(_get_members != nullptr);
		return _get_members();
	}

	int size(void)
	{
		return item_count;
	}

	int value(int index)
	{
		return value_cache[index];
	}

	std::string value_name(int index)
	{
		return name_cache[index];
	}

	int value_by_name(const std::string& name)
	{
		for(int i=0; i!=size();++i)
			if(value_name(i) == name)
				return value(i);
		return 0;
	}

	std::string name_by_value(int val)
	{
		for(int i=0; i!=size();++i)
			if(value(i) == val)
				return value_name(i);
		return std::string();
	}

	bool has_value_name(const std::string& name)
	{
		for(int i=0; i!=size();++i)
			if(value_name(i) == name)
				return true;
		return false;
	}
};

//  Implementation of the meta_enum interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
template <typename MetaEnum>
class meta_enum_
 : public meta_type_base_<MetaEnum>
 , public meta_named_scoped_object_
 , public meta_enum_base_
{
public:
	inline meta_enum_(void)
	 : meta_named_scoped_object_(MetaEnum())
	 , meta_enum_base_(MetaEnum())
	{ }
};

//  Implementation of the meta_typedef interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_typedef_base_
 : virtual meta_typedef
{
private:
	shared<meta_type> _type;
public:
	template <typename MetaTypedef>
	inline meta_typedef_base_(MetaTypedef)
	 : _type(
		aux::get_meta_object<
			typename MetaTypedef::type,
			meta_type
		>()
	){ }

	shared<meta_type> type(void)
	{
		return _type;
	}
};

//  Implementation of the meta_typedef interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
template <typename MetaTypedef>
class meta_typedef_
 : public meta_type_<MetaTypedef>
 , public meta_typedef_base_
{
public:
	inline meta_typedef_(void)
	 : meta_type_<MetaTypedef>()
	 , meta_typedef_base_(MetaTypedef())
	{ }
};

//  Implementation of the meta_namespace interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_namespace_
 : public meta_object_
 , public meta_scope_
 , virtual public meta_namespace
{
private:
	range<meta_namespace>(*_get_namespaces)(void);
	range<meta_free_variable>(*_get_free_variables)(void);
	range<meta_overloaded_functions>(*_get_free_functions)(void);
public:
	template <typename MetaNamespace>
	inline meta_namespace_(MetaNamespace)
	 : meta_object_(MetaNamespace())
	 , meta_scope_(MetaNamespace())
	 , _get_namespaces(
		&aux::sequence_holder<
			MetaNamespace,
			meta_namespace,
			mirror::namespaces
		>::all
	), _get_free_variables(
		&aux::sequence_holder<
			MetaNamespace,
			meta_free_variable,
			mirror::free_variables
		>::all
	), _get_free_functions(
		&aux::sequence_holder<
			MetaNamespace,
			meta_overloaded_functions,
			mirror::free_functions
		>::all
	)
	{ }

	range<meta_namespace> namespaces(void)
	{
		assert(_get_namespaces != nullptr);
		return _get_namespaces();
	}

	range<meta_free_variable> free_variables(void)
	{
		assert(_get_free_variables != nullptr);
		return _get_free_variables();
	}

	range<meta_overloaded_functions> free_functions(void)
	{
		assert(_get_free_functions != nullptr);
		return _get_free_functions();
	}
};

//  Implementation of the meta_global_scope interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_global_scope_
 : public meta_namespace_
 , virtual public meta_global_scope
{
public:
	template <typename MetaGlobalScope>
	inline meta_global_scope_(MetaGlobalScope)
	 : meta_namespace_(MetaGlobalScope())
	{ }
};

//  Implementation of the meta_inheritance interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_inheritance_
 : public meta_object_
 , virtual public meta_inheritance
{
private:
	shared<inheritance_type_specifier> _inheritance_type;
	shared<access_type_specifier> _access_type;
	shared<meta_class>(*_get_base_class)(void);
	shared<meta_class>(*_get_derived_class)(void);
public:
	template <typename MetaInheritance>
	inline meta_inheritance_(MetaInheritance)
	 : meta_object_(MetaInheritance())
	 , _inheritance_type(
		aux::get_inheritance_type_spec<
			typename MetaInheritance::inheritance_type,
			inheritance_type_specifier
		>()
	), _access_type(
		aux::get_access_type_spec<
			typename MetaInheritance::access_type,
			access_type_specifier
		>()
	), _get_base_class(
		&aux::get_meta_object<
			typename MetaInheritance::base_class,
			meta_class
		>
	), _get_derived_class(
		&aux::get_meta_object<
			typename MetaInheritance::derived_class,
			meta_class
		>
	)
	{ }

	shared<inheritance_type_specifier> inheritance_type(void)
	{
		return _inheritance_type;
	}

	shared<access_type_specifier> access_type(void)
	{
		return _access_type;
	}

	shared<meta_class> base_class(void)
	{
		assert(_get_base_class != nullptr);
		return _get_base_class();
	}

	shared<meta_class> derived_class(void)
	{
		assert(_get_derived_class != nullptr);
		return _get_derived_class();
	}
};

//  Implementation of the meta_variable interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_variable_
 : public meta_object_
 , public meta_named_scoped_object_
 , virtual public meta_variable
{
private:
	shared<storage_class_specifier> _storage_class;
	shared<meta_type> _type;
public:
	template <typename MetaVariable>
	inline meta_variable_(MetaVariable)
	 : meta_object_(MetaVariable())
	 , meta_named_scoped_object_(MetaVariable())
	 , _storage_class(
		aux::get_storage_class_spec<
			typename MetaVariable::storage_class,
			storage_class_specifier
		>()
	), _type(
		aux::get_meta_object<
			typename MetaVariable::type,
			meta_type
		>()
	){ }

	shared<storage_class_specifier> storage_class(void)
	{
		return _storage_class;
	}

	shared<meta_type> type(void)
	{
		return _type;
	}
};

//  Implementation of the meta_free_variable interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_free_variable_
 : public meta_variable_
 , virtual public meta_free_variable
{
	// TODO: get/set
};

//  Implementation of the meta_plain_free_variable interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_plain_free_variable_
 : public meta_free_variable_
 , virtual public meta_plain_free_variable
{
	// TODO: address
};

//  Implementation of the meta_member_variable interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_member_variable_
 : public meta_variable_
 , virtual public meta_member_variable
{
private:
	shared<access_type_specifier> _access_type;
public:
	template <typename MetaMemberVariable>
	inline meta_member_variable_(MetaMemberVariable)
	 : meta_variable_(MetaMemberVariable())
	 , _access_type(
		aux::get_access_type_spec<
			typename MetaMemberVariable::access_type,
			access_type_specifier
		>()
	){ }

	shared<access_type_specifier> access_type(void)
	{
		return _access_type;
	}
};

//  Implementation of the meta_parameter interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_parameter_
 : public meta_variable_
 , virtual public meta_parameter
{
private:
	int _position;
public:
	template <typename MetaParameter>
	inline meta_parameter_(MetaParameter)
	 : meta_variable_(MetaParameter())
	 , _position(MetaParameter::position::value)
	{ }

	int position(void)
	{
		return _position;
	}
};

//  Implementation of the meta_overloaded_functions interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_overloaded_functions_
 : public meta_object_
 , public meta_named_scoped_object_
 , virtual public meta_overloaded_functions
{
private:
	range<meta_function>(*_get_overloads)(void);
public:
	template <typename MetaOverloadedFunctions>
	inline meta_overloaded_functions_(MetaOverloadedFunctions)
	 : meta_object_(MetaOverloadedFunctions())
	 , meta_named_scoped_object_(MetaOverloadedFunctions())
	 , _get_overloads(
		&aux::sequence_holder<
			MetaOverloadedFunctions,
			meta_function,
			mirror::overloads
		>::all
	)
	{ }

	range<meta_function> overloads(void)
	{
		assert(_get_overloads != nullptr);
		return _get_overloads();
	}
};

//  Implementation of the meta_function interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_function_
 : public meta_object_
 , public meta_scope_
 , virtual public meta_function
{
private:
	shared<meta_type> _result_type;
	range<meta_parameter>(*_get_parameters)(void);
public:
	template <typename MetaFunction>
	inline meta_function_(MetaFunction)
	 : meta_object_(MetaFunction())
	 , meta_scope_(MetaFunction())
	 , _result_type(
		aux::get_meta_object<
			typename MetaFunction::result_type,
			meta_type
		>()
	), _get_parameters(
		&aux::sequence_holder<
			MetaFunction,
			meta_parameter,
			mirror::parameters
		>::all
	)
	{ }

	shared<meta_type> result_type(void)
	{
		return _result_type;
	}

	range<meta_parameter> parameters(void)
	{
		assert(_get_parameters != nullptr);
		return _get_parameters();
	}
};

//  Implementation of the meta_constructor interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_constructor_
 : public meta_function_
 , virtual public meta_constructor
{
private:
	shared<access_type_specifier> _access_type;
public:
	template <typename MetaConstructor>
	inline meta_constructor_(MetaConstructor)
	 : meta_function_(MetaConstructor())
	 , _access_type(
		aux::get_access_type_spec<
			typename MetaConstructor::access_type,
			access_type_specifier
		>()
	){ }

	shared<access_type_specifier> access_type(void)
	{
		return _access_type;
	}
};

//  Implementation of the meta_member_function interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_member_function_
 : public meta_function_
 , virtual public meta_member_function
{
private:
	shared<access_type_specifier> _access_type;
	shared<storage_class_specifier> _storage_class;
	shared<constness_specifier> _constness;
public:
	template <typename MetaMemberFunction>
	inline meta_member_function_(MetaMemberFunction)
	 : meta_function_(MetaMemberFunction())
	 , _access_type(
		aux::get_access_type_spec<
			typename MetaMemberFunction::access_type,
			access_type_specifier
		>()
	), _storage_class(
		aux::get_storage_class_spec<
			typename MetaMemberFunction::storage_class,
			storage_class_specifier
		>()
	), _constness(
		aux::get_constness_spec<
			typename MetaMemberFunction::constness,
			constness_specifier
		>()
	){ }

	shared<access_type_specifier> access_type(void)
	{
		return _access_type;
	}

	/// returns the storage class specifier
	shared<storage_class_specifier> storage_class(void)
	{
		return _storage_class;
	}

	/// returns the constness specifier
	shared<constness_specifier> constness(void)
	{
		return _constness;
	}
};

//  Implementation of the meta_conversion_operator interface
/*  Do not use this template class directly, instances
 *  of this class are created directly or indirectly
 *  by Lagoon's reflection macros.
 */
class meta_conversion_operator_
 : public meta_member_function_
 , virtual public meta_conversion_operator
{
};

LAGOON_NAMESPACE_END

#endif //include guard

