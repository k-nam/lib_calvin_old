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
	return getString(operation) + " " + std::to_string(sizeof(ElemType)) + "bytes objets";
}

std::string
lib_calvin_benchmark::container::getString(OperationType operation) {
	switch (operation) {
	case INSERTING:
		return "Inserting";
	case SEARCHING:
		return "Searching";
	case ITERATING:
		return "Iterating";
	case DELETING:
		return "Deleting";
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
	case INSERTING:
		return 0;
	case SEARCHING:
		return 1;
	case ITERATING:
		return 2;
	case DELETING:
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
		return { "std::set", "std", "tree" };
	case BOOST_SET:
		return { "boost::set", "boost", "tree" };

	case LIB_CALVIN_RB_TREE:
		return { "lib_calvin::rb_tree", "lib_calvin", "tree" };
	case LIB_CALVIN_BTREE:
		return { "lib_calvin::b_tree", "lib_calvin", "tree" };
	case LIB_CALVIN_BPLUS_TREE:
		return { "lib_calvin::b_plus_tree", "lib_calvin", "tree" };

	case STD_UNORDERED_SET:
		return { "std::unordered_set", "std", "hash table" };
	case BOOST_UNORDERED_SET:
		return { "boost::unordered_set", "boost", "hash table" };
	case LIB_CALVIN_HASH_TABLE:
		return { "lib_calvin::hash_table", "lib_calvin", "hash table" };
	case LIB_CALVIN_HASH_TABLE2:
		return { "lib_calvin::hash_table2", "lib_calvin", "hash table" };
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
	containerBenchTemplate<ElemType>(INSERTING);
	containerBenchTemplate<ElemType>(SEARCHING);
	containerBenchTemplate<ElemType>(ITERATING);
	containerBenchTemplate<ElemType>(DELETING);
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

	string subCategory(std::string("Object size: ") + std::to_string(sizeof(ElemType)) + std::string("bytes"));
	lib_calvin_benchmark::save_bench(category, subCategory,
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
				container.insert(elem);
				newValues.push_back(gen());
			}		

			watch.start();
			switch (operation)
			{
			case INSERTING:
				for (size_t i = 0; i < testSize; i++) {
					container.insert(newValues[i]);
					checkSum += container.size();
				}
				break;
			case SEARCHING:
				for (size_t i = 0; i < testSize; i++) {
					checkSum += container.count(dataArray[i]);
				}
				break;
			case ITERATING: {
				auto iter = container.begin();
				for (size_t i = 0; i < container.size(); i++) {
				//for (auto iter = container.begin(); iter != container.end(); ++iter) {
					checkSum += size_t(*iter);
					if (i < testSize - 1) {
						++iter;
					}
				}
				break;
			}
			case DELETING:
				for (size_t i = 0; i < testSize; i++) {
					checkSum += container.erase(dataArray[i]);;
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