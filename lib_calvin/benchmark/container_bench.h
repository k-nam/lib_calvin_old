#ifndef LIB_CALVIN__CONTAINER_BENCH_H
#define LIB_CALVIN__CONTAINER_BENCH_H

#include <map>
#include <vector>
#include <string>
#include <functional>

#include "container_test.h"
#include "boost/functional/hash.hpp"

namespace lib_calvin_benchmark
{
	namespace container
	{	
		size_t const numCases = 3;
		std::vector<size_t> const benchTestSize = { 1000, 10*1000, 1000*1000 };
		std::vector<std::string> const testCases = { "1K", "10K", "1M" };
		std::vector<size_t> const benchNumIter = { 1000, 10, 3 };
		std::string const unit = "M/s (higher is better)";
		std::string const category = "Container";

		enum OperationType {
			INSERTING,
			SEARCHING,
			ITERATING,
			DELETING
		};

		enum ContainerType {
			STD_SET,
			BOOST_SET,

			LIB_CALVIN_RB_TREE,
			LIB_CALVIN_BTREE,
			LIB_CALVIN_BPLUS_TREE,
			
			BOOST_UNORDERED_SET,
			LIB_CALVIN_HASH_TABLE,
			LIB_CALVIN_HASH_TABLE2,
			STD_UNORDERED_SET
		};

		std::vector<ContainerType> const allContainers = { STD_SET, BOOST_SET,
			LIB_CALVIN_RB_TREE, LIB_CALVIN_BTREE, LIB_CALVIN_BPLUS_TREE,
			BOOST_UNORDERED_SET, LIB_CALVIN_HASH_TABLE, LIB_CALVIN_HASH_TABLE2, STD_UNORDERED_SET };

		template <typename ElemType>
		struct myHasher {
			size_t operator() (ElemType const &elem) const { 
				return std::hash<size_t>()(size_t(elem));
			}
		};
		/*
		template <>
		struct myHasher<lib_calvin_container::LightObject> {
			size_t operator() (lib_calvin_container::LightObject const &elem) const {
				return boost::hash<size_t>()(size_t(elem));
			}
		};

		template <>
		struct myHasher<lib_calvin_container::HeavyObject> {
			size_t operator() (lib_calvin_container::HeavyObject const &) const {
				return boost::hash(size_t(elem));
			}
		};
		*/

		template <typename ElemType>
		std::string getTitle(OperationType);

		std::string getString(OperationType);
		std::string getString(ContainerType);
		int getOrder(OperationType);

		std::vector<std::string> getContainerNamesAndTags(ContainerType);

		std::vector<std::vector<std::string>> getContainerNamesAndTagsVector(std::vector<ContainerType>);

		std::vector<std::vector<std::string>> getAllContainerNamesAndTags();

		void containerBench();

		template <typename ElemType>
		void containerBenchTemplate();

		template <typename ElemType>
		void containerBenchTemplate(OperationType);

		template <typename ContainerType>
		std::vector<double>
			containerBenchSub(OperationType);
	}
}
#endif
