/**
 * @file mirror/meta_prog/apply_on_seq_pack.hpp
 * @brief Meta function calling the apply metafunction of a meta-function
 * class with a range of integer params of 0,1, ..., N-1.
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_META_PROG_APPLY_ON_SEQ_PACK_1011291729_HPP
#define MIRROR_META_PROG_APPLY_ON_SEQ_PACK_1011291729_HPP


#include <mirror/meta_prog/range.hpp>

MIRROR_NAMESPACE_BEGIN
namespace mp {
namespace aux {

//  Helper which calls the apply meta-function and "returns" its type
/*  This template takes the pack of integral constant types with
 *  values forming a sequence 0, 1, 2, ..., N-1 and defines a do_apply
 *  template that takes a meta-function class.
 */
template <int N, class ... I>
struct seq_pack_applier
{
	template <class MetaFunctionClass>
	struct do_apply
	{
		typedef typename MetaFunctionClass::
		template apply<
			(N - I::value - 1)...
		>::type type;
	};
};

//  Forward declaration the seq_pack helper class
/*  This template class is used for expanding
 *  the parameter pack containing the sequence
 *  of integral constant types from 0 to the
 *  value specified as the first template parameter.
 *  When the parameter pack is fully expanded
 *  the seq_packr_applier template is used to instantiate
 *  the apply meta-function of the custom meta function
 *  class.
 */
template <class N, class ...I>
struct seq_pack_helper;

/*  Specialization of the seq_pack_helper for single
 *  element packs.
 */
template <>
struct seq_pack_helper<
	std::integral_constant<int, 1>,
	std::integral_constant<int, 0>
> : seq_pack_applier<
	1,
	std::integral_constant<int, 0>
>
{ };

/*  Specialization of the seq_pack_helper with fully
 *  assembled integral constant type pack. This
 *  version derives from the seq_pack_applier providing
 *  the do_apply meta-function
 */
template <
	int N,
	class ...I
> struct seq_pack_helper<
	std::integral_constant<int, N>,
	std::integral_constant<int, N>,
	I...
> : seq_pack_applier<N, I...>
{ };

/*  This specialization of the seq_pack_helper which
 *  adds one item to the integral constant type pack
 *  which is not yet fully assembled.
 */
template <
	int N,
	int I_plus_1,
	class ... I
> struct seq_pack_helper<
	std::integral_constant<int, N>,
	std::integral_constant<int, I_plus_1>,
	I...
> : seq_pack_helper<
	std::integral_constant<int, N>,
	std::integral_constant<int, I_plus_1 + 1>,
	std::integral_constant<int, I_plus_1>,
	I...
>
{ };

} // namespace aux

#ifdef MIRROR_DOCUMENTATION_ONLY
/// Calls a nested meta-function with a pack <0,1,2, ... N-1> of ints
/** This template assembles a pack of integral non-type template parameters
 *  which form a sequence of 0, 1, 2, ... , N-1 and calls the meta-function
 *  called apply nested in the MetaFunctionClass, passing the pack as
 *  parameters.
 *
 *  @tparam MetaFunctionClass a class containing a nested template called
 *  apply which can take @a N integral template parameters having
 *  typedef called "type".
 *  @param N the count of arguments for the meta-function.
 *
 *  @see apply_on_seq_pack
 *  @ingroup meta_programming
 */
template <typename MetaFunctionClass, int N>
struct apply_on_seq_pack_c
{
	/// The result of the meta-function with the <0,1,2,...,N-1> params
	/** This type is the result of the following expression
	 *  @verbatim
	 *  typename MetaFunctionClass:: template apply<
	 *      0, 1, 2, ..., N-1
	 *  >::type
	 *  @endverbatim
	 */
	typedef unspecified_type type;
};
#else
template <typename MetaFunctionClass, int Count>
struct apply_on_seq_pack_c
{
	typedef typename aux::seq_pack_helper<
		std::integral_constant<int, Count>,
		std::integral_constant<int, 0>
	>::template do_apply<MetaFunctionClass>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 0>
{
	typedef typename MetaFunctionClass::
	template apply< >::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 1>
{
	typedef typename MetaFunctionClass::
	template apply<0>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 2>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 3>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 4>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 5>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 6>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4, 5>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 7>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4, 5, 6>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 8>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4, 5, 6, 7>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 9>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4, 5, 6, 7, 8>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 10>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 11>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 12>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 13>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>::type type;
};

template <typename MetaFunctionClass>
struct apply_on_seq_pack_c<MetaFunctionClass, 14>
{
	typedef typename MetaFunctionClass::
	template apply<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13>::type type;
};

#endif


#ifdef MIRROR_DOCUMENTATION_ONLY
/// Calls a nested meta-function with a pack <0,1,2, ... N-1> of ints
/** This template assembles a pack of integral non-type template parameters
 *  which form a sequence of 0, 1, 2, ... , N-1 and calls the meta-function
 *  called apply nested in the MetaFunctionClass, passing the pack as
 *  parameters.
 *
 *  @tparam MetaFunctionClass a class containing a nested template called
 *  apply which can take @a N integral template parameters having
 *  typedef called "type".
 *  @param N an integral constant type providing the count of arguments for
 *  the meta-function.
 *
 *  @see apply_on_seq_pack
 *  @ingroup meta_programming
 */
template <typename MetaFunctionClass, class N>
struct apply_on_seq_pack
{
	/// The result of the meta-function with the <0,1,2,...,N-1> params
	/** This type is the result of the following expression
	 *  @verbatim
	 *  typename MetaFunctionClass:: template apply<
	 *      0, 1, 2, ..., N-1
	 *  >::type
	 *  @endverbatim
	 */
	typedef unspecified_type type;
};
#else
template <typename MetaFunctionClass, class Count>
struct apply_on_seq_pack
 : public apply_on_seq_pack_c<MetaFunctionClass, Count::value>
{ };
#endif

} // namespace mp
MIRROR_NAMESPACE_END

#endif //include guard

