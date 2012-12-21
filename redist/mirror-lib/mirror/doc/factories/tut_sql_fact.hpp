/**
 *  file mirror/doc/factories/tut_sql_fact.hpp
 *  Documentation only header
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_FACTORIES_TUT_SQL_FACT_1011291729_HPP
#define MIRROR_DOC_FACTORIES_TUT_SQL_FACT_1011291729_HPP

#ifdef MIRROR_DOCUMENTATION_ONLY

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN

/** @page mirror_sql_fact_gen_tutorial Writing an SQL-based factory generator plugin from scratch
 *
 *  The aim of this tutorial is to show how to write a plugin for the
 *  @link mirror_factory_generator_utility factory generator utility@endlink,
 *  that will allow to generate factories using a database dataset as
 *  the source of data for instance construction.
 *
 *  More specifically the factory generator together with this plugin
 *  will allow to instantiate factory classes for various @c Product types.
 *  These factories will be able to create instances of a @c Product
 *  by taking a valid and open dataset resulting from an SQL query on a database,
 *  checking the available data, comparing them with the constructor signatures
 *  of the @c Product type, choosing the most appropriate constructor and
 *  using it to create and return an instance.
 *  If the dataset contains multiple rows, then multiple instances can be created
 *  by the same factory, by externally traversing the dataset.
 *
 *  The basic principles of the factory generator, necessary for the understanding
 *  of this tutorial can be found in @ref mirror_factory_generator_utility,
 *  more information about factories generally and factories generated by the
 *  factory generator utility can be found in @ref mirror_fact_gen_in_depth.
 *
 *  The whole working source explained by this tutorial can be found in the
 *  files included by the @c utils/sql_factory.hpp header in Mirror's source
 *  directory. The SOCI and libpq-based factory generator plugins, that are included
 *  with Mirror in the @c utils subdirectory are derived from this general
 *  SQL-factory plugin.
 *
 *  TODO: finish this
 *
 */

MIRROR_NAMESPACE_END

#endif // DOCUMENTATION_ONLY
#endif //include guard
