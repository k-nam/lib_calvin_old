#ifndef LIB_CALVIN__ETC_BENCH_H
#define LIB_CALVIN__ETC_BENCH_H



#include "simple_graph.h"

namespace lib_calvin_benchmark
{
	namespace etc
	{
		static size_t const numGroups = 100;

		void etc_bench();

		void memory_access_bench();
		 
		void linear_time_sorting_bench();

		void sort_by_group_bench();

		std::vector<double> linked_list_access(std::vector<size_t>);
		std::vector<double> pointer_array_access(std::vector<size_t>);

		std::vector<double> linear_method(std::vector<size_t>);
		std::vector<double> linear_method_inplace(std::vector<size_t>);
		std::vector<double> block_qsort(std::vector<size_t>);
		
		std::vector<double> one_pass_sorting(std::vector<size_t>);
		std::vector<double> two_pass_sorting(std::vector<size_t>);


		struct Node {
			size_t key_;
			size_t group_;
			size_t data_[2];
			operator size_t() const { return key_ + data_[0] + data_[1]; }
			Node *next_;

			bool operator< (Node const &rhs) const {
				return key_ < rhs.key_;
			}

			bool operator== (Node const &rhs) const {
				return key_ == rhs.key_;
			}
		};

		class key_compare {
		public:
			bool operator()(Node const &node1, Node const &node2) const {
				return node1.key_ < node2.key_;
			}
		};

		class group_compare {
		public:
			bool operator()(Node const &node1, Node const &node2) const {
				return node1.group_ < node2.group_;
			}
		};

		class all_compare {
		public:
			bool operator()(Node const &node1, Node const &node2) const {
				return node1.group_ < node2.group_ ||
					(node1.group_ == node2.group_ && node1.key_ < node2.key_);
			}
		};


		std::vector<Node> getRandomNodeArray(size_t size);
	}
}

#endif
