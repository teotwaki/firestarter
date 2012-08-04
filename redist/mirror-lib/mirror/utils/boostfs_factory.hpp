/**
 * @file mirror/utils/boostfs_factory.hpp
 * @brief Boost.Filesystem-based factory generator plugin
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIRROR_UTILS_BOOSTFS_FACTORY_1101122253_HPP
#define MIRROR_UTILS_BOOSTFS_FACTORY_1101122253_HPP

#include <mirror/config.hpp>
#include <mirror/utils/sdn_factory.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

MIRROR_NAMESPACE_BEGIN

class sdn_fact_boostfs_property;
class sdn_fact_boostfs_element;
class sdn_fact_boostfs_range;

class sdn_fact_boostfs_property
{
private:
	typedef sdn_fact_boostfs_property property;
	typedef boost::filesystem::path bfs_path;
	bfs_path filepath;

	friend class sdn_fact_boostfs_element;

	sdn_fact_boostfs_property(const bfs_path& _path)
	 : filepath(_path)
	{ }

	bool ok(void) const
	{
		return boost::filesystem::exists(filepath) &&
			boost::filesystem::is_regular_file(filepath);
	}
public:
	operator bool (void) const
	{
		return ok();
	}

	bool operator ! (void) const
	{
		return !ok();
	}

	bool has_type(const std::string& type_name)
	{
		return true;
	}

	std::string text(void)
	{
		assert(ok());
		boost::filesystem::ifstream f(filepath, std::ios_base::in);
		std::stringbuf temp;
		f.get(temp);
		return temp.str();
	}
};

class sdn_fact_boostfs_range
{
private:
	typedef sdn_fact_boostfs_element element;
	typedef boost::filesystem::path bfs_path;
	boost::filesystem::directory_iterator cur, end;
public:
	sdn_fact_boostfs_range(const bfs_path& _path)
	 : cur(_path)
	 , end()
	{ }

	bool empty(void) const
	{
		return cur == end;
	}

	void step_front(void)
	{
		assert(!empty());
		++cur;
	}

	element front(void) const;
};

class sdn_fact_boostfs_element
{
private:
	typedef sdn_fact_boostfs_range range;
	typedef sdn_fact_boostfs_element element;
	typedef sdn_fact_boostfs_property property;

	typedef boost::filesystem::path bfs_path;
	bfs_path dirpath;

	bool ok(void) const
	{
		return boost::filesystem::exists(dirpath) &&
			boost::filesystem::is_directory(dirpath);
	}
public:
	sdn_fact_boostfs_element(const bfs_path& _path)
	 : dirpath(_path)
	{
	}

	operator bool (void) const
	{
		return ok();
	}

	bool operator ! (void) const
	{
		return !ok();
	}

	bool has_type(const std::string& type_name)
	{
		assert(ok());
		return true;
	}

	range elements(void) const
	{
		assert(ok());
		return range(dirpath);
	}

	element child(const std::string& name)
	{
		assert(ok());
		return element(dirpath / name);
	}

	property attribute(void)
	{
		assert(ok());
		return property(dirpath / "value");
	}

	property attribute(const std::string& name)
	{
		assert(!name.empty());
		assert(ok());
		return property(dirpath / name);
	}
};

sdn_fact_boostfs_element sdn_fact_boostfs_range::front(void) const
{
	assert(!empty());
	return sdn_fact_boostfs_element(*cur);
}

typedef sdn_fact_traits<
	sdn_fact_boostfs_range,
	sdn_fact_boostfs_element,
	sdn_fact_boostfs_property
> sdn_fact_boostfs_traits;

typedef factory_maker<
	sdn_fact_manuf,
	sdn_fact_suppl,
	sdn_fact_enum,
	sdn_fact_boostfs_traits
> boostfs_factory_maker;

typedef sdn_fact_data<
	sdn_fact_boostfs_traits
> boostfs_factory_input;

MIRROR_NAMESPACE_END

#endif //include guard

