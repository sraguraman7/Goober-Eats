

// ExpandableHashMap.h
#ifndef ExpandableHashMap_h
#define ExpandableHashMap_h

#include <functional>
#include <vector>
#include <list>

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5)	//init hash map
	{
		if (maximumLoadFactor <= 0)	
			m_maxload = 0.5;
		else 
			m_maxload = maximumLoadFactor;

		construct();
	}
	~ExpandableHashMap()
	{
		destruct();
	}
	void reset()
	{
		vec->clear();		//clear vec, destroy it, and recreate it
		destruct();
		construct();
	}

	int size() const
	{
		return m_size;
	}
	void associate(const KeyType& key, const ValueType& value)
	{
		unsigned int buckNum = getBucketNumber(key);		//get hash bucket
		std::list<pair>* listptr = &(*vec)[buckNum];		//find the list it should be in if it exits, or should go into if it doesn't
		auto it = listptr->begin();
		bool broke = false;
		while (it != listptr->end())		//go through list to see if it's there
		{
			if ((*it).k == key)
			{
				(*it).v = value;
				broke = true;		//if it is, change value
				break;
			}
			else
				it++;
		}
		if (!broke)		//if it wasn't in the list, add it
		{
			m_size++;
			pair x(key, value);
			listptr->push_back(x);
		}

		double newload = (double)(m_size) / (double)m_buckets;		//check load factor
		if (newload > m_maxload)
		{
			m_buckets *= 2;		//buckets are two times as many
			auto oldvec = vec;
			vec = nullptr;
			vec = new std::vector<std::list<pair>>; //new pointer to vec, and create a new vec that's 2 times as big
			vec->resize(m_buckets);

			for (int i = 0; i != oldvec->size()/2; i++)	//go through old vec
			{
				
					auto it = (*oldvec)[i].begin();
					while (it != (*oldvec)[i].end()) 
					{
						unsigned int buckNum = getBucketNumber(it->k);		//go through each list of old vec
						std::list<pair>* listptr = &(*vec)[buckNum];
							pair x(it->k, it->v);		//add each pair over into the appropriate list of new vector
							listptr->push_back(x);
						it++;
					}
			}
			delete oldvec;
		}
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const
	{
		unsigned int buckNum = getBucketNumber(key);
		std::list<pair>* listptr = &(*vec)[buckNum];		//find appropriate bucket
		auto it = listptr->begin();
		while (it != listptr->end())		//if it's in the list, return a pointer to the val. Otherwise, nullptr
		{
			if (it->k == key)
				return &(it->v);
			it++;
		}
		return nullptr;
	}
	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
	unsigned int getBucketNumber(const KeyType& key) const
	{
		unsigned int hasher(const KeyType & k); //prototype
		unsigned int h = hasher(key);
		h = h % m_buckets;
		return h;
	}
	void construct()
	{
		m_size = 0;		//initialize the hash map, create new vec
		m_buckets = 8;
		vec = new std::vector<std::list<pair>>;
		vec->resize(m_buckets);
	}
	void destruct()
	{
		delete vec;		//delete the allocated memory
	}
	struct pair 
	{
		KeyType k;
		ValueType v;
		pair(KeyType key, ValueType value)		//created pair struct, I tried using utility pair<KeyType, ValueType> but it would not compile
		{
			k = key;
			v = value;
		}
	};
	std::vector<std::list<pair>>* vec;		//ptr to vector
	double m_maxload;
	int m_size;
	int m_buckets;
};

#endif