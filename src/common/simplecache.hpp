#ifndef FIRESTARTER_SIMPLECACHE_HPP
#define FIRESTARTER_SIMPLECACHE_HPP

/** \brief Simple caching base class
  *
  * This template provides an extremely basic way of implementing caching.
  * The template argument is the data type that needs to be cached. The data gets deleted automatically when it is invalidated or when the object is destroyed.
  * SimpleCache is mainly used by DependencyGraph.
  *
  * This class can be used both through inheritance or composition.
  * Inheritance example:
  * \code
  * // Let's assume there is a class called BigDataType:
  * class MyExample : private SimpleCache<BigDataType> {
  *     public:
  *     MyExample() : SimpleCache<BigDataType>() { this->initCache(); };
  *     insertData(Data foo) { this->getCache()->add(foo); };
  *     deleteData() { this->invalidateCache(); };
  *     // ...
  * };
  * \endcode
  * It is possible to override any of the methods in order to adapt the structure initialisation / assignment to your needs.
  *
  * Object composition example:
  * \code
  * class MySecondExample {
  *     private:
  *     SimpleCache<BigDataType> cache;
  *
  *     public:
  *     insertData(Data foo) { if (not this->cache->cacheIsValid()) this->cache->initCache(); this->cache->getCache()->add(foo); };
  *     deleteData() { this->cache->invalidateCache(); };
  *     // ...
  * };
  * \endcode
  *
  * \see For an example implementation, see DependencyGraph
  */
template<typename T> class SimpleCache {
	protected:
	/// \brief The pointer to the data that will be held
	T * cache;

	public:
	/** \brief Initialises the object with no cache
	  *
	  * Call initCache() to initialise the internal cache and storage.
	  */
	SimpleCache() : cache(NULL) { };
	/// \brief Destroy the object and delete the cache if it exists
	~SimpleCache() { if (this->cacheIsValid()) delete this->getCache(); };

	/** \brief Return a pointer to the cache
	  *
	  * \warning This pointer becomes invalid when invalidateCache() is called.
	  */
	T * getCache() { return this->cache; };
	/** \brief Give a new value to the pointer
	  *
	  * This function should not be used directly. Rather, use initCache() to instantiate the data.
	  * \see initCache
	  */
	virtual void setCache(/** [in] */ T * _cache) { this->cache = _cache; };
	/** \brief Check if the cache is in a valid state or not
	  *
	  * \return true if the cache is initialised, false otherwise
	  */
	virtual bool cacheIsValid() { return (this->getCache() != NULL); };
	/** \brief Delete and invalidate the cache if it was valid
	  *
	  * \warning If a pointer was issued before calling invalidateCache(), it can no longer be used after
	  * a call to invalidateCache().
	  */
	virtual void invalidateCache() { if (this->cacheIsValid()) { delete this->getCache(); this->setCache(NULL); } };
	/** \brief Initialise a new cache
	  *
	  * Note that if the cache is valid, it will not be overwritten. Call invalidateCache() first.
	  */
	virtual void initCache() { if (not this->cacheIsValid()) { this->setCache(new T); } };
};

#endif
