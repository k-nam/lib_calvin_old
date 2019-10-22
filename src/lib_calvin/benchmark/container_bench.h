#ifndef LIB_CALVIN__CONTAINER_BENCH_H
#define LIB_CALVIN__CONTAINER_BENCH_H

#include <map>
#include <vector>
#include <string>
#include <functional>



namespace lib_calvin_benchmark
{
	namespace container
	{
		std::vector<size_t> const benchTestSize = { 1000, 10*1000, 1000*1000 };
		std::vector<std::string> const testCases = { "1K", "10K", "1M" };
		std::vector<size_t> const benchNumIter = { 10, 3, 1 };

		size_t const numWorkingSetCases = 2;
		// This much mega-bytes of memory will be working area
		std::vector<size_t> const workingSetSize = { 0, 32*1000*1000 };
		static size_t currentWorkingSetSize;

		std::string const unit = "M/s (higher is better)";
		std::string const category = "Container";

		enum OperationType {
			GROWING,
			INSERT_DELETE,
			SEARCHING,
			ITERATING
		};

		static OperationType currentOperation;

		enum ContainerType {
			STD_SET,
			BOOST_SET,
			LIB_CALVIN_RB_TREE,
			LIB_CALVIN_RB_TREE_HEAD_NODE,

			GOOGLE_BTREE,
			LIB_CALVIN_BTREE,
			LIB_CALVIN_BPLUS_TREE,

			STD_UNORDERED_SET,
			BOOST_UNORDERED_SET,
			LIB_CALVIN_HASH_TABLE,
			LIB_CALVIN_HASH_TABLE2
		};

		std::vector<ContainerType> const allContainers = { 
			STD_SET, BOOST_SET, LIB_CALVIN_RB_TREE, LIB_CALVIN_RB_TREE_HEAD_NODE,
			GOOGLE_BTREE, LIB_CALVIN_BTREE, LIB_CALVIN_BPLUS_TREE,
			STD_UNORDERED_SET, BOOST_UNORDERED_SET, LIB_CALVIN_HASH_TABLE };

		template <typename ElemType>
		struct myHasher {
			size_t operator() (ElemType const &elem) const {
				return std::hash<size_t>()(size_t(elem));
			}
		};

		template <typename ElemType>
		std::string getTitle(OperationType);

		template <typename ElemType>
		std::string getSubCategory();

		std::string getString(OperationType);
		std::string getString(ContainerType);
		int getOrder(OperationType);

		std::vector<std::string> getContainerNamesAndTags(ContainerType);

		std::vector<std::vector<std::string>> getContainerNamesAndTagsVector(std::vector<ContainerType>);

		std::vector<std::vector<std::string>> getAllContainerNamesAndTags();

		void containerBench();

		void containerBench(size_t workingSetSize);

		template <typename ElemType>
		void containerBenchTemplate();

		template <typename ElemType>
		void containerBenchTemplate(OperationType);

		template <typename ContainerType>
		std::vector<double>
			containerBenchSub();


	}
}
#endif
