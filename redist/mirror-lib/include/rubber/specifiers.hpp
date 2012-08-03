/**
 * @file rubber/specifiers.hpp
 * @brief Wrapper for specifiers
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef RUBBER_SPECIFIERS_1103151301_HPP
#define RUBBER_SPECIFIERS_1103151301_HPP

#include <rubber/config.hpp>
#include <rubber/range.hpp>
#include <mirror/specifier_tags.hpp>
#include <puddle/specifiers.hpp>

RUBBER_NAMESPACE_BEGIN

/// Class wrapping Mirror's Specifiers
/**
 *  @see mirror::Specifier
 *  @ingroup rubber_type_erased
 */
class specifier
{
private:
	static inline bool category_matches(unsigned a, unsigned b)
	{
		return (a & b) == b;
	}

	puddle::specifier_category _cat;
public:
	/// Constructor erasing the type of a Specifier
	template <typename Specifier>
	specifier(Specifier s)
	 : _cat(puddle::get_category(s))
	{ }

	/// Equality comparison
	friend bool operator == (const specifier& a, const specifier& b)
	{
		return a._cat == b._cat;
	}

	/// Nonequality comparison
	friend bool operator != (const specifier& a, const specifier& b)
	{
		return a._cat != b._cat;
	}

	/// Less-equal comparison
	friend bool operator <= (const specifier& a, const specifier& b)
	{
		return a._cat <= b._cat;
	}

	/// Greater-equal comparison
	friend bool operator >= (const specifier& a, const specifier& b)
	{
		return a._cat >= b._cat;
	}

	/// Less-than comparison
	friend bool operator <  (const specifier& a, const specifier& b)
	{
		return a._cat <  b._cat;
	}

	/// Greater-than comparison
	friend bool operator >  (const specifier& a, const specifier& b)
	{
		return a._cat >  b._cat;
	}

	/// Returns the category of the specifier
	puddle::specifier_category category(void) const
	{
		return _cat;
	}

	/// Checks if the specifier is of a certain type
	inline bool is_a(puddle::specifier_category cat) const
	{
		return category_matches(
			unsigned(category()),
			unsigned(cat)
		);
	}

	#ifdef MIRROR_DOCUMENTATION_ONLY
	/// A set of member functions indicating this meta-object's category
	/** This is not a single function, it is a set of member functions
	 *  like is_static, is_const, is_virtual, etc. for every specifier
	 *  in C++, each returning true or false based on what the specifier
	 *  reflects.
	 */
	bool is_X(void) const;
	#endif
#define RUBBER_HELPER_MAKE_SPEC_IS_X(SPEC, KW, GRP, I, X) \
	inline bool is_## SPEC(void) const \
	{ \
		return is_a(puddle::specifier_category::spec_ ## SPEC ## _tag); \
	}
MIRROR_FOR_EACH_SPECIFIER(RUBBER_HELPER_MAKE_SPEC_IS_X, _)
#undef RUBBER_HELPER_MAKE_SPEC_IS_X

};

RUBBER_NAMESPACE_END

#endif //include guard

