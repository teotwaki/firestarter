#ifndef __SIMPLECACHE_H
#define __SIMPLECACHE_H

template<typename T> class SimpleCache {
	public:
	T * cache;

	SimpleCache(T * _cache) : cache(_cache) { }

	T * getCache() { return this->cache; };
	void setCache(T * _cache) { this->cache = _cache; };
	virtual bool cacheIsValid() { return (this->getCache() != NULL) ? true : false; };
	virtual void invalidateCache() { if (this->cacheIsValid()) { delete this->getCache(); this->setCache(NULL); } };
	virtual void initCache() { if (not this->cacheIsValid()) { this->setCache(new T); } };
};

#endif
