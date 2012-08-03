/**
 * .file mirror/doc/concepts_fwd.hpp
 * .brief Forward declaration of concepts
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_CONCEPTS_FWD_1104261426_HPP
#define MIRROR_DOC_CONCEPTS_FWD_1104261426_HPP

#include <mirror/config.hpp>

// This section is here for documentation purposes only
#ifdef MIRROR_DOCUMENTATION_ONLY

MIRROR_NAMESPACE_BEGIN

struct SpecifierTag;
struct InheritanceTypeTag;
struct StorageClassTag;
struct ConstnessTag;
struct AccessTypeTag;
struct ElaboratedTypeTag;

struct MetaObject;
struct MetaNamedObject;
struct MetaMetaObject;
struct MetaScopedObject;
struct MetaNamedScopedObject;
struct MetaScope;
struct MetaNamespace;
struct MetaType;
struct MetaTypedef;
struct MetaTemplatedType;
struct MetaTypeTemplate;
struct MetaClass;
struct MetaClassMember;
struct MetaTemplatedClass;
struct MetaEnum;
struct MetaEnumValue;
struct MetaInheritance;
struct MetaVariable;
struct MetaFreeVariable;
struct MetaPlainFreeVariable;
struct MetaMemberVariable;
struct MetaPlainMemberVariable;
struct MetaParameter;
struct MetaFunction;
struct MetaMemberFunction;
struct MetaConstructor;
struct MetaConversionOperator;
struct MetaOverloadedFunctions;

struct Locator;
struct SafeLocator;
struct Traversal;
struct MetaTraversal;
struct MetaLocator;
struct MetaInserter;
struct MetaEraser;
struct MetaContainer;

struct IntegralConstantType;
struct BooleanConstantType;
template <typename ... Elements> struct Range;
template <typename Element> struct Optional;

MIRROR_NAMESPACE_END

#endif // MIRROR_DOCUMENTATION_ONLY

#endif //include guard

