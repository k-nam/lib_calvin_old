#include <string>
#include <set>
#include <unordered_set>
#include "boost/unordered_set.hpp"
#include "boost/container/set.hpp"

#include "container_test_types.h"
#include "bench.h"
#include "container_bench.h"
#include "random.h"



template <typename ElemType>
std::string
lib_calvin_benchmark::container::getTitle(OperationType operation) {
	return getString(operation);
}

template <typename ElemType>
std::string 
lib_calvin_benchmark::container::getSubCategory(OperationType operation) {
	auto result = std::string("Object size: ") + std::to_string(sizeof(ElemType)) + std::string("byte");
	if (sizeof(ElemType) >= 24) {
		result += " (vector)";
	}
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
	case LIB_CALVIN_BTREE:
		return { "lib_calvin::b_tree" };
	case LIB_CALVIN_BPLUS_TREE:
		return { "lib_calvin::b_plus_tree" };

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
	//containerBenchTemplate<lib_calvin_container::Numeric>();
	containerBenchTemplate<lib_calvin_container::LightObject>();
	containerBenchTemplate<lib_calvin_container::HeavyObject>();
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
	
	results.push_back(containerBenchSub<std::set<ElemType>>(operation));
	results.push_back(containerBenchSub<boost::container::set<ElemType>>(operation));

	results.push_back(containerBenchSub<lib_calvin_container::RbTree<ElemType>>(operation));
	results.push_back(containerBenchSub<lib_calvin_container::BTree<ElemType>>(operation));
	results.push_back(containerBenchSub<lib_calvin_container::BPlusTree<ElemType>>(operation));
	
	results.push_back(containerBenchSub<boost::unordered_set<ElemType, myHasher<ElemType>>>(operation));
	results.push_back(containerBenchSub<lib_calvin_container::HashTable<ElemType, ElemType, Identity<ElemType>, myHasher<ElemType>>>(operation));
	results.push_back(containerBenchSub<lib_calvin_container::HashTable2<ElemType, ElemType, Identity<ElemType>, myHasher<ElemType>>>(operation));
	results.push_back(containerBenchSub<std::unordered_set<ElemType, myHasher<ElemType>>>(operation));

	lib_calvin_benchmark::save_bench(category, getSubCategory<ElemType>(operation),
									 getTitle<ElemType>(operation), "",
									 getAllContainerNamesAndTags(),
									 results, testCases, unit, getOrder(operation));
}

template <typename Container>
std::vector<double>
lib_calvin_benchmark::container::containerBenchSub(OperationType operation) {	
	using namespace std;
	vector<double> result;
	typedef Container::value_type ElemType;
	lib_calvin::random_number_generator gen;
	lib_calvin::stopwatch watch;
	size_t checkSum = 0;

	std::cout << "Now testing " << std::to_string(sizeof(ElemType)) << 
		"bytes " << getString(operation) << "\n";

	for (size_t i = 0; i < numCases; i++) {
		size_t testSize = benchTestSize[i];
		size_t numIter = benchNumIter[i];
		double min = 1000;		

		for (size_t i = 0; i < numIter; i++) {
			vector<ElemType> dataArray;
			vector<ElemType> newValues;
			Container container;
			for (size_t i = 0; i < testSize; i++) {	
				ElemType elem(gen());
				dataArray.push_back(elem);	
				if (operation != GROWING) {
					container.insert(elem);
				}
			}		

			watch.start();
			switch (operation)
			{
			case GROWING:
				for (size_t i = 0; i < testSize; i++) {
					container.insert(std::move(dataArray[i]));
					checkSum += container.size();
				}
				break;
			case INSERT_DELETE:
				for (size_t i = 0; i < testSize; i++) {
					if (i % 100 < 50) {
						container.insert(std::move(dataArray[i]));
						checkSum += container.size();
					} else {
						checkSum += container.erase(dataArray[i]);
					}					
				}
				break;
			case SEARCHING:
				for (size_t i = 0; i < testSize; i++) {
					checkSum += container.count(dataArray[i]);
				}
				break;
			case ITERATING: 
				for (auto iter = container.begin(); iter != container.end(); ++iter) {
					checkSum += size_t(*iter);
				}				
				break;			
			default:
				std::cout << "containerBenchSub error!" + std::to_string(checkSum);
				exit(0);
			}
			watch.stop();
			min = std::min(min, watch.read());
		}
		double problemSize = testSize * std::log(testSize);
		double record = problemSize / min / 1000000;
		std::cout << "record was " << record << "\n";
		result.push_back(record);
	}
	return result;
}