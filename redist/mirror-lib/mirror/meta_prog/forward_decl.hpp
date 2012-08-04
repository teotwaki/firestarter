/**
 * @file mirror/meta_prog/forward_decl.hpp
 * @brief Forward declaration of the meta-programming utilities
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_FORWARD_DECL_1011291729_HPP
#define MIRROR_META_PROG_FORWARD_DECL_1011291729_HPP


#include <type_traits>

MIRROR_NAMESPACE_BEGIN
namespace mp {

struct nil_t;

template <typename T>
struct identity;

template <typename BooleanConstant>
struct not_;

template <typename ... BooleanConstants>
struct or_;

template <typename ... BooleanConstants>
struct and_;

template <class Bool, class IfTrue, class IfFalse>
struct if_;

template <typename Range, typename Item>
struct push_back;

template <typename T>
struct optional;

template <typename Optional>
struct get;

template <typename Optional>
struct nil;

template <typename ... P>
struct range;

template <typename Range>
struct empty;

template <typename Range>
struct size;

template <typename Range>
struct front;

template <typename Range>
struct back;

template <typename Range, int Index>
struct at_c;

template <typename Range, class Index>
struct at;

template <typename Range>
struct step_front;

template <
	typename Range,
	typename Initial,
	class ForwardOp
> struct fold;

template <typename Range, typename T>
struct find;

template <typename Range, typename T>
struct contains;

template <typename Range, typename T>
struct lacks;

template <class ... Ranges>
struct concat;

template <typename T, class Range>
struct prepend;

template <class Range, typename T>
struct append;

template <class Range, class UnaryMetaFnClass>
struct transform;

template <class Range, class PredicateMetaFnClass>
struct only_if;

template <class Range>
struct unique;

template <class Expr>
struct protect;

template <class MetaObject, class Concept>
struct is_a;

template <class MetaObject, class Concepts>
struct is_one_of;

template <class MetaObject, class Concept>
struct as_a;

template <typename Range, typename Functor>
void for_each(Functor func);

template <typename Range, typename Functor>
void for_each_ii(Functor func);

template <
	typename Range,
	typename ResultType,
	typename Functor,
	typename ValueType
> ResultType accumulate(Functor func, ValueType initial);

template <
	typename Range,
	typename ResultType,
	typename Functor,
	typename ValueType
> ResultType accumulate_ii(Functor func, ValueType initial);

template <typename T>
struct is_pack_nil_type;

template <typename Range>
struct remove_nil_types;

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

