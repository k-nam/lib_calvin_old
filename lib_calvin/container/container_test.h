#ifndef LIB_CALVIN__CONTAINER__CONTAINER_TEST_H
#define LIB_CALVIN__CONTAINER__CONTAINER_TEST_H

#include "adt_test.h"
#include "b_tree_test.h"
#include "vector_test.h"
#include "set_test.h"
#include "map_test.h"
#include "vector.h"
#include "set.h"
#include <vector>

namespace lib_calvin_container
{
void containerTest();


template <typename T>
class GenericHash {
public:
	size_t operator()(T const &object) const { return boost::hash<void *>()((void *)&object); }
};
}

#endif