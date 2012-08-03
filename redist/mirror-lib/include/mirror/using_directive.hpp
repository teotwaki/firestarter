/**
 *  @file mirror/using_directive.hpp
 *  @brief Implementation of the Mirror's using directive utility
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_USING_DIRECTIVE_1011291729_HPP
#define MIRROR_USING_DIRECTIVE_1011291729_HPP

#include <mirror/auxiliary/using_directive.hpp>

MIRROR_NAMESPACE_BEGIN

/// Macro which tells Mirror to remove this nested namespace's name from local names
/** This macro acts in a similar way as the 'using namespace' directive
 *  and causes the local_name member function of MetaNamedScopedObject(s)
 *  to remove the namespace name from the full names of members of this
 *  namespace. This version of the macro must be used for nested namespaces
 *  (i.e. namespaces whose name contains ::).
 *
 *  @param ID an identifier for the using directive, which distinguishes it
 *  from other using directives on the same scope. Generally the same rules
 *  as for the identifier names apply, except that it can start with a digit.
 *
 *  @param NAMESPACE the name of the namespace to be used
 *
 *  @see MetaNamedScopedObject
 *  @see MIRROR_USING_NAMESPACE
 *  @see MIRROR_USING_NAMESPACE_
 *  @ingroup utility_macros
 */
#define MIRROR_USING_NESTED_NAMESPACE(ID, NAMESPACE) \
	const mirror::aux::_using< \
		MIRRORED_NAMESPACE(NAMESPACE) \
	> _boost_mirror_using_ ## ID(__COUNTER__)

/// Macro which tells Mirror to remove this namespace's name from local names
/** This macro acts in a similar way as the 'using namespace' directive
 *  and causes the local_name member function of MetaNamedScopedObject(s)
 *  to remove the namespace name from the full names of members of this
 *  namespace. It can be used only with top-level namespaces. For nested
 *  namespaces see the #MIRROR_USING_NESTED_NAMESPACE() or the
 *  #MIRROR_USING_NAMESPACE_() macro.
 *
 *  @param NAMESPACE the name of the namespace to be used. If it is
 *  a nested namespace (i.e. its name contains ::) then the
 *  #MIRROR_USING_NESTED_NAMESPACE() macro must be used instead.
 *
 *  @see MetaNamedScopedObject
 *  @see MIRROR_USING_NESTED_NAMESPACE
 *  @see MIRROR_USING_NAMESPACE_
 *  @ingroup utility_macros
 */
#define MIRROR_USING_NAMESPACE(NAMESPACE) \
	MIRROR_USING_NESTED_NAMESPACE(NAMESPACE, NAMESPACE)

/// Macro which tells Mirror to remove this namespace's name from local names
/** This macro acts in a similar way as the 'using namespace' directive
 *  and causes the local_name member function of MetaNamedScopedObject(s)
 *  to remove the namespace name from the full names of members of this
 *  namespace. Unlike #MIRROR_USING_NAMESPACE(), this macro can be also used
 *  for nested namespaces, but only one namespace can be 'used' on a particular
 *  scope.
 *
 *  @param NAMESPACE the name of the namespace to be used.
 *
 *  @see MetaNamedScopedObject
 *  @see MIRROR_USING_NESTED_NAMESPACE
 *  @see MIRROR_USING_NAMESPACE
 *  @ingroup utility_macros
 */
#define MIRROR_USING_NAMESPACE_(NAMESPACE) \
	MIRROR_USING_NESTED_NAMESPACE(_, NAMESPACE)


MIRROR_NAMESPACE_END

#endif //include guard

