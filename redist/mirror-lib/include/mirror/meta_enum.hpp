/**
 * @file mirror/meta_enum.hpp
 * @brief Implementation of enum registering and reflection
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_ENUM_1101121701_HPP
#define MIRROR_META_ENUM_1101121701_HPP

#include <mirror/meta_namespace.hpp>
#include <mirror/meta_type.hpp>
#include <mirror/specifier_tags.hpp>
#include <mirror/auxiliary/enum_values.hpp>
#include <mirror/meta_prog/range.hpp>
#include <mirror/meta_prog/apply_on_seq_pack.hpp>
#include <mirror/meta_prog/make_int_seq.hpp>
#include <vector>

MIRROR_NAMESPACE_BEGIN

#define MIRROR_REG_GLOBAL_SCOPE_ENUM_BEGIN_CTS(NAME, SPELLED) \
MIRROR_REG_GLOBAL_SCOPE_TYPE_BASICS(enum, NAME, SPELLED) \
MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace :: _, \
	mirror::meta_enum< NAME > \
) \
namespace _enum { \
template <> \
struct _< NAME > \
{ \
	typedef NAME _this_enum; \
	typedef std::integral_constant<int, 0>

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro which begins the registering of a global scope enum
/**
 *  @param ENUM name of the enumeration type
 *
 *  @see MIRROR_REG_ENUM_BEGIN
 *  @see MIRROR_REG_ENUM_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_GLOBAL_SCOPE_ENUM_BEGIN(ENUM)
#else
#define MIRROR_REG_GLOBAL_SCOPE_ENUM_BEGIN(ENUM) \
	MIRROR_REG_GLOBAL_SCOPE_ENUM_BEGIN_CTS(ENUM, ())
#endif

#define MIRROR_REG_ENUM_BEGIN_CTS(NAMESPACE, NAME, SPELLED) \
MIRROR_REG_TYPE_BASICS(enum, NAMESPACE, NAME, SPELLED) \
MIRROR_ADD_TO_GLOBAL_LIST( \
	mirror::_namespace :: NAMESPACE :: _, \
	mirror::meta_enum< :: NAMESPACE :: NAME > \
) \
namespace _enum { \
template <> \
struct _< :: NAMESPACE :: NAME > \
{ \
	typedef :: NAMESPACE :: NAME _this_enum; \
	typedef std::integral_constant<int, 0>

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro which begins the registering of an enum nested in a namespace
/**
 *  @param NAMESPACE full namespace name inside of which the enum is defined
 *  @param ENUM name of the enumeration type
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_ENUM_BEGIN
 *  @see MIRROR_REG_ENUM_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_ENUM_BEGIN(NAMESPACE, ENUM)
#else
#define MIRROR_REG_ENUM_BEGIN(NAMESPACE, ENUM) \
	MIRROR_REG_ENUM_BEGIN_CTS(NAMESPACE, ENUM, ())
#endif

#define MIRROR_REG_NESTED_ENUM_BEGIN_CTS(PARENT_CLASS, NAME, SPELLED) \
MIRROR_REG_NESTED_TYPE_BASICS(enum, PARENT_CLASS, NAME, SPELLED) \
namespace _enum { \
template <> \
struct _< :: PARENT_CLASS :: NAME > \
{ \
	typedef :: PARENT_CLASS :: NAME _this_enum; \
	typedef std::integral_constant<int, 0>

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro which begins the registering of an enum nested in a class
/**
 *  @param PARENT_CLASS full class name inside of which the enum is defined
 *  @param ENUM name of the enumeration type
 *
 *  @see MIRROR_REG_GLOBAL_SCOPE_ENUM_BEGIN
 *  @see MIRROR_REG_ENUM_BEGIN
 *  @see MIRROR_REG_ENUM_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_NESTED_ENUM_BEGIN(PARENT_CLASS, ENUM)
#else
#define MIRROR_REG_NESTED_ENUM_BEGIN(PARENT_CLASS, ENUM) \
	MIRROR_REG_NESTED_ENUM_BEGIN_CTS(PARENT_CLASS, ENUM, ())
#endif

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro which ends the registering on an enumeration
/**
 *  @see MIRROR_REG_GLOBAL_SCOPE_ENUM_BEGIN
 *  @see MIRROR_REG_ENUM_BEGIN
 *  @ingroup registering_macros
 */
#define MIRROR_REG_ENUM_END
#else
#define MIRROR_REG_ENUM_END \
	item_count; \
}; /* struct _enum::<...> */ \
}  /* namespace _enum */
#endif

#define MIRROR_REG_ENUM_VALUE_CTS(ENUM_VALUE, SPELLED) \
	item_ ## ENUM_VALUE; \
	static std::integral_constant<_this_enum, _this_enum::ENUM_VALUE > \
	item_val(item_ ## ENUM_VALUE); \
	static inline _this_enum \
	item_val_c(item_ ## ENUM_VALUE) \
	{ \
		return _this_enum::ENUM_VALUE ;\
	} \
	static const char* item_name(item_ ## ENUM_VALUE) \
	{ \
		return #ENUM_VALUE; \
	} \
	static size_t item_name_length(item_ ## ENUM_VALUE) \
	{ \
		return sizeof(#ENUM_VALUE) - 1; \
	} \
	static typename mirror::cts::string< \
		MIRROR_PP_EXPAND_ARGS SPELLED \
	>::type item_ct_name(item_ ## ENUM_VALUE); \
	typedef std::integral_constant<int, item_ ## ENUM_VALUE :: value + 1>

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Macro which registers an enum value
/**
 *  @see MIRROR_REG_ENUM_BEGIN
 *  @see MIRROR_REG_ENUM_END
 *  @ingroup registering_macros
 */
#define MIRROR_REG_ENUM_VALUE(ENUM_VALUE)
#else
#define MIRROR_REG_ENUM_VALUE(ENUM_VALUE) \
	MIRROR_REG_ENUM_VALUE_CTS(ENUM_VALUE, ())
#endif

template <class Enum>
struct meta_enum
 : public meta_type<Enum>
{
public:
	typedef typename mirror::_enum::_<Enum>::item_count size;

	template <int I>
	static Enum value(std::integral_constant<int, I> index)
	{
		return mirror::_enum::_<Enum>::item_val_c(index);
	}

	template <int I>
	static std::string value_name(std::integral_constant<int, I> index)
	{
		return std::string(
			mirror::_enum::_<Enum>::item_name(index),
			mirror::_enum::_<Enum>::item_name_length(index)
		);
	}
private:
	template <typename ... Idx>
	static std::vector<Enum> cache_values(mp::range<Idx ...>)
	{
		return {mirror::_enum::_<Enum>::item_val_c(Idx())...};
	}

	template <typename ... Idx>
	static std::vector<const char*> cache_names(mp::range<Idx ...>)
	{
		return {mirror::_enum::_<Enum>::item_name(Idx())...};
	}

	static const std::vector<Enum>& _value_cache(void)
	{
		typename mp::make_index_seq<size>::type indices;
		static auto cache = cache_values(indices);
		return cache;
	}
public:

	//NOTE: this is an implementation detail, do not use in client code
	static const std::vector<const char*>& _name_cache(void)
	{
		typename mp::make_index_seq<size>::type indices;
		static auto cache = cache_names(indices);
		return cache;
	}

	static Enum value(int index)
	{
		assert(index >= 0);
		assert(index < size::value);
		return _value_cache()[index];
	}

	static std::string value_name(int index)
	{
		assert(index >= 0);
		assert(index < size::value);
		return _name_cache()[index];
	}

	static std::string name_by_value(Enum x)
	{
		for(int i=0; i!= size::value; ++i)
			if(value(i) == x)
				return value_name(i);
		return std::string();
	}

	static Enum value_by_name(const std::string& x)
	{
		for(int i=0; i!= size::value; ++i)
			if(value_name(i) == x)
				return value(i);
		return Enum();
	}

	static bool has_value_name(const std::string& x)
	{
		for(int i=0; i!= size::value; ++i)
			if(value_name(i) == x)
				return true;
		return false;
	}
};

template <class Enum, int Index>
struct meta_enum_value
{
public:
	typedef std::integral_constant<int, Index> position;
	typedef meta_enum<Enum> scope;

private:
	template <typename Local>
	static std::string make_name(Local local)
	{
		// get the full or local name of the scope
		std::string result(
			Local::value ?
			scope::local_name() :
			scope::full_name()
		);
		// append the separating double colon
		result.append("::");
		// and append the base name of this member variable
		result.append(base_name());
		// return the result
		return result;
	}
public:
	typedef decltype(mirror::_enum::_<Enum>::item_ct_name(position()))
		static_name;

	// returns the base name of the member variable
	static std::string base_name(void)
	{
		return std::string(
			mirror::_enum::_<Enum>::item_name(position()),
			mirror::_enum::_<Enum>::item_name_length(position())
		);
	}

	// returns the full name (including the full nested name qualifier)
	static std::string full_name(void)
	{
		return make_name(std::false_type());
	}

	// returns the local name (including partial nested name qualifier)
	static std::string local_name(void)
	{
		return make_name(std::true_type());
	}

	typedef decltype(mirror::_enum::_<Enum>::item_val(position())) value;
};

template <class Enum>
struct enum_values<meta_enum<Enum> >
{
	typedef typename mp::apply_on_seq_pack<
		mirror::aux::enum_val_helper<Enum>,
		typename mirror::_enum::_<Enum>::item_count
	>::type type;
};

template <typename Enum>
struct members<meta_enum<Enum> >
{
	typedef typename enum_values<meta_enum<Enum> >::type type;
};

template <class Class>
struct constructors<meta_enum<Class> >
 : constructors<meta_type<Class> >
{ };


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Reflects the class passed as the @a FULL_ENUM_NAME argument
/**
 *  This macro expands into a type conforming to the MetaEnum
 *  concept, which provides meta-data about the given enum.
 *  The enum name passed as the @a FULL_ENUM_NAME argument
 *  should be a fully qualified type name.
 *
 *  @see mirror::MetaEnum
 *  @see mirror::reflected
 *  @see lagoon::reflected_type
 *  @see lagoon::reflected_class
 *
 *  @ingroup reflection_macros
 */
#define MIRRORED_ENUM(FULL_ENUM_NAME) mirror::MetaEnum
#else
#define MIRRORED_ENUM(FULL_ENUM_NAME) \
	mirror::meta_enum< FULL_ENUM_NAME >
#endif

MIRROR_NAMESPACE_END

#endif //include guard

