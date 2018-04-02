#ifndef LIB_CALVIN__CONTAINER__HASH_MAP_H
#define LIB_CALVIN__CONTAINER__HASH_MAP_H

#include "container.h"
#include "hash_table.h"
#include "hash_table2.h"
#include "boost/container/map.hpp"
#include "boost/unordered_map.hpp"
#include <unordered_map>
#include "map.h"
#include "hash_set.h"

namespace lib_calvin_container
{

	template <typename K, typename V, typename HashFunc = boost::hash<K>>
	using HashMap = MapWrapper<K, V, void, HashSetImpl<std::pair<K, V>, K, TakeFirstOfPair<K, V>, HashFunc>>;

} 

namespace lib_calvin
{
	template <typename K, typename V>
	class hash_map : public lib_calvin_container::HashMap<K, V> { };
} 

#endif