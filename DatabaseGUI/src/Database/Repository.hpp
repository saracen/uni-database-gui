#ifndef __REPOSITORY_HPP__
#define __REPOSITORY_HPP__

#include <list>

namespace Database
{

/**
 * The repository class is a base class that implements a storage area
 * and iterator for a collection of entities.
 *
 * The add and remove methods are expected to be implemented via a child
 * class, a repository handling a specific entity. This repository will
 * also index said entity if required.
 */
template <class T>
class Repository {
public:
	/**
	 * Iterator class used for iteration over the stored entities
	 */
	class Iterator
	{
		unsigned int index;
		const Repository<T> *repo;

	public:
		Iterator(const Repository<T> *repo, unsigned int index) : repo(repo), index(index) {
		}
 
		bool operator!=(const Iterator &other) const {
			return index != other.index;
		}
 
		Iterator operator++() {
			++index;
			return *this;
		}
 
		const T *operator->() const {
			return &repo->storage.at(index);
		}
 
		const T &operator*() const {
			return repo->storage.at(index);
		}
	};

	virtual bool Add(const T &item) = 0;
	virtual bool Remove(const T &item) = 0;
	
	Iterator Begin() const {
		// Start of iterator
		return Iterator(this, 0);
	}
 
	Iterator End() const {
		// End of iterator
		return Iterator(this, storage.size());
	}
 
protected:
	friend class Iterator;
	std::list<T> storage;
};

};

#endif