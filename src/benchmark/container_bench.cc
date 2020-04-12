#include <string>
#include <set>
#include <unordered_set>
#include <algorithm>

#include "container_bench.h"
#include "rb_tree.h"
#include "rb_plus_tree.h"
#include "boost/unordered_set.hpp"
#include "boost/container/set.hpp"
#include "google/btree/google_btree_set.h"
#include "hash_table.h"
#include "hash_table2.h"
#include "container_test_types.h"
#include "bench.h"
#include "random.h"
#include "vector.h"
#include "stopwatch.h"

using namespace lib_calvin_benchmark::container;

template <typename ElemType>
std::string
lib_calvin_benchmark::container::getTitle(OperationType operation) {
	return getString(operation);
}

template <typename ElemType>
std::string
lib_calvin_benchmark::container::getSubCategory() {
	auto result = ElemType::to_string();
	result += " / ";
	if (currentWorkingSetSize == 0) {
		result += "minimum";
	} else {
		result += std::to_string(currentWorkingSetSize /  1000 / 1000) + "MB";
	}
	result += " working set";
	return result;
}

std::string
lib_calvin_benchmark::container::getString(OperationType operation) {
	switch (operation) {
	case GROWING:
		return "Growing";
	case INSERT_DELETE:
		return "Insert/Delete";
	case SEARCHING:
		return "Searching";
	case ITERATING:
		return "Iterating";
	default:
		std::cout << "getString error!\n";
		exit(0);
	}
}

std::string
lib_calvin_benchmark::container::getString(ContainerType container) {
	return getContainerNamesAndTags(container)[0];
}

int
lib_calvin_benchmark::container::getOrder(OperationType operation) {
	switch (operation) {
	case GROWING:
		return 0;
	case INSERT_DELETE:
		return 1;
	case SEARCHING:
		return 2;
	case ITERATING:
		return 3;
	default:
		std::cout << "getOrder error!\n";
		exit(0);
	}
}


std::vector<std::string>
lib_calvin_benchmark::container::getContainerNamesAndTags(ContainerType container) {
	switch (container) {
	case STD_SET:
		return { "std::set"};
	case BOOST_SET:
		return { "boost::set" };
	case LIB_CALVIN_RB_TREE:
		return { "lib_calvin::rb_tree" };
	case LIB_CALVIN_RB_TREE_HEAD_NODE:
		return { "lib_calvin::rb_tree+" };

	case GOOGLE_BTREE:
		return { "google::b_tree", "B-tree" };
	case LIB_CALVIN_BTREE:
		return { "lib_calvin::b_tree", "B-tree" };
	case LIB_CALVIN_BPLUS_TREE:
		return { "lib_calvin::b_plus_tree", "B-tree" };

	case STD_UNORDERED_SET:
		return { "std::unordered_set", "hash table" };
	case BOOST_UNORDERED_SET:
		return { "boost::unordered_set", "hash table" };
	case LIB_CALVIN_HASH_TABLE:
		return { "lib_calvin::hash_table",  "hash table" };
	case LIB_CALVIN_HASH_TABLE2:
		return { "lib_calvin::hash_table2",  "hash table" };
	default:
		std::cout << "getString error!\n";
		exit(0);
	}
}

std::vector<std::vector<std::string>>
lib_calvin_benchmark::container::getContainerNamesAndTagsVector(std::vector<ContainerType> containers) {
	std::vector<std::vector<std::string>> result;
	for (auto container : containers) {
		result.push_back(getContainerNamesAndTags(container));
	}
	return result;
}

std::vector<std::vector<std::string>>
lib_calvin_benchmark::container::getAllContainerNamesAndTags() {
	return getContainerNamesAndTagsVector(allContainers);
}

void
lib_calvin_benchmark::container::containerBench() {
	for (auto size : workingSetSize) {
		containerBench(size);
	}
}

void
lib_calvin_benchmark::container::containerBench(size_t workingSetSize) {
	using namespace lib_calvin_benchmark;
	currentWorkingSetSize = workingSetSize;
	containerBenchTemplate<object_16>();
	containerBenchTemplate<object_32>();
	containerBenchTemplate<object_64>();
	containerBenchTemplate<object_vector>();

	//containerBenchTemplate<lib_calvin_container::LightObject>();
	//containerBenchTemplate<lib_calvin_container::HeavyObject>();
}

template <typename ElemType>
void lib_calvin_benchmark::container::containerBenchTemplate() {
	containerBenchTemplate<ElemType>(GROWING);
	containerBenchTemplate<ElemType>(INSERT_DELETE);
	containerBenchTemplate<ElemType>(SEARCHING);
	containerBenchTemplate<ElemType>(ITERATING);
}

template <typename ElemType>
void lib_calvin_benchmark::container::containerBenchTemplate(OperationType operation) {
	using std::vector;
	using std::string;
	using lib_calvin_container::Identity;
	vector<vector<double>> results;

	currentOperation = operation;

	results.push_back(containerBenchSub<std::set<ElemType>>());
	results.push_back(containerBenchSub<boost::container::set<ElemType>>());
	results.push_back(containerBenchSub<lib_calvin_container::RbTree<ElemType>>());
	results.push_back(containerBenchSub<lib_calvin_container::RbPTreeWithHead<ElemType>>());

	results.push_back(containerBenchSub<btree::btree_set<ElemType>>()); //Google's
	results.push_back(containerBenchSub<lib_calvin_container::BTree<ElemType>>());
	results.push_back(containerBenchSub<lib_calvin_container::BPlusTree<ElemType>>());

	results.push_back(containerBenchSub<std::unordered_set<ElemType, myHasher<ElemType>>>());
	results.push_back(containerBenchSub<boost::unordered_set<ElemType, myHasher<ElemType>>>());
	results.push_back(containerBenchSub<lib_calvin_container::HashTable<ElemType, ElemType, Identity<ElemType>, myHasher<ElemType>>>());

	auto testCases = lib_calvin_benchmark::getSizeStrings(benchTestSize);

	lib_calvin_benchmark::save_bench(category, getSubCategory<ElemType>(),
									 getTitle<ElemType>(currentOperation), "",
									 getAllContainerNamesAndTags(),
									 results, testCases, unit, getOrder(currentOperation));
}

template <typename Container>
std::vector<double>
lib_calvin_benchmark::container::containerBenchSub() {
	using namespace std;
	vector<double> result;
	typedef typename Container::value_type ElemType;
	lib_calvin::random_number_generator gen;
	lib_calvin::stopwatch watch;
	size_t checkSum = 0;

	std::cout << "Now testing " << std::to_string(sizeof(ElemType)) <<
		"bytes " << getString(currentOperation) << " working size: " << currentWorkingSetSize << "\n";

	for (size_t i = 0; i < benchTestSize.size(); i++) {
		size_t testSize = benchTestSize[i];
		size_t numContainers = std::max(
			currentWorkingSetSize / (sizeof(ElemType) * testSize), static_cast<size_t>(1));
		size_t totalNumOps = testSize * numContainers;
		size_t numIter = benchNumIter[i];
		if (currentWorkingSetSize > 0 || ElemType::to_string().size() > 6) {
			numIter = 1;
		}
		double min = 1000;
		size_t problemSize = totalNumOps * static_cast<size_t>(std::log(testSize));
		//std::cout << "Test size " << testSize << "numContainers " << numContainers << "\n";
		for (size_t i = 0; i < numIter; i++) {
			vector<ElemType> dataArray;
			vector<ElemType> newValues;
			lib_calvin::vector<Container> containers(numContainers, Container());
			for (size_t i = 0; i < testSize; i++) {
				for (auto &container : containers) {
					ElemType elem(gen());
					dataArray.push_back(elem);
					if (currentOperation != GROWING) {
						container.insert(elem);
					}
				}
			}

			watch.start();
			switch (currentOperation)
			{
			case GROWING:
				for (size_t i = 0; i < testSize; i++) {
					for (size_t j = 0; j < numContainers; j++) {
						containers[j].insert(std::move(dataArray[i * numContainers + j]));
						checkSum += containers[j].size();
					}
				}
				break;
			case INSERT_DELETE:
				for (size_t i = 0; i < testSize; i++) {
					for (size_t j = 0; j < numContainers; j++) {
						if (i % 100 < 50) {
							containers[j].insert(std::move(dataArray[i * numContainers + j]));
							checkSum += containers[j].size();
						} else {
							checkSum += containers[j].erase(dataArray[i * numContainers + j]);
						}
					}
				}
				break;
			case SEARCHING:
				for (size_t i = 0; i < testSize; i++) {
					for (size_t j = 0; j < numContainers; j++) {
						checkSum += containers[j].count(dataArray[i * numContainers + j]);
					}
				}
				break;
			case ITERATING: {
				problemSize = totalNumOps;
				for (size_t j = 0; j < numContainers; j++) {
					//std::cout << "iter size: " << containers[j].size() << "\n";
					for (auto iter = containers[j].begin(); iter != containers[j].end(); ++iter) {
						checkSum += *iter;
					}
				}
				break;
			}
			default:
				std::cout << "containerBenchSub error!" + std::to_string(checkSum);
				exit(0);
			}
			watch.stop();
			min = std::min(min, watch.read());
		}

		double record = problemSize / min / 1000000;
		std::cout << "record was " << record << " checksum: " << checkSum << "\n";
		result.push_back(record);
	}
	return result;
}