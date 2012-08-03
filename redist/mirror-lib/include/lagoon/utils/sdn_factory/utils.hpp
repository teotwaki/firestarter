/**
 * .file lagoon/utils/sdn_factory/utils.hpp
 * .brief Utils for the script-parsing polymorphic factory
 *
 *  Copyright 2008-2010 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef LAGOON_UTILS_SDN_FACTORY_UTILS_1011291729_HPP
#define LAGOON_UTILS_SDN_FACTORY_UTILS_1011291729_HPP


LAGOON_NAMESPACE_BEGIN
namespace aux {

template <class Traits>
class sdn_fact_utils
{
private:
	typedef mirror::aux::sdn_fact_param<Traits> param_type;
protected:
	param_type& deref(raw_ptr raw_param)
	{
		assert(raw_param != nullptr);
		return *raw_cast<param_type*>(raw_param);
	}
};

} // namespace aux
LAGOON_NAMESPACE_END

#endif //include guard

