/**
 *  file mirror/doc/reflection.hpp
 *  Documentation only header
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_REFLECTION_1011291729_HPP
#define MIRROR_DOC_REFLECTION_1011291729_HPP

#ifdef MIRROR_DOCUMENTATION_ONLY

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN

/** @page mirror_reflection Reflection with Mirror
 *
 *  @section reflection_examples Examples of reflection
 *
 *  This section shows how to use Mirror to obtain
 *  meta-data describing various language constructs like
 *  namespaces, types, typedefs, classes, templates, member variables,
 *  member functions, constructors, etc.
 *
 *  This section however shows only the basics which may not seem very useful
 *  at the first glance. For more elaborate examples showing the real power
 *  of reflective programming and meta-programming with Mirror see the example
 *  section of this documentation and the examples included with the sources.
 *
 *  Generally reflection is performed by using the @ref reflection_macros
 *  "reflection macros", which are expanded into types conforming to various
 *  @ref meta_object_concepts "concepts". This means that they can be given
 *  a symbolic name by using @c typedef or be used as the type of a variable,
 *  for example:
 *
 *  @code
 *  // reflect the 'std' namespace                                       //
 *  typedef MIRRORED_NAMESPACE(std) meta_std_t;                          // typedef
 *  MIRRORED_NAMESPACE(std) meta_std;                                    // variable
 *                                                                       //
 *  meta_std_t::base_name();                                             // use the typedef
 *  meta_std.full_name();                                                // use the variable
 *                                                                       //
 *  // reflect the 'mstring' typedef                                     //
 *  typedef MIRRORED_TYPEDEF(mirror,mstring) meta_mstring_t;             // typedef
 *  MIRRORED_TYPEDEF(mirror,mstring) meta_mstring;                       // variable
 *                                                                       //
 *  meta_mstring.base_name();                                            // use the variable
 *  meta_mstring_t::full_name();                                         // use the typedef
 *  @endcode
 *
 *  The meta-objects (types) are stateless and have only @c static member
 *  functions and thus constructing instances of these types is cheap.
 *  On the other hand it is probably better to use the meta-objects as types since
 *  one cannot access the member typedefs of a variable nor use them directly in the
 *  template metafunctions, like @c members, @c parameters, @c base_classes, etc.
 *  Therefore the meta-objects are used as types in most of the following examples.
 *
 *  @section mirror_namespace_reflection Namespace reflection
 *
 *  Namespaces are reflected with the @c #MIRRORED_NAMESPACE() macro.
 *  This macro expands into a type conforming to the @link MetaNamespace
 *  MetaNamespace @endlink concept. For starters, let us reflect the @c std namespace.
 *
 *  @code
 *  // reflect the 'std' namespace                                       //
 *  typedef MIRRORED_NAMESPACE(std) meta_std;                            //
 *  @endcode
 *
 *  The first thing we can do is to examine the @link MetaNamedObject::base_name()
 *  base @endlink and the @link MetaNamedScopedObject::full_name() full @endlink
 *  name of the reflected entity.
 *
 *  @code
 *  // get the base name of the namespace                                //
 *  meta_std::base_name();                                               // string: "std"
 *  // get the full name of the namespace                                //
 *  meta_std::full_name();                                               // string: "std"
 *  @endcode
 *
 *  This is not very spectacular since the base and the full names are the same
 *  because the namespace is not nested. Let us do the same thing with the
 *  @c mirror namespace:
 *
 *  @code
 *  // reflect the 'mirror' namespace                                    //
 *  typedef MIRRORED_NAMESPACE(mirror) meta_mirror;                      //
 *  // get the base name of the namespace                                //
 *  meta_mirror::base_name();                                            // string: "mirror"
 *  // get the full name of the namespace                                //
 *  meta_mirror::full_name();                                            // string: "mirror"
 *  @endcode
 *
 *  The @link MetaNamedObject::base_name() @c base_name @endlink
 *  member function is defined by the @link MetaNamedObject
 *  MetaNamedObject @endlink concept and allows to get the base name of the reflected
 *  entity without the nested name specifier.
 *  The @link MetaNamedScopedObject::full_name() @c full_name @endlink
 *  member function is defined by the @link MetaNamedScopedObject
 *  MetaNamedScopedObject @endlink concept and returns the full name including
 *  the nested name specifier.
 *
 *  There is one another name getting function in the MetaNamedScopedObject concept
 *  called @link MetaNamedScopedObject::local_name() @c local_name @endlink,
 *  which allows to strip certain prefixes of the full nested name specifier, based
 *  on the use of the @c #MIRROR_USING_NAMESPACE() macro, as shown for example
 *  @link mirror/example/typenames_03.cpp here @endlink and on the following code
 *  sample:
 *
 *  @code
 *  // reflect the 'mirror' namespace                                    //
 *  typedef MIRRORED_NAMESPACE(mirror) meta_mirror;                      //
 *  // get the local name of the namespace                               //
 *  meta_mirror::local_name();                                           // string: "mirror"
 *  // now 'use' the boost namespace                                     //
 *  MIRROR_USING_NAMESPACE(boost)                                        //
 *  // get the local name of the namespace again                         //
 *  meta_mirror::local_name();                                           // string: "mirror"
 *  @endcode
 *
 *  Of course namespaces are not the only named constructs; meta-objects
 *  reflecting for example types, typedefs, classes, member functions and
 *  many other things are also models of the @link MetaNamedObject
 *  MetaNamedObject @endlink or the @link MetaNamedScopedObject
 *  MetaNamedScopedObject @endlink concept and have the @c get_name, @c base_name,
 *  @c full_name and @c local_name member functions.
 *
 *  @subsection mirror_global_scope_reflection Global scope reflection
 *
 *  There is one special namespace - the global scope, which is anonymous
 *  and must be reflected with the @c #MIRRORED_GLOBAL_SCOPE macro:
 *  @code
 *  // reflect the global scope                                //
 *  typedef MIRRORED_GLOBAL_SCOPE() meta_gs;                   //
 *                                                             //
 *  meta_gs::base_name();                                      // string: ""
 *  meta_gs::full_name();                                      // string: ""
 *  @endcode
 *
 *  @section mirror_type_reflection Type reflection
 *
 *  Types are reflected with the @c #MIRRORED_TYPE() macro, which expands
 *  into a type conforming to the @link MetaType MetaType @endlink
 *  concept.
 *
 *  @code
 *  // reflect the 'int' type                                            //
 *  typedef MIRRORED_TYPE(int) meta_int;                                 //
 *  // reflect the standard string type                                  //
 *  typedef MIRRORED_TYPE(std::string) meta_str;                         //
 *                                                                       //
 *  meta_int::base_name();                                               // string: "int"
 *  meta_str::base_name();                                               // string: "string"
 *  meta_int::full_name();                                               // string: "int"
 *  meta_str::full_name();                                               // string: "std::string"
 *  meta_int::scope;                                                     // type: MetaNamespace
 *  meta_str::scope;                                                     // type: MetaNamespace
 *  meta_int::scope::full_name();                                        // string: ""
 *  meta_str::scope::full_name();                                        // string: "std"
 *  meta_int::original_type;                                             // type: int
 *  meta_str::original_type;                                             // type: std::string
 *  @endcode
 *
 *  @section mirror_class_reflection Class reflection
 *
 *  Elaborated types (classes) are reflected with the @c #MIRRORED_CLASS()
 *  macro, which expands into a type conforming
 *  to the @link MetaClass MetaClass @endlink concept.
 *
 *  @code
 *                                                                       //
 *  @endcode
 *
 */

MIRROR_NAMESPACE_END

#endif // DOCUMENTATION_ONLY
#endif //include guard

