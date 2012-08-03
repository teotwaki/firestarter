/**
 * @file rubber/meta_objects.hpp
 * @brief Wrapper for meta-objects
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_META_OBJECTS_1103151301_HPP
#define RUBBER_META_OBJECTS_1103151301_HPP

#include <rubber/config.hpp>
#include <rubber/range.hpp>
#include <rubber/meta_object.hpp>

#include <cassert>
#include <string>

RUBBER_NAMESPACE_BEGIN

/// Class erasing the type of Mirror's MetaNamedObjects
/**
 *  @see mirror::MetaNamedObject
 *  @ingroup rubber_type_erased
 */
class meta_named_object
 : virtual public meta_object
{
private:
	std::string _base_name;
public:
	/// Constructor erasing the type of a MetaNamedObject
	template <typename MetaNamedObject>
	meta_named_object(MetaNamedObject mo)
	 : meta_object(mo)
	 , _base_name(MetaNamedObject::base_name())
	{ }

	/// Returns the base-name of the reflected entity
	inline std::string base_name(void) const
	{
		return _base_name;
	}
};

/// Class erasing the type of Mirror's MetaScopedObjects
/**
 *  @see mirror::MetaScopedObject
 *  @ingroup rubber_type_erased
 */
class meta_scoped_object
 : virtual public meta_object
{
private:
	aux::return_value<meta_scope>::type (*_get_scope)(void);
public:
	/// Constructor erasing the type of a MetaScopedObject
	template <typename MetaScopedObject>
	meta_scoped_object(MetaScopedObject mo)
	 : meta_object(mo)
	 , _get_scope(
		&wrap_shaped<
			typename MetaScopedObject::scope,
			meta_scope
		>
	)
	{ }

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns the scope of the reflected scoped object
	meta_scope scope(void) const;
#else
	inline aux::return_value<meta_scope>::type scope(void) const;
#endif
};

/// Class erasing the type of Mirror's MetaNamedScopedObjects
/**
 *  @see mirror::MetaNamedScopedObject
 *  @ingroup rubber_type_erased
 */
class meta_named_scoped_object
 : public meta_scoped_object
 , public meta_named_object
{
private:
	std::string(*_get_full_name)(void);
#if RUBBER_MSNO_WITH_LOCAL_NAME
	std::string(*_get_local_name)(void);
#endif
public:
	/// Constructor erasing the type of a MetaNamedScopedObject
	template <typename MetaNamedScopedObject>
	meta_named_scoped_object(MetaNamedScopedObject mo)
	 : meta_object(mo)
	 , meta_scoped_object(mo)
	 , meta_named_object(mo)
	 , _get_full_name(&MetaNamedScopedObject::full_name)
#if RUBBER_MSNO_WITH_LOCAL_NAME
	 , _get_local_name(&MetaNamedScopedObject::local_name)
#endif
	{ }

	/// Returns the fully specified path of the reflected named object
	inline std::string full_name(void) const
	{
		assert(_get_full_name != nullptr);
		return _get_full_name();
	}

#if RUBBER_MSNO_WITH_LOCAL_NAME || MIRROR_DOCUMENTATION_ONLY
	/// Returns the local name of the reflected named object
	/**
	 *  This member function if available only if the
	 *  #RUBBER_MSNO_WITH_LOCAL_NAME preprocessor symbol
	 *  is defined with a nonzero integer value.
	 *
	 *  @see mirror::MetaNamedScopedObject::local_name()
	 *  @see base_name
	 *  @see full_name
	 */
	inline std::string local_name(void) const
	{
		assert(_get_local_name != nullptr);
		return _get_local_name();
	}
#endif
};

/// Class erasing the type of Mirror's MetaScopes
/**
 *  @see mirror::MetaScope
 *  @ingroup rubber_type_erased
 */
class meta_scope
 : virtual public meta_named_scoped_object
{
private:
	range_holder<meta_named_scoped_object> _members;

	template <typename MetaScope>
	static mirror::members<MetaScope> static_members(
		MetaScope,
		const mirror::meta_scope_tag&
	)
	{
		return mirror::members<MetaScope>();
	}

#if RUBBER_NO_NAMESPACE_MEMBERS
	template <typename MetaNamespace>
	static mirror::mp::empty_range static_members(
		MetaNamespace,
		const mirror::meta_namespace_tag&
	)
	{
		return mirror::mp::empty_range();
	}
#endif
public:
	/// Constructor erasing the type of a MetaScope
	template <typename MetaScope>
	meta_scope(MetaScope mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , _members(static_members(mo, mirror::categorize_meta_object(mo)))
	{ }

	/// Returns a range of members of the reflected scope
	range<meta_named_scoped_object> members(void) const
	{
		return _members.all();
	}
};

/// Class erasing the type of Mirror's MetaNamespaces
/**
 *  @see mirror::MetaNamespace
 *  @ingroup rubber_type_erased
 */
class meta_namespace
 : public meta_scope
{
private:
	range_holder<meta_namespace> _namespaces;
	range_holder<meta_free_variable> _free_variables;
	range_holder<meta_overloaded_functions> _free_functions;
public:
	/// Constructor erasing the type of a MetaNamespace
	template <typename MetaNamespace>
	meta_namespace(MetaNamespace mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_scope(mo)
	 , _namespaces(mirror::namespaces<MetaNamespace>())
	 , _free_variables(mirror::free_variables<MetaNamespace>())
	 , _free_functions(mirror::free_functions<MetaNamespace>())
	{ }

	/// Returns a range of meta-namespaces reflecting nested namespaces
	range<meta_namespace> namespaces(void) const
	{
		return _namespaces.all();
	}

	/// Returns a range of meta-variables reflecting namespace's variables
	range<meta_free_variable> free_variables(void) const
	{
		return _free_variables.all();
	}

	/// Returns a range of meta-functions reflecting namespace's functions
	range<meta_overloaded_functions> free_functions(void) const
	{
		return _free_functions.all();
	}
};

/// Class erasing the type of Mirror's MetaGlobalScope
/**
 *  @see mirror::MetaGlobalScope
 *  @ingroup rubber_type_erased
 */
class meta_global_scope
 : public meta_namespace
{
private:
public:
	/// Constructor erasing the type of a MetaGlobalScope
	template <typename MetaGlobalScope>
	meta_global_scope(MetaGlobalScope mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_namespace(mo)
	{ }
};

/// Class erasing the type of Mirror's MetaTypes
/**
 *  @see mirror::MetaType
 *  @ingroup rubber_type_erased
 */
class meta_type
 : virtual public meta_named_scoped_object
{
private:
	const std::type_info& _type_id;

	template <typename T>
	static const std::size_t get_sizeof(std::false_type)
	{
		return sizeof(T);
	}

	template <typename T>
	static const std::size_t get_sizeof(std::true_type)
	{
		return 0;
	}

	template <typename T>
	static const std::size_t get_sizeof(void)
	{
		return get_sizeof<T>(
			typename std::is_void<T>::type()
		);
	}

	std::size_t _size_of;

	range_holder<meta_constructor> _constructors;

	range_holder<meta_container> _containers;

	template <typename T, typename B1, typename B2>
	static inline some create(B1, B2)
	{
		return some();
	}

	template <typename T>
	static inline some create(std::true_type, std::true_type)
	{
		return some();
		//return some(T());
	}

	template <typename OriginalType>
	static some call_create(void)
	{
		return create<OriginalType>(
			typename mirror::is_default_constructible<
				OriginalType
			>::type(),
			typename mirror::is_copy_constructible<
				OriginalType
			>::type()
		);
	}

	some (*_call_create)(void);

#ifdef RUBBER_META_TYPE_OPERATIONS
	template <typename T>
	static void call_op(
		void(*operation_ptr)(void),
		some& value,
		some& context,
		std::false_type
	)
	{
		if(operation_ptr)
		{
			assert(!value.empty());
			typedef void(*operation_type)(T&, some&);
			operation_type operation = operation_type(operation_ptr);
			operation(some_cast<T>(value), context);
		}
	}

	template <typename T>
	static void call_op(
		void(*operation_ptr)(void),
		some& value,
		some& context,
		std::true_type
	)
	{ }

	template <typename T>
	static void call_op(
		void(*operation_ptr)(void),
		some& value,
		some& context
	)
	{
		call_op<T>(
			operation_ptr,
			value,
			context,
			typename std::is_void<T>::type()
		);
	}

	void (*_call_op_mutating)(void(*)(void), some&, some&);
	void (*_call_op_const)(void(*)(void), some&, some&);

	struct ti_less_than
	{
		bool operator()(
			const std::type_info* a,
			const std::type_info* b
		) const
		{
			return a->before(*b);
		}
	};

	typedef std::map<
		const std::type_info*,
		void(*)(void),
		ti_less_than
	> operation_map;

	operation_map mutating_operations, const_operations;

	void do_init_mutating_op(
		void(*raw_operation_ptr)(void),
		const std::type_info& tag
	)
	{
		mutating_operations[&tag] = raw_operation_ptr;
	}

	template <typename T>
	void do_init_op(
		void (*operation_ptr)(T&, some&),
		const std::type_info& tag,
		std::true_type
	)
	{
		typedef void(*raw)(void);
		do_init_mutating_op(raw(operation_ptr), tag);
	}

	void do_init_const_op(
		void(*raw_operation_ptr)(void),
		const std::type_info& tag
	)
	{
		const_operations[&tag] = raw_operation_ptr;
	}

	template <typename T>
	void do_init_op(
		void (*operation_ptr)(const T&, some&),
		const std::type_info& tag,
		std::false_type
	)
	{
		typedef void(*raw)(void);
		do_init_const_op(raw(operation_ptr), tag);
	}

	template <typename Tag, typename T>
	void init_op(T*, std::true_type)
	{
		typename Tag::mutating is_mutating;
		do_init_op<T>(
			&Tag::apply,
			typeid(Tag),
			is_mutating
		);
	}

	template <typename Tag, typename T>
	void init_op(T*, std::false_type)
	{ }

	template <typename T>
	struct operation_initializer
	{
		meta_type& that;
		T* selector;

		template <typename Op>
		void operator()(Op)
		{
			decltype(Op::available(*selector)) available;
			that.init_op<Op>(selector, available);
		}
	};

	template <typename T>
	void init_operations(T* _dummy)
	{
		operation_initializer<T> init = {*this, _dummy};
		mirror::mp::for_each<
			mirror::mp::range<
				RUBBER_META_TYPE_OPERATIONS
			>
		>(init);
	}

	void init_operations(void*)
	{ }
#endif // RUBBER_META_TYPE_OPERATIONS
public:
	/// Constructor erasing the type of a MetaType
	template <typename MetaType>
	meta_type(MetaType mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , _type_id(typeid(typename MetaType::original_type))
	 , _size_of(get_sizeof<typename MetaType::original_type>())
	 , _constructors(mirror::constructors<MetaType>())
	 , _containers(mirror::containers<MetaType>())
	 , _call_create(&call_create<typename MetaType::original_type>)
#ifdef RUBBER_META_TYPE_OPERATIONS
	 , _call_op_mutating(&call_op<typename MetaType::original_type>)
	 , _call_op_const(&call_op<const typename MetaType::original_type>)
	{
		init_operations((typename MetaType::original_type*)nullptr);
	}
#else  // no RUBBER_META_TYPE_OPERATIONS
	{ }
#endif // RUBBER_META_TYPE_OPERATIONS

	/// Returns the standard type information for the reflected type
	const std::type_info& typeid_(void) const
	{
		return _type_id;
	}

	/// Returns the instance size in bytes for the reflected type
	std::size_t sizeof_(void) const
	{
		return _size_of;
	}

	/// Returns the constructors of the type
	range<meta_constructor> constructors(void) const
	{
		return _constructors.all();
	}

	/// Default-constructs (if possible) an instance of the reflected type
	/** If the reflected type supports default construction, then this
	 *  member function creates and returns a new instance. Otherwise
	 *  the returned @c some type is empty.
	 */
	some create(void) const
	{
		assert(_call_create != nullptr);
		return _call_create();
	}

	/// Returns the containers of the type
	range<meta_container> containers(void) const
	{
		return _containers.all();
	}

#ifdef RUBBER_META_TYPE_OPERATIONS
	//TODO: docs
	template <typename Tag>
	bool can_examine_by(void) const
	{
		return	const_operations.find(&typeid(Tag)) !=
			const_operations.end();
	}

	void examine_by(
		const std::type_info& tag,
		some& value,
		some& context
	) const
	{
		assert(_call_op_const != nullptr);
		auto op = const_operations.find(&tag);
		_call_op_const(op->second, value, context);
	}

	template <typename Tag>
	void examine_by(some& value, some& context) const
	{
		examine_by(typeid(Tag), value, context);
	}

	template <typename Tag>
	void examine_by(some value) const
	{
		some context;
		examine_by(typeid(Tag), value, context);
	}

	template <typename Tag>
	bool can_modify_by(void) const
	{
		return	mutating_operations.find(&typeid(Tag)) !=
			mutating_operations.end();
	}

	void modify_by(
		const std::type_info& tag,
		some& value,
		some& context
	)
	{
		assert(_call_op_mutating != nullptr);
		auto op = mutating_operations.find(&tag);
		_call_op_mutating(op->second, value, context);
	}

	template <typename Tag>
	void modify_by(some& value, some& context)
	{
		modify_by(typeid(Tag), value, context);
	}

	template <typename Tag>
	void modify_by(some& value)
	{
		some context;
		modify_by(typeid(Tag), value, context);
	}
#endif // RUBBER_META_TYPE_OPERATIONS
};

/// Class erasing the type of Mirror's MetaTypedefs
/**
 *  @see mirror::MetaTypedef
 *  @ingroup rubber_type_erased
 */
class meta_typedef
 : public meta_type
{
private:
	aux::return_value<meta_type>::type (*_get_type)(void);
public:
	/// Constructor erasing the type of a MetaTypedef
	template <typename MetaTypedef>
	meta_typedef(MetaTypedef mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_type(mo)
	 , _get_type(
		&wrap_shaped<
			typename MetaTypedef::type,
			meta_type
		>
	)
	{ }

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns meta-type reflecting the source type of the typedef
	meta_type type(void) const;
#else
	inline aux::return_value<meta_type>::type type(void) const;
#endif
};

/// Class erasing the type of Mirror's MetaTypeTemplates
/**
 *  @see mirror::MetaTypeTemplate
 *  @ingroup rubber_type_erased
 */
class meta_type_template
 : public meta_named_scoped_object
{
private:
public:
	/// Constructor erasing the type of a MetaTypeTemplate
	template <typename MetaTypeTemplate>
	meta_type_template(MetaTypeTemplate mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	{ }

	// TODO
};

/// Class erasing the type of Mirror's MetaTemplatedTypes
/**
 *  @see mirror::MetaTemplateType
 *  @ingroup rubber_type_erased
 */
class meta_templated_type
 : virtual public meta_type
{
private:
	aux::return_value<meta_type_template>::type (*_get_type_template)(void);
public:
	/// Constructor erasing the type of a MetaTemplatedType
	template <typename MetaTemplatedType>
	meta_templated_type(MetaTemplatedType mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_type(mo)
	 , _get_type_template(
		&wrap_shaped<
			typename MetaTemplatedType::type_template,
			meta_type_template
		>
	)
	{ }

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns the template used in instantion of the reflected type
	meta_type_template type_template(void) const;
#else
	aux::return_value<meta_type_template>::type type_template(void) const;
#endif
};

/// Class erasing the type of Mirror's MetaClasses
/**
 *  @see mirror::MetaClass
 *  @ingroup rubber_type_erased
 */
class meta_class
 : virtual public meta_type
 , public meta_scope
{
private:
	specifier _elaborated_type;
	range_holder<meta_inheritance> _base_classes;
	range_holder<meta_class> _class_layout;
	range_holder<meta_member_variable> _member_variables;
	range_holder<meta_member_variable> _all_member_variables;
	range_holder<meta_member_function> _member_functions;
	range_holder<meta_container> _all_containers;
public:
	/// Constructor erasing the type of a MetaClass
	template <typename MetaClass>
	meta_class(MetaClass mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_type(mo)
	 , meta_scope(mo)
	 , _elaborated_type(typename MetaClass::elaborated_type())
	 , _base_classes(mirror::base_classes<MetaClass>())
	 , _class_layout(mirror::class_layout<MetaClass>())
	 , _member_variables(mirror::member_variables<MetaClass>())
	 , _all_member_variables(mirror::all_member_variables<MetaClass>())
	 , _member_functions(mirror::member_functions<MetaClass>())
	 , _all_containers(mirror::all_containers<MetaClass>())
	{ }

	/// Returns the elaborated type specifier of the reflected class
	const specifier& elaborated_type(void) const
	{
		return _elaborated_type;
	}

	/// Returns a range of meta-inheritances reflecting the base-classes
	range<meta_inheritance> base_classes(void) const
	{
		return _base_classes.all();
	}

	range<meta_class> class_layout(void) const
	{
		return _class_layout.all();
	}

	/// Returns the member variables of the reflected class
	range<meta_member_variable> member_variables(void) const
	{
		return _member_variables.all();
	}

	/// Returns member variables of the reflected class including inherited ones
	range<meta_member_variable> all_member_variables(void) const
	{
		return _all_member_variables.all();
	}

	/// Returns the member functions of the reflected class
	range<meta_member_function> member_functions(void) const
	{
		return _member_functions.all();
	}

	/// Returns the containers of the reflected class
	range<meta_container> all_containers(void) const
	{
		return _all_containers.all();
	}
};

/// Class erasing the type of Mirror's MetaTemplatedClasses
/**
 *  @see mirror::MetaTemplatedClass
 *  @ingroup rubber_type_erased
 */
struct meta_templated_class
 : public meta_templated_type
 , public meta_class
{
private:
public:
	/// Constructor erasing the type of a MetaTemplatedClass
	template <typename MetaTemplatedClass>
	meta_templated_class(MetaTemplatedClass mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_type(mo)
	 , meta_templated_type(mo)
	 , meta_class(mo)
	{ }
};

/// Class erasing the type of Mirror's MetaInheritances
/**
 *  @see mirror::MetaInheritance
 *  @ingroup rubber_type_erased
 */
class meta_inheritance
 : virtual public meta_object
{
private:
	specifier _inheritance_type;
	specifier _access_type;
	aux::return_value<meta_class>::type (*_get_base_class)(void);
	aux::return_value<meta_class>::type (*_get_derived_class)(void);
public:
	/// Constructor erasing the type of a MetaInheritance
	template <typename MetaInheritance>
	meta_inheritance(MetaInheritance mo)
	 : meta_object(mo)
	 , _inheritance_type(typename MetaInheritance::inheritance_type())
	 , _access_type(typename MetaInheritance::access_type())
	 , _get_base_class(
		&wrap_shaped<
			typename MetaInheritance::base_class,
			meta_class
		>
	), _get_derived_class(
		&wrap_shaped<
			typename MetaInheritance::derived_class,
			meta_class
		>
	)
	{ }

	/// Returns the inheritance type specifier of the base class
	const specifier& inheritance_type(void) const
	{
		return _inheritance_type;
	}

	/// Returns the access type specifier of the base class
	const specifier& access_type(void) const
	{
		return _access_type;
	}

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_class reflecting the base class in the inheritance
	meta_class base_class(void) const;
#else
	inline aux::return_value<meta_class>::type base_class(void) const;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_class reflecting the derived class in the inheritance
	meta_class derived_class(void) const;
#else
	inline aux::return_value<meta_class>::type derived_class(void) const;
#endif
};

/// Class erasing the type of Mirror's MetaEnumValues
/**
 *  @see mirror::MetaEnumValue
 *  @ingroup rubber_type_erased
 */
class meta_enum_value
 : public meta_named_scoped_object
{
private:
	typedef long long value_type;
	value_type _value;
public:
	/// Constructor erasing the type of a MetaEnumValue
	template <typename MetaEnumValue>
	meta_enum_value(MetaEnumValue mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , _value(value_type(MetaEnumValue::value::value))
	{ }

	/// Returns the enumeration value cast to and integer
	value_type value(void) const
	{
		return _value;
	}
};

/// Class erasing the type of Mirror's MetaEnums
/**
 *  @see mirror::MetaEnum
 *  @ingroup rubber_type_erased
 */
class meta_enum
 : public meta_type
 , public meta_scope
{
private:
	int _size;
	range_holder<meta_enum_value> _enum_values;

	typedef long long value_type;

	template <typename MetaEnum>
	static value_type get_value(int index)
	{
		return value_type(MetaEnum::value(index));
	}

	value_type (*_get_value)(int);
	std::string (*_get_value_name)(int);

	template <typename MetaEnum>
	static std::string get_name_by_value(value_type value)
	{
		return MetaEnum::name_by_value(
			typename MetaEnum::original_type(value)
		);
	}

	std::string (*_get_name_by_value)(value_type);

	template <typename MetaEnum>
	static value_type get_value_by_name(const std::string& name)
	{
		return value_type(MetaEnum::value_by_name(name));
	}

	value_type (*_get_value_by_name)(const std::string&);
	bool (*_get_has_value_name)(const std::string&);
public:
	/// Constructor erasing the type of a MetaEnum
	template <typename MetaEnum>
	meta_enum(MetaEnum mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_type(mo)
	 , meta_scope(mo)
	 , _size(MetaEnum::size::value)
	 , _enum_values(mirror::enum_values<MetaEnum>())
	 , _get_value(&get_value<MetaEnum>)
	 , _get_value_name(&MetaEnum::value_name)
	 , _get_name_by_value(&get_name_by_value<MetaEnum>)
	 , _get_value_by_name(&get_value_by_name<MetaEnum>)
	 , _get_has_value_name(&MetaEnum::has_value_name)
	{ }

	/// Returns the number of values in the enumeration
	int size(void) const
	{
		return _size;
	}

	/// Returns the enumerated values
	range<meta_enum_value> enum_values(void) const
	{
		return _enum_values.all();
	}

	/// Returns the value at the specifier position
	value_type value(int index) const
	{
		assert(_get_value != nullptr);
		return _get_value(index);
	}

	/// Returns the name of the value at the specified position
	std::string value_name(int index) const
	{
		assert(_get_value_name != nullptr);
		return _get_value_name(index);
	}

	/// Returns the name of the specified enumerated value
	std::string name_by_value(value_type value) const
	{
		assert(_get_name_by_value!= nullptr);
		return _get_name_by_value(value);
	}

	/// Returns an enumerated value by its name
	value_type value_by_name(const std::string& name) const
	{
		assert(_get_value_by_name != nullptr);
		return _get_value_by_name(name);
	}

	/// Checks if there is a value with the specified name
	bool has_value_name(const std::string& name) const
	{
		assert(_get_has_value_name != nullptr);
		return _get_has_value_name(name);
	}
};

/// Class erasing the type of Mirror's MetaClassMembers
/**
 *  @see mirror::MetaClassMember
 *  @ingroup rubber_type_erased
 */
class meta_class_member
 : public meta_named_scoped_object
{
private:
	specifier _access_type;
public:
	/// Constructor erasing the type of a MetaClassMember
	template <typename MetaClassMember>
	meta_class_member(MetaClassMember mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , _access_type(typename MetaClassMember::access_type())
	{ }

	/// Returns the access type specifier of the class member
	const specifier& access_type(void) const
	{
		return _access_type;
	}

};

/// Class erasing the type of Mirror's MetaVariables
/**
 *  @see mirror::MetaVariable
 *  @ingroup rubber_type_erased
 */
class meta_variable
 : virtual public meta_named_scoped_object
{
private:
	specifier _storage_class;
	aux::return_value<meta_type>::type (*_get_type)(void);
public:
	/// Constructor erasing the type of a MetaVariable
	template <typename MetaVariable>
	meta_variable(MetaVariable mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , _storage_class(typename MetaVariable::storage_class())
	 , _get_type(
		&wrap_shaped<
			typename MetaVariable::type,
			meta_type
		>
	)
	{ }

	/// Returns the storage class specifier of the reflected variable
	const specifier& storage_class(void) const
	{
		return _storage_class;
	}

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type reflecting the type of the variable
	meta_type type(void) const;
#else
	inline aux::return_value<meta_type>::type type(void) const;
#endif
};

/// Class erasing the type of Mirror's MetaPlainVariables
/**
 *  @see mirror::MetaPlainVariable
 *  @ingroup rubber_type_erased
 */
class meta_plain_variable
 : virtual public meta_variable
{
public:
	/// Constructor erasing the type of a MetaPlainVariable
	template <typename MetaPlainVariable>
	meta_plain_variable(MetaPlainVariable mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_variable(mo)
	{ }
};

/// Class erasing the type of Mirror's MetaFreeVariables
/**
 *  @see mirror::MetaFreeVariable
 *  @ingroup rubber_type_erased
 */
class meta_free_variable
 : virtual public meta_variable
{
private:
	template <typename MetaFreeVariable>
	static some call_get(void)
	{
		return some(
			MetaFreeVariable::get(),
			typename mirror::mp::is_a<
				MetaFreeVariable,
				mirror::meta_plain_variable_tag
			>::type()
		);
	}

	some (*_call_get)(void);

	template <typename MetaFreeVariable>
	static void call_set(const some& value)
	{
		typedef typename MetaFreeVariable::type::original_type type;
		MetaFreeVariable::set(some_cast<const type>(value));
	}

	void (*_call_set)(const some&);
public:
	/// Constructor erasing the type of a MetaFreeVariable
	template <typename MetaFreeVariable>
	meta_free_variable(MetaFreeVariable mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_variable(mo)
	 , _call_get(&call_get<MetaFreeVariable>)
	 , _call_set(&call_set<MetaFreeVariable>)
	{ }

	/// Gets the value of the reflected variable
	some get(void) const
	{
		assert(_call_get != nullptr);
		return _call_get();
	}

	/// Sets the value of the reflected variable
	void set(const some& value) const
	{
		assert(_call_set != nullptr);
		_call_set(value);
	}
};

/// Class erasing the type of Mirror's MetaPlainFreeVariables
/**
 *  @see mirror::MetaPlainFreeVariable
 *  @ingroup rubber_type_erased
 */
class meta_plain_free_variable
 : public meta_free_variable
 , public meta_plain_variable
{
private:
	template <typename MetaPlainFreeVariable>
	static raw_ptr call_address(void)
	{
		return raw_ptr(MetaPlainFreeVariable::address());
	}

	raw_ptr (*_call_address)(void);
public:
	/// Constructor erasing the type of a MetaPlainFreeVariable
	template <typename MetaPlainFreeVariable>
	meta_plain_free_variable(MetaPlainFreeVariable mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_variable(mo)
	 , meta_free_variable(mo)
	 , meta_plain_variable(mo)
	 , _call_address(&call_address<MetaPlainFreeVariable>)
	{ }

	/// Returns the address of the reflected variable
	raw_ptr address(void) const
	{
		assert(_call_address != nullptr);
		return _call_address();
	}
};

/// Class erasing the type of Mirror's MetaMemberVariables
/**
 *  @see mirror::MetaMemberVariable
 *  @ingroup rubber_type_erased
 */
class meta_member_variable
 : virtual public meta_variable
 , public meta_class_member
{
private:
	template <typename MetaMemberVariable>
	static some call_get(const some& parent)
	{
		typedef typename MetaMemberVariable::scope::original_type
			parent_t;
		return some(
			MetaMemberVariable::get(
				some_cast<const parent_t>(parent)
			),
			typename mirror::mp::is_a<
				MetaMemberVariable,
				mirror::meta_plain_variable_tag
			>::type()
		);
	}

	some (*_call_get)(const some& parent);

	template <typename MetaMemberVariable>
	static void call_set(some& parent, const some& value)
	{
		typedef typename MetaMemberVariable::scope::original_type
			parent_t;
		typedef typename MetaMemberVariable::type::original_type type;
		MetaMemberVariable::set(
			some_cast<parent_t>(parent),
			some_cast<const type>(value)
		);
	}

	void (*_call_set)(some&, const some&);
public:
	/// Constructor erasing the type of a MetaMemberVariable
	template <typename MetaMemberVariable>
	meta_member_variable(MetaMemberVariable mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_variable(mo)
	 , meta_class_member(mo)
	 , _call_get(&call_get<MetaMemberVariable>)
	 , _call_set(&call_set<MetaMemberVariable>)
	{ }

	/// Returns the value of the member variable in the given instance
	some get(const some& parent) const
	{
		assert(_call_get != nullptr);
		return _call_get(parent);
	}

	/// Sets the value of the member variable in the given instance
	void set(some& parent, const some& value) const
	{
		assert(_call_set != nullptr);
		_call_set(parent, value);
	}
};

/// Class erasing the type of Mirror's MetaPlainMemberVariables
/**
 *  @see mirror::MetaPlainMemberVariable
 *  @ingroup rubber_type_erased
 */
class meta_plain_member_variable
 : public meta_member_variable
 , public meta_plain_variable
{
private:
	template <typename MetaPlainMemberVariable>
	static raw_ptr call_address(some& parent)
	{
		typedef typename MetaPlainMemberVariable::scope::original_type
			parent_t;
		return raw_ptr(
			MetaPlainMemberVariable::address(
				some_cast<parent_t>(parent)
			)
		);
	}

	raw_ptr (*_call_address)(some&);
public:
	/// Constructor erasing the type of a MetaPlainMemberVariable
	template <typename MetaPlainMemberVariable>
	meta_plain_member_variable(MetaPlainMemberVariable mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_variable(mo)
	 , meta_member_variable(mo)
	 , meta_plain_variable(mo)
	 , _call_address(&call_address<MetaPlainMemberVariable>)
	{ }

	/// Returns the address of the member variable in the given instance
	raw_ptr address(some& parent) const
	{
		assert(_call_address != nullptr);
		return _call_address(parent);
	}
};

/// Class erasing the type of Mirror's MetaParameter
/**
 *  @see mirror::MetaParameter
 *  @ingroup rubber_type_erased
 */
class meta_parameter
 : public meta_variable
{
private:
	int _position;
public:
	/// Constructor erasing the type of a MetaParameter
	template <typename MetaParameter>
	meta_parameter(MetaParameter mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_variable(mo)
	 , _position(MetaParameter::position::value)
	{ }

	/// Returns the position of the parameter
	int position(void) const
	{
		return _position;
	}
};

/// Class erasing the type of Mirror's MetaFunctions
/**
 *  @see mirror::MetaFunction
 *  @ingroup rubber_type_erased
 */
class meta_function
 : public meta_scope
{
private:
	aux::return_value<meta_type>::type (*_get_result_type)(void);
	range_holder<meta_parameter> _parameters;
public:
	/// Constructor erasing the type of a MetaFunction
	template <typename MetaFunction>
	meta_function(MetaFunction mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_scope(mo)
	 , _get_result_type(
		&wrap_shaped<
			typename MetaFunction::result_type,
			meta_type
		>
	), _parameters(mirror::parameters<MetaFunction>())
	{ }

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns a meta_type reflecting the result type of the function
	meta_type result_type(void) const;
#else
	inline aux::return_value<meta_type>::type result_type(void) const;
#endif

	/// Returns the parameters of the function
	range<meta_parameter> parameters(void) const
	{
		return _parameters.all();
	}
};

/// Class erasing the type of Mirror's MetaConstructors
/**
 *  @see mirror::MetaConstructor
 *  @ingroup rubber_type_erased
 */
class meta_constructor
 : public meta_function
 , public meta_class_member
{
private:
public:
	/// Constructor erasing the type of a MetaConstructor
	template <typename MetaConstructor>
	meta_constructor(MetaConstructor mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_function(mo)
	 , meta_class_member(mo)
	{ }
};

/// Class erasing the type of Mirror's MetaOverloadedFunctions
/**
 *  @see mirror::MetaOverloadedFunctions
 *  @ingroup rubber_type_erased
 */
class meta_overloaded_functions
 : public meta_named_scoped_object
{
private:
	range_holder<meta_function> _overloads;
public:
	/// Constructor erasing the type of a MetaOverloadedFunctions
	template <typename MetaOverloadedFunctions>
	meta_overloaded_functions(MetaOverloadedFunctions mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , _overloads(mirror::overloads<MetaOverloadedFunctions>())
	{ }

	/// Returns the indvidual overloads in the set overloaded functions
	range<meta_function> overloads(void) const
	{
		return _overloads.all();
	}
};

/// Class erasing the type of Mirror's MetaMemberFunctions
/**
 *  @see mirror::MetaMemberFunction
 *  @ingroup rubber_type_erased
 */
class meta_member_function
 : public meta_function
 , public meta_class_member
{
private:
	specifier _storage_class;
	specifier _constness;
public:
	/// Constructor erasing the type of a MetaMemberFunction
	template <typename MetaMemberFunction>
	meta_member_function(MetaMemberFunction mo)
	 : meta_object(mo)
	 , meta_named_scoped_object(mo)
	 , meta_function(mo)
	 , meta_class_member(mo)
	 , _storage_class(typename MetaMemberFunction::storage_class())
	 , _constness(typename MetaMemberFunction::constness())
	{ }

	/// Returns the storage class specifier of the reflected function
	const specifier& storage_class(void) const
	{
		return _storage_class;
	}

	/// Returns the linkage specifier of the reflected member function
	const specifier& linkage(void) const
	{
		return _storage_class;
	}

	/// Returns the constness class specifier of the reflected function
	const specifier& constness(void) const
	{
		return _constness;
	}
};

/// Class erasing the type of Mirror's MetaTraversals
/**
 *  @see mirror::MetaTraversal
 *  @ingroup rubber_type_erased
 */
class meta_traversal
 : public meta_named_object
{
private:
public:
	/// Constructor erasing the type of a MetaTraversal
	template <typename MetaTraversal>
	meta_traversal(MetaTraversal mo)
	 : meta_object(mo)
	 , meta_named_object(mo)
	{ }

	// TODO signature + start
};

/// Class erasing the type of Mirror's MetaLocators
/**
 *  @see mirror::MetaLocator
 *  @ingroup rubber_type_erased
 */
class meta_locator
 : public meta_named_object
{
private:
public:
	/// Constructor erasing the type of a MetaLocator
	template <typename MetaLocator>
	meta_locator(MetaLocator mo)
	 : meta_object(mo)
	 , meta_named_object(mo)
	{ }

	// TODO signature + go_to
};

/// Class erasing the type of Mirror's MetaInserters
/**
 *  @see mirror::MetaInserter
 *  @ingroup rubber_type_erased
 */
class meta_inserter
 : public meta_named_object
{
private:
public:
	/// Constructor erasing the type of a MetaInserter
	template <typename MetaInserter>
	meta_inserter(MetaInserter mo)
	 : meta_object(mo)
	 , meta_named_object(mo)
	{ }

	// TODO signature + insert
};

/// Class erasing the type of Mirror's MetaErasers
/**
 *  @see mirror::MetaEraser
 *  @ingroup rubber_type_erased
 */
class meta_eraser
 : public meta_named_object
{
private:
public:
	/// Constructor erasing the type of a MetaEraser
	template <typename MetaEraser>
	meta_eraser(MetaEraser mo)
	 : meta_object(mo)
	 , meta_named_object(mo)
	{ }

	// TODO signature + erase
};

/// Class erasing the type of Mirror's MetaContainers
/**
 *  @see mirror::MetaContainer
 *  @ingroup rubber_type_erased
 */
class meta_container
 : public meta_scoped_object
{
private:
	aux::return_value<meta_type>::type (*_get_element_type)(void);

	aux::return_value<meta_traversal>::type (*_get_default_traversal)(void);

	range_holder<meta_traversal> _traversals;
	range_holder<meta_locator> _locators;
	range_holder<meta_inserter> _inserters;
	range_holder<meta_eraser> _erasers;
public:
	/// Constructor erasing the type of a MetaContainer
	template <typename MetaContainer>
	meta_container(MetaContainer mo)
	 : meta_object(mo)
	 , meta_scoped_object(mo)
	 , _get_element_type(
		&wrap_shaped<
			typename MetaContainer::element_type,
			meta_type
		>
	), _get_default_traversal(
		&wrap_shaped<
			typename MetaContainer::default_traversal,
			meta_traversal
		>
	), _traversals(mirror::traversals<MetaContainer>())
	 , _locators(mirror::locators<MetaContainer>())
	 , _inserters(mirror::inserters<MetaContainer>())
	 , _erasers(mirror::erasers<MetaContainer>())
	{ }

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns meta-type reflecting the element type of the container
	meta_type element_type(void) const;
#else
	inline aux::return_value<meta_type>::type element_type(void) const;
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
	/// Returns meta-type reflecting the default traversal method
	meta_traversal default_traversal(void) const;
#else
	aux::return_value<meta_traversal>::type default_traversal(void) const
	{
		assert(_get_default_traversal != nullptr);
		return _get_default_traversal();
	}
#endif

	/// Returns the element traversal methods of the container
	range<meta_traversal> traversals(void) const
	{
		return _traversals.all();
	}

	/// Returns the element location methods of the container
	range<meta_locator> locators(void) const
	{
		return _locators.all();
	}

	/// Returns the element insertion methods of the container
	range<meta_inserter> inserters(void) const
	{
		return _inserters.all();
	}

	/// Returns the element removal methods of the container
	range<meta_eraser> erasers(void) const
	{
		return _erasers.all();
	}
};

aux::return_value<meta_scope>::type
meta_scoped_object::scope(void) const
{
	assert(_get_scope != nullptr);
	return _get_scope();
}


aux::return_value<meta_type>::type
meta_typedef::type(void) const
{
	assert(_get_type != nullptr);
	return _get_type();
}

aux::return_value<meta_type_template>::type
meta_templated_type::type_template(void) const
{
	assert(_get_type_template != nullptr);
	return _get_type_template();
}

aux::return_value<meta_class>::type
meta_inheritance::base_class(void) const
{
	assert(_get_base_class != nullptr);
	return _get_base_class();
}

aux::return_value<meta_class>::type
meta_inheritance::derived_class(void) const
{
	assert(_get_derived_class != nullptr);
	return _get_derived_class();
}

aux::return_value<meta_type>::type
meta_variable::type(void) const
{
	assert(_get_type != nullptr);
	return _get_type();
}

aux::return_value<meta_type>::type
meta_function::result_type(void) const
{
	assert(_get_result_type != nullptr);
	return _get_result_type();
}

aux::return_value<meta_type>::type
meta_container::element_type(void) const
{
	assert(_get_element_type != nullptr);
	return _get_element_type();
}

RUBBER_NAMESPACE_END

#endif //include guard

