/**
 * .file puddle/auxiliary/meta_objects.hpp
 * .brief The Mirror meta-object wrappers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUDDLE_AUXILIARY_META_OBJECTS_1103151318_HPP
#define PUDDLE_AUXILIARY_META_OBJECTS_1103151318_HPP

#include <puddle/auxiliary/fwd.hpp>
#include <puddle/auxiliary/meta_object.hpp>
#include <puddle/auxiliary/specifier.hpp>
#include <puddle/auxiliary/ct_string.hpp>
#include <puddle/auxiliary/container_kind.hpp>
#include <puddle/auxiliary/by_name.hpp>
#include <mirror/class_gen.hpp>
#include <string>
#include <cstddef>

PUDDLE_NAMESPACE_BEGIN
namespace aux {

template <typename MetaNamedObject>
struct meta_named_object
 : virtual meta_object<MetaNamedObject>
{
	static std::string base_name(void)
	{
		return MetaNamedObject::base_name();
	}

	static ct_string_<
		typename MetaNamedObject::static_name
	> static_name(void)
	{
		return ct_string_<
			typename MetaNamedObject::static_name
		>();
	}
};

template <typename MetaMetaObject>
struct meta_meta_object
 : meta_named_object<MetaMetaObject>
{
	static std::string construct_name(void)
	{
		return MetaMetaObject::construct_name();
	}

	static range<
		typename mirror::generalizations<MetaMetaObject>::type
	> generalizations(void)
	{
		return range<
			typename mirror::generalizations<
				MetaMetaObject
			>::type
		>();
	}

	// TODO: categorization / is_X
};

template <typename MetaScopedObject>
struct meta_scoped_object
 : virtual meta_object<MetaScopedObject>
{
	static typename wrap<
		typename MetaScopedObject::scope
	>::type scope(void)
	{
		return typename wrap<
			typename MetaScopedObject::scope
		>::type();
	}
};

template <typename MetaNamedScopedObject>
struct meta_named_scoped_object
 : meta_named_object<MetaNamedScopedObject>
 , meta_scoped_object<MetaNamedScopedObject>
{
	static std::string full_name(void)
	{
		return MetaNamedScopedObject::full_name();
	}

	static std::string local_name(void)
	{
		return MetaNamedScopedObject::local_name();
	}
};

template <typename MetaScope>
struct meta_scope
 : meta_named_scoped_object<MetaScope>
{
	static range<
		typename mirror::members<MetaScope>::type
	> members(void)
	{
		return range<
			typename mirror::members<MetaScope>::type
		>();
	}
};

template <typename MetaUnspecifiedScope>
struct meta_unspecified_scope
 : meta_scope<MetaUnspecifiedScope>
{
};

template <typename MetaNamespace>
struct meta_namespace
 : meta_scope<MetaNamespace>
{
	static range<
		typename mirror::namespaces<MetaNamespace>::type
	> namespaces(void)
	{
		return range<
			typename mirror::namespaces<
				MetaNamespace
			>::type
		>();
	}

	static range<
		typename mirror::free_variables<MetaNamespace>::type
	> free_variables(void)
	{
		return range<
			typename mirror::free_variables<
				MetaNamespace
			>::type
		>();
	}

	static typename mirror::scope_class_generator<
		MetaNamespace,
		mirror::free_variables,
		by_name_scope_transf,
		by_name_memvar_transf
	>::type free_variables_by_name(void)
	{
		return typename mirror::scope_class_generator<
			MetaNamespace,
			mirror::free_variables,
			by_name_scope_transf,
			by_name_memvar_transf
		>::type (0);
	}

	typename mirror::scope_class_generator<
		MetaNamespace,
		mirror::free_variables,
		by_name_scope_transf,
		by_name_memvar_transf
	>::type* operator -> (void) const
	{
		static typename mirror::scope_class_generator<
			MetaNamespace,
			mirror::free_variables,
			by_name_scope_transf,
			by_name_memvar_transf
		>::type x(0);
		return &x;
	}

	static range<
		typename mirror::free_functions<MetaNamespace>::type
	> free_functions(void)
	{
		return range<
			typename mirror::free_functions<
				MetaNamespace
			>::type
		>();
	}
};

template <typename MetaGlobalScope>
struct meta_global_scope
 : meta_namespace<MetaGlobalScope>
{
};

template <typename T>
struct trait_applier
{
	typedef typename wrap<
		typename mirror::reflected_type<T>::type
	>::type type;
};

template <typename Int, Int Value>
struct trait_applier<std::integral_constant<Int, Value> >
{
	typedef integer<
		std::integral_constant<Int, Value>
	> type;
};

template <typename MetaType>
struct meta_type
 : meta_named_scoped_object<MetaType>
{
private:
	typedef typename MetaType::original_type orig_type;
public:
	static orig_type original_type(void);

	static range<
		typename mirror::constructors<MetaType>::type
	> constructors(void)
	{
		return range<
			typename mirror::constructors<
				MetaType
			>::type
		>();
	}

	static container_kind_tag<
		typename mirror::container_kind<MetaType>::type
	> container_kind(void)
	{
		return container_kind_tag<
			typename mirror::container_kind<
				MetaType
			>::type
		>();
	}

	static range<
		typename mirror::containers<MetaType>::type
	> containers(void)
	{
		return range<
			typename mirror::containers<
				MetaType
			>::type
		>();
	}

	template <typename T, typename ElemFunctor, typename EmptyFunctor>
	int traverse_elements(
		T& inst,
		ElemFunctor elem_func,
		EmptyFunctor empty_func
	)
	{
		return mirror::traverse_elements(
			MetaType(),
			inst,
			elem_func,
			empty_func
		);
	}

	template <template <typename> class UnaryTrait>
	static auto apply(void) ->
	typename trait_applier<typename UnaryTrait<orig_type>::type>::type
	{
		return typename trait_applier<
			typename UnaryTrait<orig_type>::type
		>::type();
	}

};

template <typename MetaTypedef>
struct meta_typedef
 : meta_type<MetaTypedef>
{
	static typename wrap<
		typename MetaTypedef::type
	>::type type(void)
	{
		return typename wrap<
			typename MetaTypedef::type
		>::type();
	}
};

template <typename MetaTemplatedType>
struct meta_templated_type
 : meta_type<MetaTemplatedType>
{
	static typename wrap<
		typename MetaTemplatedType::type_template
	>::type type_template(void)
	{
		return typename wrap<
			typename MetaTemplatedType::type_template
		>::type();
	}

	static range<
		typename mirror::template_parameters<
			MetaTemplatedType
		>::type
	> template_parameters(void)
	{
		return range<
			typename mirror::template_parameters<
				MetaTemplatedType
			>::type
		>();
	}
};

template <typename MetaTypeTemplate>
struct meta_type_template
 : meta_named_scoped_object<MetaTypeTemplate>
{
	//TODO:
};

template <typename MetaClass>
struct meta_class
 : meta_type<MetaClass>
 , meta_scope<MetaClass>
{
	static specifier<typename MetaClass::elaborated_type>
	elaborated_type(void)
	{
		return specifier<typename MetaClass::elaborated_type>();
	}

	static range<
		typename mirror::base_classes<MetaClass>::type
	> base_classes(void)
	{
		return range<
			typename mirror::base_classes<MetaClass>::type
		>();
	}

	static range<
		typename mirror::member_variables<MetaClass>::type
	> member_variables(void)
	{
		return range<
			typename mirror::member_variables<
				MetaClass
			>::type
		>();
	}

	static range<
		typename mirror::all_member_variables<MetaClass>::type
	> all_member_variables(void)
	{
		return range<
			typename mirror::all_member_variables<
				MetaClass
			>::type
		>();
	}

	static typename mirror::class_generator<
		typename MetaClass::original_type,
		by_name_scope_transf,
		by_name_memvar_transf
	>::type member_variables_by_name(void)
	{
		return typename mirror::class_generator<
			typename MetaClass::original_type,
			by_name_scope_transf,
			by_name_memvar_transf
		>::type(0);
	}

	typename mirror::class_generator<
		typename MetaClass::original_type,
		by_name_scope_transf,
		by_name_memvar_transf
	>::type* operator -> (void) const
	{
		static typename mirror::class_generator<
			typename MetaClass::original_type,
			by_name_scope_transf,
			by_name_memvar_transf
		>::type x(0);
		return &x;
	}

	static range<
		typename mirror::all_containers<MetaClass>::type
	> all_containers(void)
	{
		return range<
			typename mirror::all_containers<
				MetaClass
			>::type
		>();
	}
};

template <typename MetaClassMember>
struct meta_class_member
 : meta_named_scoped_object<MetaClassMember>
{
	static specifier<typename MetaClassMember::access_type>
	access_type(void)
	{
		return specifier<typename MetaClassMember::access_type>();
	}
};

template <typename MetaTemplatedClass>
struct meta_templated_class
 : meta_templated_type<MetaTemplatedClass>
 , meta_class<MetaTemplatedClass>
{
};

template <typename MetaEnum>
struct meta_enum
 : meta_type<MetaEnum>
 , meta_scope<MetaEnum>
{
	static integer<
		typename MetaEnum::size
	> size(void)
	{
		return integer<
			typename MetaEnum::size
		>();
	}

	static range<
		typename mirror::enum_values<MetaEnum>::type
	> enum_values(void)
	{
		return range<
			typename mirror::enum_values<
				MetaEnum
			>::type
		>();
	}

	template <typename I, I Index>
	static auto value(integer<std::integral_constant<I, Index> > i) ->
	decltype(MetaEnum::value(i.static_value()))
	{
		return MetaEnum::value(i.static_value());
	}

	static auto value(int index) ->
	decltype(MetaEnum::value(index))
	{
		return MetaEnum::value(index);
	}

	template <typename I, I Index>
	static std::string value(integer<std::integral_constant<I, Index> > i)
	{
		return MetaEnum::value_name(i.static_value());
	}

	static std::string value_name(int index)
	{
		return MetaEnum::value_name(index);
	}

	static std::string name_by_value(typename MetaEnum::original_type val)
	{
		return MetaEnum::name_by_value(val);
	}

	// TODO:
};

template <typename MetaEnumValue>
struct meta_enum_value
 : meta_named_scoped_object<MetaEnumValue>
{
	static integer<
		typename MetaEnumValue::value
	> value(void)
	{
		return integer<
			typename MetaEnumValue::value
		>();
	}
};

template <typename MetaInheritance>
struct meta_inheritance
 : virtual meta_object<MetaInheritance>
{
	static typename wrap<
		typename MetaInheritance::derived_class
	>::type derived_class(void)
	{
		return typename wrap<
			typename MetaInheritance::derived_class
		>::type();
	}

	static specifier<typename MetaInheritance::inheritance_type>
	inheritance_type(void)
	{
		return specifier<typename MetaInheritance::inheritance_type>();
	}

	static specifier<typename MetaInheritance::access_type>
	access_type(void)
	{
		return specifier<typename MetaInheritance::access_type>();
	}

	static typename wrap<
		typename MetaInheritance::base_class
	>::type base_class(void)
	{
		return typename wrap<
			typename MetaInheritance::base_class
		>::type();
	}
};

template <typename MetaVariable>
struct meta_variable
 : meta_named_scoped_object<MetaVariable>
{

	static specifier<typename MetaVariable::storage_class>
	storage_class(void)
	{
		return specifier<typename MetaVariable::storage_class>();
	}

	static typename wrap<
		typename MetaVariable::type
	>::type type(void)
	{
		return typename wrap<
			typename MetaVariable::type
		>::type();
	}
};

template <typename MetaFreeVariable>
struct meta_free_variable
 : meta_variable<MetaFreeVariable>
{
	static auto get(void) -> decltype(MetaFreeVariable::get())
	{
		return MetaFreeVariable::get();
	}

	template <typename T>
	static void set(T val)
	{
		return MetaFreeVariable::set(val);
	}
};

template <typename MetaPlainVariable>
struct meta_plain_variable
 : meta_variable<MetaPlainVariable>
{
};

template <typename MetaPlainFreeVariable>
struct meta_plain_free_variable
 : meta_free_variable<MetaPlainFreeVariable>
 , meta_plain_variable<MetaPlainFreeVariable>
{
	static auto address(void) -> decltype(MetaPlainFreeVariable::address())
	{
		return MetaPlainFreeVariable::address();
	}
};

template <typename MetaMemberVariable>
struct meta_member_variable
 : meta_class_member<MetaMemberVariable>
 , meta_variable<MetaMemberVariable>
{
private:
	typedef typename MetaMemberVariable::scope::original_type _class;
public:
	static auto get(const _class& inst) ->
	decltype(MetaMemberVariable::get(inst))
	{
		return MetaMemberVariable::get(inst);
	}

	template <typename Type>
	static void set(_class& inst, Type new_value)
	{
		MetaMemberVariable::set(inst, new_value);
	}
};

template <typename MetaPlainMemberVariable>
struct meta_plain_member_variable
 : meta_member_variable<MetaPlainMemberVariable>
 , meta_plain_variable<MetaPlainMemberVariable>
{
private:
	typedef typename MetaPlainMemberVariable::scope::original_type _class;
public:
	static std::ptrdiff_t offset(void)
	{
		return MetaPlainMemberVariable::offset();
	}

	static auto address(_class& inst) ->
	decltype(MetaPlainMemberVariable::address(inst))
	{
		return MetaPlainMemberVariable::address(inst);
	}

	static auto address(const _class& inst) ->
	decltype(MetaPlainMemberVariable::address(inst))
	{
		return MetaPlainMemberVariable::address(inst);
	}

	static auto member_pointer(void) ->
	decltype(MetaPlainMemberVariable::member_pointer())
	{
		return MetaPlainMemberVariable::member_pointer();
	}
};

template <typename MetaParameter>
struct meta_parameter
 : meta_variable<MetaParameter>
{
	static integer<
		typename MetaParameter::position
	> position(void)
	{
		return integer<
			typename MetaParameter::position
		>();
	}
};

template <typename MetaFunction>
struct meta_function
 : meta_scope<MetaFunction>
{

	static typename wrap<
		typename MetaFunction::result_type
	>::type result_type(void)
	{
		return typename wrap<
			typename MetaFunction::result_type
		>::type();
	}

	static specifier<typename MetaFunction::storage_class>
	storage_class(void)
	{
		return specifier<typename MetaFunction::storage_class>();
	}

	static specifier<typename MetaFunction::linkage>
	linkage(void)
	{
		return specifier<typename MetaFunction::linkage>();
	}

	static range<
		typename mirror::parameters<MetaFunction>::type
	> parameters(void)
	{
		return range<
			typename mirror::parameters<
				MetaFunction
			>::type
		>();
	}
};

template <typename MetaMemberFunction>
struct meta_member_function
 : meta_class_member<MetaMemberFunction>
 , meta_function<MetaMemberFunction>
{
	static specifier<typename MetaMemberFunction::constness>
	constness(void)
	{
		return specifier<typename MetaMemberFunction::constness>();
	}
};

template <typename MetaOverloadedFunctions>
struct meta_overloaded_functions
 : meta_named_scoped_object<MetaOverloadedFunctions>
{
	static range<
		typename mirror::overloads<MetaOverloadedFunctions>::type
	> overloads(void)
	{
		return range<
			typename mirror::overloads<
				MetaOverloadedFunctions
			>::type
		>();
	}
};

template <typename MetaConversionOperator>
struct meta_conversion_operator
 : meta_member_function<MetaConversionOperator>
{
	//TODO:
};

template <typename MetaTraversal>
struct meta_traversal
 : meta_named_object<MetaTraversal>
{
	static auto signature(void) ->
	decltype(typename MetaTraversal::signature())
	{
		return typename MetaTraversal::signature();
	}

	template <class Container>
	static auto start(
		const Container& container,
		const typename MetaTraversal::_param_tuple& params
	) -> decltype(MetaTraversal::start(container, params))
	{
		return MetaTraversal::start(container, params);
	}

	template <class Container>
	static auto start(
		Container& container,
		const typename MetaTraversal::_param_tuple& params
	) -> decltype(MetaTraversal::start(container, params))
	{
		return MetaTraversal::start(container, params);
	}
};

template <typename MetaLocator>
struct meta_locator
 : meta_named_object<MetaLocator>
{
	static auto signature(void) ->
	decltype(typename MetaLocator::signature())
	{
		return typename MetaLocator::signature();
	}

	template <class Container>
	static auto go_to(
		Container& container,
		const typename MetaLocator::_param_tuple& params
	) -> decltype(MetaLocator::go_to(container, params))
	{
		return MetaLocator::go_to(container, params);
	}
};

template <typename MetaInserter>
struct meta_inserter
 : meta_named_object<MetaInserter>
{
	static auto signature(void) ->
	decltype(typename MetaInserter::signature())
	{
		return typename MetaInserter::signature();
	}

	template <class Container>
	static void insert(
		Container& container,
		const typename MetaInserter::_param_tuple& params
	)
	{
		MetaInserter::insert(container, params);
	}
};

template <typename MetaEraser>
struct meta_eraser
 : meta_named_object<MetaEraser>
{
	static auto signature(void) ->
	decltype(typename MetaEraser::signature())
	{
		return typename MetaEraser::signature();
	}

	template <class Container>
	static void erase(
		Container& container,
		const typename MetaEraser::_param_tuple& params
	)
	{
		MetaEraser::erase(container, params);
	}
};

template <typename MetaContainer>
struct meta_container
 : meta_scoped_object<MetaContainer>
{
	static typename wrap<
		typename MetaContainer::element_type
	>::type element_type(void)
	{
		return typename wrap<
			typename MetaContainer::element_type
		>::type();
	}

	static typename wrap<
		typename MetaContainer::default_traversal
	>::type default_traversal(void)
	{
		return typename wrap<
			typename MetaContainer::default_traversal
		>::type();
	}

	static range<
		typename mirror::traversals<MetaContainer>::type
	> traversals(void)
	{
		return range<
			typename mirror::traversals<
				MetaContainer
			>::type
		>();
	}

	static range<
		typename mirror::locators<MetaContainer>::type
	> locators(void)
	{
		return range<
			typename mirror::locators<
				MetaContainer
			>::type
		>();
	}

	static range<
		typename mirror::inserters<MetaContainer>::type
	> inserters(void)
	{
		return range<
			typename mirror::inserters<
				MetaContainer
			>::type
		>();
	}

	static range<
		typename mirror::erasers<MetaContainer>::type
	> erasers(void)
	{
		return range<
			typename mirror::erasers<
				MetaContainer
			>::type
		>();
	}
};

} // namespace aux
PUDDLE_NAMESPACE_END

#endif //include guard

