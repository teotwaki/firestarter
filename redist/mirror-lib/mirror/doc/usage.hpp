/**
 *  file mirror/doc/usage.hpp
 *  Documentation only header
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_USAGE_1011291729_HPP
#define MIRROR_DOC_USAGE_1011291729_HPP

#ifdef MIRROR_DOCUMENTATION_ONLY

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN

/** @page reflection_usage Reflection usage
 *
 *  This section discusses several of these problematic situations and how
 *  reflection can be of help, with some examples how things can be implemented
 *  with Mirror.
 *
 *  - @ref usage_simple_logging
 *  - @ref usage_persistence
 *  - @ref usage_o_r_mapping
 *  - @ref usage_objinspect_infovis
 *  - @ref usage_scripting_rpc
 *  - @ref usage_design_patterns
 *  - @ref usage_semantics
 *  - @ref usage_code_generation
 *  - @ref usage_generic_and_meta_programming
 *
 *  @section usage_simple_logging Simple logging
 *
 *  There are applications which need to get and then somehow process the name
 *  of a given type. One example is simple logging where some of the messages
 *  may contain names of types. This is very helpful for example if one wants
 *  to log something in a template function and wants to distinguish between
 *  various instantiations in the logs. In this case it is very useful if
 *  the type name that goes into the log is human-readable and preferrably
 *  corresponding to the typename used in the source code. A rather naive
 *  implementation could look as follows:
 *
 *  @code
 *  #include <iostream>
 *
 *  ::std::ostream& log = ::std::cerr;
 *  using ::std::endl;
 *
 *  // a template function that returns the name
 *  // of the passed type
 *  template < typename T >
 *  const char* get_typename(void);
 *
 *
 *  template < typename A, typename B, typename C >
 *  void foo(A a, B b, C c)
 *  {
 *    log <<
 *      "entering foo(" <<
 *      get_typename<A>() <<
 *      ", " <<
 *      get_typename<B>() <<
 *      ", " <<
 *      get_typename<C>() <<
 *      ")" <<
 *    endl;
 *    //
 *    // do something useful here
 *    //
 *    log <<
 *      "leaving foo(" <<
 *      get_typename<A>() <<
 *      ", " <<
 *      get_typename<B>() <<
 *      ", " <<
 *      get_typename<C>() <<
 *      ")" <<
 *    endl;
 *  }
 *
 *  int main(void)
 *  {
 *    foo('A', "B", 0xC);
 *    foo(0xAL, ::std::string("B"), L'C');
 *    return 0;
 *  }
 *  @endcode
 *
 *  Maybe the first, obvious choice if one needs to find out what the name of
 *  a given type is is to use the <CODE>typeid</CODE> operator and call
 *  the <CODE>name()</CODE> function on the returned reference
 *  to <CODE>type_info</CODE>. The implementation of our
 *  <CODE>get_typename<T>()</CODE> function from the previous sample code could
 *  be following:
 *
 *  @code
 *  #include <typeinfo>
 *
 *  // a template function that returns the name
 *  // of the passed type
 *  template < typename T >
 *  const char* get_typename(void)
 *  {
 *    return typeid(T).name();
 *  }
 *  @endcode
 *
 *  There are however, several problems with is approach. The notoriously known
 *  issue of <CODE>type_info::name()</CODE> is that the string returned by this
 *  function is implementation-defined and compilers are free to return anything
 *  from an empty string, through a mangled typename to a correctly formatted
 *  typename and many of them enjoy this freedom to its full extent. Thus
 *  the returned name is not guarenteed to be unique nor human readable
 *  or easily understandable, nor is it portable. Some compilers provide functions
 *  that demangle the names returned by <CODE>type_info::name()</CODE>, but again
 *  this is not very portable.
 *
 *  To complicate things even more, we might want to get the base name of the type
 *  without the nested-name-specifier (<CODE>basic_string</CODE> vs.
 *  <CODE>::std::basic_string</CODE>) and to get the nested-name-specifier or
 *  even the individual names of the enclosing namespaces or classes separatelly.
 *
 *  This is when Mirror comes in handy. One of the basic facilities is the
 *  <CODE>meta_type</CODE> class template. Among other things
 *  this template has two member functions - <CODE>base_name</CODE>  and
 *  <CODE>full_name</CODE> that return the base type name without the nested
 *  name specifier and the full type name with the nested name specifier
 *  respectively. The names returned by these functions correspond to the C++
 *  typenames, thus are human-readable, unique (if using the full_name member
 *  function) and portable.
 *  There is also a <CODE>local_name</CODE> member function which allows to strip
 *  parts of the nested name specifier based on the use of the Mirror's
 *  <CODE>MIRROR_USING_NAMESPACE</CODE> macro.
 *
 *  Using <CODE>meta_type</CODE> our <CODE>get_typename<T>()</CODE> function
 *  could look like this:
 *
 *  @code
 *  #include <mirror/meta_type.hpp>
 *
 *  template < typename T >
 *  const char* get_typename(void)
 *  {
 *    // The MIRRORED_TYPE macro expands into
 *    // the proper specialization of meta_type
 *    // reflecting the passed type
 *    return MIRRORED_TYPE(T)::full_name().c_str();
 *  }
 *  @endcode
 *
 *  If you want to strip only selected parts of the full type name, you
 *  can use the <CODE>local_name</CODE> instead of <CODE>full_name</CODE>
 *  and to specify the namespaces to be stripped by using the
 *  <CODE>MIRROR_USING_NAMESPACE</CODE> macro.
 *
 *  @code
 *  #include <mirror/meta_type.hpp>
 *
 *  template < typename T >
 *  const char* get_typename(void)
 *  {
 *    // this tells the local_name function to strip the std::
 *    // prefix from all full type names
 *    MIRROR_USING_NAMESPACE(std);
 *    // this tells the local_name function to strip the myproject::
 *    // prefix from all full type names
 *    MIRROR_USING_NAMESPACE(myproject);
 *    // this is similar to the previous implementation
 *    return MIRRORED_TYPE(T)::local_name().c_str();
 *  }
 *  @endcode
 *
 *  With native C++ types and some common types from the STL and from Boost
 *  the <CODE>meta_type</CODE> works out of the box.
 *
 *  @section usage_persistence Persistence-related operations
 *
 *  There is a large group of operations which work on the structure
 *  of different (possibly elaborated) types in a program or on instances
 *  of these types in the same way and are generally related to converting
 *  the instances of various types to an external representation.
 *
 *  This includes serialization (or saving and loading) of instances of various
 *  types to some external data format (XML, JSON, XDR, ASN.1, etc.), marshalling
 *  i.e. transforming function parameters during remote procedure calls to
 *  a format suitable for transport over network and restoring them on the remote
 *  side where the actual call takes place, etc.
 *
 *  Such operations are generally pair-wise, one for converting native C++
 *  objects @em to and the other to convert them @em from the external format.
 *  It is often desirable to have the ability to do conversions of the objects
 *  of the same types to different representations.
 *
 *  One of the traditional ways is to define a common interface for such types
 *  and to define a pair of member functions for every external format, one
 *  for saving and one for loading.
 *
 *  This means that for @em N different operations one needs to add @em 2*N
 *  member functions to the interface (for example save_to_xml/load_from_xml,
 *  save_to_json / load_from_json, etc.).
 *  Furthermore if there are M classes which we want to be persistent we would
 *  need to implement @em 2*N*M member functions related only to serialization.
 *  For large projects with many classes and multiple serialization / marshalling
 *  formats the number of member functions starts to grow very fast.
 *
 *  For every new serialization format to be added one needs to add @em 2 functions
 *  to the persistence interface and to implement @em 2*M member functions in the
 *  persistent classes.
 *  To add a new persistent class one needs to implement @em 2*N member functions.
 *
 *  It is clear that a large project using this approach will get unmaintainable
 *  and full of bugs over time.
 *  On the other hand if a reflection facility able to traverse through the
 *  member variables of a class and to provide meta-data about them,
 *  then it is simple do decouple the persistence-related operations from the
 *  types they work on.
 *
 *  Every elaborated type is directly or indirectly composed out of base level types.
 *  If we want to save an instance of a class into an XML document, we use
 *  reflection to obtain the meta-data describing the instance, the variable
 *  used to access it and the class of the instance. Thus we know its memory
 *  address, its symbolic name, the internal structure of the class, its
 *  member variables, their names and types. If the member variable types are
 *  also structured we can obtain meta-data about them recursively until we
 *  reach the C++ intrinsic data-type level. The set of these basic data-types
 *  is limited (the number is less than 20).
 *
 *  Now all we need to know inside the persistence-related meta-operation is
 *  what to do with the native data type instances.
 *  In this example how to save them to an XML document, and also
 *  how handle structured parts of the class when provided with the meta-data
 *  by the reflection facility. This same approach is used also for loading
 *  the data from the external representation.
 *
 *  When using this approach in a custom reflection facility without native
 *  reflection support, one needs to register the @em M classes with the facility.
 *  To add a new persistence-related operation you need to implement functions
 *  for handling the (limited set of) native types.
 *
 *  So for @em M classes and @em N persistent data formats one needs to register
 *  these @em M classes with the reflection facility plus to implement
 *  @em 2*N persistent-format-handlers instead of @em 2*M*N member functions.
 *  The advantages of this approach are obvious; the classes are separated
 *  from the persistence-related operations. To make a class persistent,
 *  one needs only to make it reflectible. Once the meta-data is available
 *  any of the persistence-related operations can be executed on instances
 *  of that class. To add a new operation it is necessary to implement
 *  the interface for meta-operations. Once it is done, the operation can
 *  be executed on any object for which meta-data is available. Now
 *  the addition of a new class does not depend on the number of the operations
 *  and the addition of a new operation does not depend on the number of classes.
 *
 *  @section usage_o_r_mapping Object-Relational Mapping (ORM)
 *
 *  Another example of reflection usage may be the problem of implementing
 *  object-relational mapping.
 *
 *  This includes generation of relational database schema (tables,
 *  their columns, data types, etc.) for storing data
 *  of an object-oriented application. The schema should reflect the structure
 *  of the classes defined by the application. If a class C has member variables
 *  x, y and z, so should the table used to store instances of this class.
 *  Furthermore the names of the tables and their columns should be equivalent
 *  to the names of the classes and their member variables to avoid confusion.
 *
 *  As the application evolves so should the database schema and this evolution
 *  should be automatic or require as less developer attention as possible.
 *
 *
 *
 *  @section usage_objinspect_infovis Object inspectors and information visualization
 *
 *  Meta-data can be used to generate GUI components which when passed
 *  an instance and the required meta-data can show the layout of the class
 *  and also the values of its member attributes
 *
 *  @section usage_scripting_rpc RPC/RMI and Scripting
 *
 *  An instance of a service class can be registered inside of some
 *  implementation of the RPC/RMI (Remote Procedure Call / Remote Method
 *  Invocation) mechanism using the meta-data describing
 *  the methods of that service class. This metadata can be then used
 *  on the server side to find and invoke the appropriate method of the instance
 *  and also as already mentioned the meta-data can be used to convert
 *  the parameters and the return value to a form suitable for transport
 *  between the address spaces of the client and server processes.
 *
 *  Reflection can be also useful in implementing scripting support for
 *  applications. Actually this is very similar to remote calls with the
 *  difference that the caller is our scripting language parser or interpreter
 *  and it again uses some service object registry and the meta-data to find
 *  the proper instance and method, convert the parameters from the script code
 *  to suitable binary form and do the call.
 *
 *  @section usage_design_patterns Implementation of several design patterns
 *
 *  Design patterns are general reusable solutions applicable to recurring
 *  problems in software design. The meta-data and the reflective utilities
 *  provided by Mirror can be used for the concrete implementations of these
 *  design patterns.
 *
 *  One example of such utility is the @link mirror_factory_generator_utility
 *  factory generator@endlink, which allows to create factory classes that
 *  can be used to construct instances of a @c Product type through a generic
 *  interface. The generated factories do not require the caller
 *  to supply the parameters for the construction directly.
 *  Such factories pick or let the application user pick the @c Product 's most
 *  appropriate constructor, they gather the necessary parameters in
 *  a generic, application-specific way and use the selected constructor
 *  to create an instance of the @c Product.
 *  These factories can then be used to implement the
 *  <A HREF="http://en.wikipedia.org/wiki/Abstract_factory_pattern">abstract factory</A>,
 *  [wikipedia.org] the
 *  <A HREF="http://en.wikipedia.org/wiki/Factory_method_pattern">factory method</A>
 *  [wikipedia.org] or in the case of complex object constructions even the
 *  <A HREF="http://en.wikipedia.org/wiki/Builder_pattern">builder</A>
 *  [wikipedia.org] design patterns.
 *
 *  Since reflection is especially handy when implementing persistence-related
 *  operations, it can be also used for the implementation of the
 *  <A HREF="http://en.wikipedia.org/wiki/Memento_pattern">memento</A>
 *  [wikipedia.org] pattern, where the @em caretaker object can use reflection
 *  to create a snapshot of the internal state of another object and to restore
 *  this internal state in case of a rollback operation.
 *
 *  @section usage_semantics Access point to high-level semantic information
 *
 *
 *
 *  @section usage_code_generation Source code generation and replication
 *
 *  @section usage_generic_and_meta_programming Support for generic and meta-programming
 *
 */

MIRROR_NAMESPACE_END

#endif // DOCUMENTATION_ONLY
#endif //include guard

