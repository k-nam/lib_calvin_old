#ifndef LIB_CALVIN__ETC_BENCH_H
#define LIB_CALVIN__ETC_BENCH_H



#include "simple_graph.h"

namespace lib_calvin_benchmark
{
	namespace etc
	{
		void etc_bench();

		void memory_access_bench(size_t);

		std::vector<double> linked_list_access(size_t);
		std::vector<double> pointer_array_access(size_t);

		struct Node {
			size_t key_;
			size_t data_[3];
			operator size_t() const { return key_ + data_[0] + data_[1] + data_[2]; }
			Node *next_;
		};
	}
}

#endif
