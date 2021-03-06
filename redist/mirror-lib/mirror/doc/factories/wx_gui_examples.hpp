/**
 *  file mirror/doc/factories/wx_gui_examples.hpp
 *  Documentation only header
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_DOC_FACTORIES_WX_GUI_EXAMPLES_1011291729_HPP
#define MIRROR_DOC_FACTORIES_WX_GUI_EXAMPLES_1011291729_HPP

#ifdef MIRROR_DOCUMENTATION_ONLY

#include <mirror/config.hpp>

MIRROR_NAMESPACE_BEGIN

/** @page mirror_wx_gui_fact_examples Examples of GUI generated by the wx_gui_factory
 *
 *  Here can be found some examples of wxWidgets-based GUI dialogs
 *  generated by the
 *  @link mirror_factory_generator_utility factory generator utility@endlink
 *  with the @c wx_gui_factory plugin. These input dialogs are a part of
 *  a object factory and serve as the input for data that are to be converted
 *  to parameters for the constructors. Furthermore the dialog also allows to
 *  choose which constructor should be used for the construction of a particular
 *  instance. The dialogs also support localization and input data validation.
 *  The screenshots show how the dialogs look in different operating systems
 *  with different window managers.
 *
 *  The following few dialogs are used for the construction of instances of
 *  the @c tetrahedron class, also using the @c triangle and @c vector classes
 *  with the following declaration:
 *
 *  @code
 *
 * struct vector
 * {
 *     double x,y,z;
 *
 *      vector(double _x, double _y, double _z)
 *      : x(_x)
 *      , y(_y)
 *      , z(_z)
 *     { }
 *
 *     vector(double _w)
 *      : x(_w)
 *      , y(_w)
 *      , z(_w)
 *     { }
 *
 *     vector(void)
 *      : x(0.0)
 *      , y(0.0)
 *      , z(0.0)
 *     { }
 *
 *     ...
 * };
 *
 * struct triangle
 * {
 *     vector a, b, c;
 *
 *     triangle(const vector& _a, const vector& _b, const vector& _c)
 *      : a(_a)
 *      , b(_b)
 *      , c(_c)
 *     { }
 *
 *     triangle(void){ }
 *
 *     ...
 * };
 *
 * struct tetrahedron
 * {
 *     triangle base;
 *     vector apex;
 *
 *     tetrahedron(const triangle& _base, const vector& _apex)
 *      : base(_base)
 *      , apex(_apex)
 *     { }
 *
 *     tetrahedron(
 *         const vector& a,
 *         const vector& b,
 *         const vector& c,
 *         const vector& d
 *     ): base(a, b, c)
 *      , apex(d)
 *     { }
 *
 *     ...
 * };
 *
 *  @endcode
 *
 *  In the dialog below the red color of the input component indicates that
 *  the input is invalid. In such state the dialog cannot be closed by clicking
 *  on @c OK, instead a hint is shown saying that there is invalid input data.
 *
 *  @image html  wx_gui_dialog_01.png ""
 *  @image latex wx_gui_dialog_01.png ""
 *
 *  The same dialog as above, with a different constructor selected for
 *  the construction of the @c triangle 's @c c parameter.
 *
 *  @image html  wx_gui_dialog_02.png ""
 *  @image latex wx_gui_dialog_02.png ""
 *
 *  Here the default constructor has been selected for the construction
 *  of the @c triangle 's @c c parameter.
 *
 *  @image html  wx_gui_dialog_03.png ""
 *  @image latex wx_gui_dialog_03.png ""
 *
 *  Here another constructor, taking four vectors, instead of a triangle
 *  and a vector is selected to construct the tetrahedron.
 *
 *  @image html  wx_gui_dialog_04.png ""
 *  @image latex wx_gui_dialog_04.png ""
 *
 *  @image html  wx_gui_dialog_05.png ""
 *  @image latex wx_gui_dialog_05.png ""
 *
 *  @image html  wx_gui_dialog_06.png ""
 *  @image latex wx_gui_dialog_06.png ""
 *
 *  This is basically the same dialog using another book-control component
 *  to manage the individual constructors.
 *
 *  @image html  wx_gui_dialog_07.png ""
 *  @image latex wx_gui_dialog_07.png ""
 *
 *  This is an older version of the same dialog as above, but this time in Vista.
 *
 *  @image html  wx_gui_dialog_08.png ""
 *  @image latex wx_gui_dialog_08.png ""
 *
 *  @image html  wx_gui_dialog_09.png ""
 *  @image latex wx_gui_dialog_09.png ""
 *
 *  @image html  wx_gui_dialog_10.png ""
 *  @image latex wx_gui_dialog_10.png ""
 *
 *  Two more examples showing a generated dialog, this time for the construction
 *  of a @c person class. Here the localization / translation feature has been
 *  enabled and the basic class and parameter names are translated to a more
 *  user-friendly form.
 *
 *  @image html  wx_gui_dialog_11.png ""
 *  @image latex wx_gui_dialog_11.png ""
 *
 *  @image html  wx_gui_dialog_12.png ""
 *  @image latex wx_gui_dialog_12.png ""
 */

MIRROR_NAMESPACE_END

#endif // DOCUMENTATION_ONLY
#endif //include guard

