#ifndef LIB_CALVIN__ETC_BENCH_H
#define LIB_CALVIN__ETC_BENCH_H



#include "simple_graph.h"

namespace lib_calvin_benchmark
{
	namespace etc
	{
		static size_t const numGroups = 100;

		std::vector<std::string> const testCases = { "1K", "10K", "100K", "1M", "10M" };
		std::vector<size_t> const testSizes = { 1000, 1000*10, 1000*100, 1000*1000, 1000*1000*10 };
		std::vector<size_t> const numIters = { 1000, 100, 10, 10, 3 };


		void etc_bench();

		void memory_access_bench();
		 
		void linear_time_sorting_bench();

		void sort_by_group_bench();

		void following_link_bench();

		void n_nlogn_bench();

		std::vector<double> linked_list_access();
		std::vector<double> pointer_array_access();

		std::vector<double> linear_method();
		std::vector<double> linear_method_inplace();
		std::vector<double> block_qsort();
		
		std::vector<double> one_pass_sorting();
		std::vector<double> two_pass_sorting();

		std::vector<double> null_pointer();
		std::vector<double> null_flag();

		std::vector<double> hashing();
		std::vector<double> build_tree();
		std::vector<double> sorting();
		std::vector<double> block_qsort_int();

		std::vector<size_t> getRandomIntArray(size_t size);

		struct Node {
			size_t key_;
			size_t group_;
			size_t data_[2];
			bool is_null;

			operator size_t() const { return key_ + data_[0] + data_[1]; }
			Node *next_;


			bool operator== (Node const &rhs) const {
				return key_ == rhs.key_ && group_ == rhs.group_;
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
