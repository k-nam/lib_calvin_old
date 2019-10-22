#ifndef LIB_CALVIN__CONTAINTER__SET_H
#define LIB_CALVIN__CONTAINTER__SET_H


#include "container.h"
#include "bin_tree.h"
#include "b_tree.h"
#include "b_plus_tree.h"
#include "rb_tree.h"
#include "ordered_array.h"
#include <set>

namespace lib_calvin
{
	template <typename T, typename Comp = std::less<T>>
	using set = lib_calvin_container::RbTree<T, T, Comp>;

	//using set = public boost::container::set<T, Comp>;
	//using set = public std::set<T, Comp>;

	template <typename T, typename Comp = std::less<T>>
	using btree_set = lib_calvin_container::BTree<T, T, Comp>;

} // end namespace lib_calvin
#endif