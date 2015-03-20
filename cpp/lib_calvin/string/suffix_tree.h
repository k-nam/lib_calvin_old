#ifndef LIB_CALVIN__STRING__SUFFIX_TEST_H
#define LIB_CALVIN__STRING__SUFFIX_TEST_H

#include "abstract_string.h"
#include "graph.h"

namespace lib_calvin
{
template <typename Alphabet>
class suffix_tree {
public:
	suffix_tree(abstract_string<Alphabet> text);
};



template <typename Alphabet>
suffix_tree<Alphabet>::suffix_tree(abstract_string<Alphabet> text) {
}

}

#endif