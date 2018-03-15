#ifndef LIB_CALVIN__CONTAINER__MAP_H
#define LIB_CALVIN__CONTAINER__MAP_H

#include "container.h"
#include "b_tree.h"
#include "b_plus_tree.h"
#include "set.h"
#include "bin_tree.h"
#include "rb_tree.h"
#include "hash_table.h"
#include "hash_table2.h"
#include "boost/container/map.hpp"
#include "boost/unordered_map.hpp"
#include <unordered_map>
#include <map>

namespace lib_calvin_container
{
	// This class makes it easy to switch among various implementations
	// We use another class for test wrapping purpose: SetAdaptor, as 
	// insert method's return type contains iterator, which can not be included
	// in the base class. 
template <typename K, typename V, typename Comp, typename Set>
class MapWrapper: public Set {
public:
	typedef K key_type;
	typedef V mapped_type;

	mapped_type & operator[](key_type const &key)
	{ return insert(std::pair<K, V>(key, V())).first->second; }
	mapped_type & operator[](key_type &&key)
	{ return insert(std::pair<K, V>(std::forward<key_type>(key), V())).first->second; }
};

template <typename K, typename V, typename Comp = std::less<K>>
using Map = MapWrapper<K, V, Comp, SetImpl<std::pair<K, V>, K, Comp, TakeFirstOfPair<K, V>>>;

template <typename K, typename V, typename HashFunc = boost::hash<K>>
using HashMap = MapWrapper<K, V, void, HashSetImpl<std::pair<K, V>, K, TakeFirstOfPair<K, V>, HashFunc>>;

} // end lib_calvin_container

namespace lib_calvin
{
template <typename K, typename V>
class hash_map: public lib_calvin_container::HashMap<K, V> { };

template <typename K, typename V>
class map: public lib_calvin_container::Map<K, V> { };
//class map: public boost::container::map<K, V> { };
//class map: public std::map<K, V> { };
//class map: public lib_calvin::hash_map<K, V> { };
//class map: public std::unordered_map<K, V> { };
//class map: public boost::unordered_map<K, V> { };

} // end namespace lib_calvin

#endif