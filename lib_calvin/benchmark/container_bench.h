#ifndef LIB_CALVIN__CONTAINER_BENCH_H
#define LIB_CALVIN__CONTAINER_BENCH_H

#include <map>
#include <vector>
#include <string>
#include <functional>

#include "container_test.h"

namespace lib_calvin_benchmark
{
	namespace container
	{
		size_t const numCases = 3;
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

			GOOGLE_BTREE,
			LIB_CALVIN_BTREE,
			LIB_CALVIN_BPLUS_TREE,

			STD_UNORDERED_SET,
			BOOST_UNORDERED_SET,
			LIB_CALVIN_HASH_TABLE,
			LIB_CALVIN_HASH_TABLE2
		};

		std::vector<ContainerType> const allContainers = { 
			STD_SET, BOOST_SET, LIB_CALVIN_RB_TREE, 
			GOOGLE_BTREE, LIB_CALVIN_BTREE, LIB_CALVIN_BPLUS_TREE,
			STD_UNORDERED_SET, BOOST_UNORDERED_SET, LIB_CALVIN_HASH_TABLE, LIB_CALVIN_HASH_TABLE2 };

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

		struct object_8 {
			object_8() {}
			object_8(size_t key): key_(key) {}
			bool operator< (object_8 const &rhs) const { return key_ < rhs.key_; }
			bool operator== (object_8 const &rhs) const { return key_ == rhs.key_; }
			static std::string to_string() { return std::to_string(sizeof(object_8)) + "byte"; }
			operator size_t () const { return key_; }
			size_t key_;
		};

		struct object_16: public object_8 {
			object_16() {}
			object_16(size_t key): object_8(key) {}
			static std::string to_string() { return std::to_string(sizeof(object_16)) + "byte"; }
			char value_[8];
		};

		struct object_32: public object_8 {
			object_32() {}
			object_32(size_t key) : object_8(key) {}
			static std::string to_string() { return std::to_string(sizeof(object_32)) + "byte"; }
			char value_[24];
		};

		struct object_64 : public object_8 {
			object_64() {}
			object_64(size_t key) : object_8(key) {}
			static std::string to_string() { return std::to_string(sizeof(object_64)) + "byte"; }
			char value_[56];
		};

		struct object_vector {
			object_vector() {}
			object_vector(size_t num) {
				for (size_t i = 0; i < 5; ++i) {
					key_.push_back(num % 100);
					num = std::hash<size_t>()(num);
				}
			}
			bool operator< (object_vector const &rhs) const { return key_ < rhs.key_; }
			bool operator== (object_vector const &rhs) const { return key_ == rhs.key_; }
			operator size_t () const {
				size_t hash = 0;
				for (auto number : key_) {
					hash *= 100;
					hash += number;
				}
				return hash;
			}
			static std::string to_string() { return std::to_string(sizeof(object_vector)) + "byte (vector)"; }
			std::vector<int> key_;
			size_t value_;
		};
	}
}
#endif
