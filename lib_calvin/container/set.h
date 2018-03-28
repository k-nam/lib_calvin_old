#ifndef LIB_CALVIN__CONTAINTER__SET_H
#define LIB_CALVIN__CONTAINTER__SET_H


#include "boost/container/set.hpp"
#include "container.h"
#include "bin_tree.h"
#include "b_tree.h"
#include "b_plus_tree.h"
#include "rb_tree.h"
#include "ordered_array.h"
#include "hash_table.h"
#include <set>

namespace lib_calvin_container
{
template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = Identity<T>>
using SetImpl = 
	lib_calvin_container::BPlusTree<T, K, Comp, ExtractKey>;
	//lib_calvin_container::RbTree<T, K, Comp, ExtractKey>;
	//lib_calvin_container::BinTree<T, K, Comp, ExtractKey>;

template <typename T, typename K = T, typename ExtractKey = Identity<T>, typename HashFunc = boost::hash<T>>
using HashSetImpl = 
	lib_calvin_container::HashTable<T, K, ExtractKey, HashFunc>;

template <typename T, typename Comp = std::less<K>>
using Set = SetImpl<T, T, Comp>;

template <typename T, typename HashFunc>
using HashSet = HashSetImpl<T, T, Identity<T>, HashFunc>;
} // end namespace lib_calvin_container

namespace lib_calvin
{
template <typename T, typename Comp = std::less<T>>
using set = lib_calvin_container::Set<T, Comp>;
//using set = public boost::container::set<T, Comp>;
//using set = public std::set<T, Comp>;

template <typename T, typename HashFunc>
using hash_set = lib_calvin_container::HashSet<T, HashFunc>;

} // end namespace lib_calvin
#endif