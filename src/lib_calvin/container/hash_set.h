#ifndef LIB_CALVIN__CONTAINTER__HASH_SET_H
#define LIB_CALVIN__CONTAINTER__HASH_SET_H

#include "hash_table.h"
#include "hash_table2.h"
#include <set>
#include "set.h"

namespace lib_calvin_container
{
	template <typename T, typename K = T, typename ExtractKey = Identity<T>, typename HashFunc = boost::hash<T>>
	using HashSetImpl =
		lib_calvin_container::HashTable<T, K, ExtractKey, HashFunc>;

	template <typename T, typename HashFunc>
	using HashSet = HashSetImpl<T, T, Identity<T>, HashFunc>;
} 

namespace lib_calvin
{

	template <typename T, typename HashFunc>
	using hash_set = lib_calvin_container::HashSet<T, HashFunc>;

} // end namespace lib_calvin
#endif