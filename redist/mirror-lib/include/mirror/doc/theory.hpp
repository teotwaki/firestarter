/**
 *  file mirror/doc/theory.hpp
 *  Documentation only header
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_THEORY_1011291729_HPP
#define MIRROR_DOC_THEORY_1011291729_HPP

#ifdef MIRROR_DOCUMENTATION_ONLY

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN

/** @page reflection_basics Reflection basics
 *
 *  This is a very short introduction into reflection and reflective programming
 *  theory.
 *
 *  @section reflection_intro Introduction
 *
 *  Generally speaking reflection is a mechanism, which makes possible for an entity
 *  to investigate and change itself.
 *  In computer science, reflection refers to the ability of a computer program
 *  to examine and modify its own structure and behavior. This includes building
 *  new or altering the existing data structures or if possible also the program
 *  code. The programming paradigm driven by reflection is called @em reflective
 *  programming. It is a particular kind of @em metaprogramming.
 *
 *  Metaprogramming is usually a more general term where a program creates,
 *  examines and/or manipulates a different program, i.e. not itself.
 *  Reflection-oriented programming includes self-examination, self-modification,
 *  and self-replication, which can be (depending on the programming language)
 *  done at compile-time and/or run-time.
 *
 *  Native support for reflective programming is most common in high-level virtual machine
 *  programming languages like Java, C#, CLOS, Smalltalk, etc. and in interpreted
 *  scripting languages. Reflective programming is less common or limited
 *  in statically typed programming languages like C or C++. Because of this, dynamic
 *  or run-time reflection is more common.
 *  Besides native support by the language itself, reflection can be added by
 *  the means of a library.
 *
 *  One of the basic functions of a reflection facility is to provide
 *  meta-data describing the program's structure and code. This meta-data can
 *  be then used in various programming tasks. Furthermore a reflection facility
 *  may provide higher-level functionality based on this meta-data.
 *
 *  Various implementations of reflection and meta-programming facilities can be
 *  classified by their support for:
 *
 *  - @em Introspection: the ability of the program to examine its own structure,
 *    which includes the program's data-structures and/or code.
 *
 *  - @em Self-modification: the ability of the program to change its own structure;
 *    again either data-structures or code.
 *
 *  - @em Code-generation: the ability to generate and execute new program code.
 *    Partially this is a special case of self-modification.
 *
 *  - @em Intercession: the ability to change the semantics of the underlying
 *    programming language from within the language itself. Support for intercession
 *    is quite rare at least in the mainstream languages.
 *
 *  @section reflection_in_cpp Support for reflection in C++
 *
 *  Although C++ has a huge generic programming and meta-programming potential
 *  (as demonstrated by some of the Boost Libraries, STL and many other libraries),
 *  C++ implementations (conforming to the ANSI/ISO standard) have only a very limited
 *  native support for reflection. Prior to the C++0x standard, basically the only
 *  reflection mechanism was the @em RTTI (Run Time Type Identification).
 *  RTTI allows to identify types at both compile-time and run-time (the latter is
 *  valid only for classes with virtual member functions), do comparison between types,
 *  and query some very basic information about these types.
 *
 *  The C++0x standard introduced some other useful features like the decltype operator
 *  and the auto type specifier. Furthermore the standard library now contains several
 *  useful type traits - template classes for inspecting various traits, categorizing
 *  and modifying types.
 *
 *  Various compilers and development environments provide additional non-standard
 *  extensions for reflective programming. Prior to C++0x many compilers supported
 *  for example some version of the typeof / decltype operator.
 *  Another example may be the Borland's C++ Builder RAD tool that introduced several
 *  basic reflection facilities, like the @em __classid operator, the @em TMetaClass
 *  type, etc., which allow to perform basic object inspection, and other things.
 *  These usually serve for internal purposes of the IDEs, and developers are
 *  discouraged to use them directly.
 *  The main problem of these extensions is that they are not portable.
 *
 *  C++ has strong type checking and is considered to be rather a static
 *  language. This can be very limiting in the terms of data-structure modifications
 *  during run-time. Furthermore C++ programs are not running inside of a virtual
 *  machine, but are compiled into executable binaries and therefore run-time
 *  modification of the code is @em generally not possible, without re-compilation.
 *
 *  @section reflection_design_principles Design principles for meta-programming facilities
 *
 *  Similar to the basic principles of object-oriented programming, there are
 *  also three principles that are attributed to well designed reflection and
 *  meta-programming architectures:
 *
 *  - @em Encapsulation: the meta-level facilities encapsulate their implementation
 *    behind appropriate interfaces; either generic compile-time or abstract
 *    run-time interfaces. The client should never get access to implementation
 *    dependent details and use them. Encapsulation is generally agreed to be
 *    a useful programming technique.
 *
 *  - @em Stratification: meta-level facilities must be separated from base-level
 *    functionality and the base-level structures must be usable without
 *    the meta-level objects. According to the principle of stratification
 *    the meta-level objects must be queried by using of special reflection
 *    functions detached and separable from the base-level objects.
 *
 *  - @em Ontological @em correspondence: the ontology of meta-level facilities
 *    should correspond to the ontology of the language they reflect.
 *    This includes structural and temporal correspondence. The meta-level
 *    facilities should correspond to the base-level objects they describe or
 *    manipulate and also distinguish between the phases in the lifetime of
 *    a program (design, preprocessing, compilation, linking and run-time),
 *    although some of these phases may be ommited.
 *
 *  Today most of the mainstream implementations of the reflection facilities
 *  (usually referred to as traditional reflection facilities) do not follow
 *  these principles.
 *  The one of the common issues is, that in the traditional reflection
 *  facilities the meta-data is tightly tied to the base-level data, usually
 *  by deriving all predefined and user-defined classes from a single ultimate base
 *  class, that has several virtual member functions which provide meta-data
 *  about the concrete instance for which they are called and also about its
 *  type.
 *
 *  The problem with this approach is that it violates the rule of stratification,
 *  because the meta-level objects cannot be detached from the base-level objects.
 *  Thus in applications where reflection is not needed it still must be included.
 *  This can enlarge the footprint of the application, which can be a problem
 *  for example in mobile applications, where storage and memory resources are
 *  limited.
 *
 *  @code
 *  // definition of a meta class' interface by the reflection facility
 *  interface iMetaClass
 *  {
 *    virtual string getName(void) = 0;
 *    //...other member functions providing meta-data about the reflected class
 *  };
 *
 *  // a special common (implicit) base class for every other class
 *  class Object
 *  {
 *    iMetaClass* getClass(void) = 0;
 *    // other members
 *  };
 *
 *  // user defined class implicitly derives from Object
 *  class myClass [ : public Object ]
 *  {
 *    // class' members ...
 *    // automatically generated implicit members possibly
 *    // using some variant of covariant return type
 *    MetaClass<myClass>* getClass(void){return new MetaClass<myClass>();}
 *  };
 *
 *  myClass* myInst1 = new myClass;
 *  // use of the reflection facility
 *  // reflection is inherent part of the base-level objects
 *  std::cout << myInst1->getClass()->getName() << std::endl;
 *  //
 *  // PROBLEM: this won't work because the intrinsic types don't
 *  // inherit from Object
 *  double myInst2;
 *  std::cout << myInst2.getClass()->getName() << std::endl;
 *  @endcode
 *
 *  Another approach is to separate the meta-objects from the base-level objects
 *  and tie them only on demand by special reflection functors, sometimes called
 *  @em mirrors.
 *
 *  A simple example is shown in the following pseudo-code sample:
 *
 *  @code
 *  // definition of a meta class' interface by the reflection facility
 *  interface iMetaClass
 *  {
 *    virtual string getName(void) = 0;
 *    //...other member functions providing meta-data about the reflected class
 *  };
 *
 *  // reflection functor returning the meta-data about a class
 *  template <Class> iMetaClass* reflect_class(void);
 *
 *  // user defined class. no common base necessary
 *  class myClass
 *  {
 *    // class' members ...
 *    // none of them reflection-related
 *  };
 *
 *  // use of the reflection facility
 *  std::cout << reflect_type<myClass>()->getName() << std::endl;
 *  // no problem here either
 *  std::cout << reflect_type<double>()->getName() << std::endl;
 * // nor here
 *  double myInst;
 *  std::cout << reflect_type<decltype(myInst)>()->getName() << std::endl;
 *  @endcode
 *
 *  @section source_of_metadata Sources of meta-data
 *
 *  In languages which support reflection inherently, there is no need to solve
 *  the problem of how to get the meta-data, reflecting the base-level objects.
 *  The source is the program compiler or interpreter. However in custom built
 *  reflection APIs, one needs to find alternate means how to collect meta-data.
 *  Some of the options usable in a C++ reflection facility are presented
 *  in the following list:
 *
 *  - External applications for parsing and transforming the source code.
 *    Such applications may parse the source files passed as input and use hardcoded
 *    or custom transformation instructions to generate the source code containing
 *    meta-data usable by the reflection facility.
 *
 *  - Compiler extensions enabling access to the parse tree of the program.
 *    Several compilers, for example GCC can be extended to optionally export
 *    the parse data, which can be further processed and transformed into
 *    the meta-data. C++2XML project enables to export the preprocessing
 *    and compilation data into XML, which can be further transformed via
 *    XSLT to produce C++ sources containing meta-level objects.
 *    The problem of this solution is that it is not portable.
 *
 *  - Extending compiler with meta-object protocol (MOP). Similar to
 *    the previous option, a compiler is including or can be extended with
 *    a meta-object protocol, which enables direct transformation of the source code
 *    containing the base-level declarations into code containing
 *    the implementation of the meta-level objects. The OpenC++ project provides
 *    a working implementation of such meta-object-protocol. Unfortunately
 *    it failes to work with more complex template code. This option also
 *    shares the main drawback with the previous, it is not portable.
 *
 *  - User defined meta-data in an external language, for example in XML.
 *    Programmers put the declarations into an XML file which can be
 *    transformed to C++ meta-object implementations. Actually the XML
 *    representation could be also used to generate the base-level C++
 *    declarations. The main issue can be the synchronization
 *    of the C++ source code with the XML representation, an advantage
 *    is portability and the openness of the XML and XSLT formats,
 *    and many free and portable XSL transformation tools.
 *
 *  - Meta-objects provided by the programmer. Programmers can manually
 *    implement the meta-level interfaces for their classes and other base-level
 *    objects. This approach is portable since the meta-objects are implemented
 *    in the same language in which they are used. The main drawback is that
 *    it is a tedious and possibly error-prone task and requires continual
 *    synchronization between the base-level objects and the meta-object
 *    implementations as the application evolves. Another problem is that
 *    if the meta-object interfaces change for some reason, all the implementations
 *    must be updated as well.
 *
 *  - Base-level object registering with the reflection library. The reflection
 *    facility can provide means for the
 *    registering of the base-level objects and provides meta-level objects
 *    based on this basic registered meta-data. Furthermore the registering
 *    process can autodetect many aspects of the registered base-level
 *    objects. This option has many advantages when compared to the previous
 *    option because the programmer must input only essential the meta-information
 *    and the rest is auto-detected and inferred by the reflection facility
 *    with the help of the tools provided by the programming language and
 *    the meta-data is provided by objects with appropriate interfaces.
 *
 *  Generally when implementing reflection in a language without inherent support
 *  one should use a suitable combination of the mentioned options.
 *
 */

MIRROR_NAMESPACE_END

#endif // DOCUMENTATION_ONLY
#endif //include guard

