#ifndef LIB_CALVIN__UTIL__SAVE_BENCH_H
#define LIB_CALVIN__UTIL__SAVE_BENCH_H

#include <string>
#include <vector>
#include <map>
#include <fstream>


namespace lib_calvin_benchmark
{
	std::string const saveFilePath = "../../../bench_result/";
	std::string const environment = "Intel 8700 with 16GB ram, Windows 10, Visual C++ v141";

	std::string getDatetime();

	std::string getSizeString(size_t);
	std::vector<std::string> getSizeStrings(std::vector<size_t>);


	// algorithms: [["qsort", "unstable", "std"], ["parallel_block_qsort", "unstable", "lib_calvin"], ...]
	void save_bench(std::string category, std::string subCategory, std::string title, std::string comment,
					std::vector<std::vector<std::string>> algorithms,
					std::vector<std::vector<double>> results,
					std::vector<std::string> testCases, std::string unit, size_t order);
}

namespace lib_calvin_benchmark 
{
	struct object_8 {
		object_8() {}
		object_8(size_t key) : key_(key) {}
		object_8(object_8 const &rhs) { key_ = rhs.key_; }
		bool operator< (object_8 const &rhs) const { return key_ < rhs.key_; }
		bool operator== (object_8 const &rhs) const { return key_ == rhs.key_; }
		static std::string to_string() { return std::to_string(sizeof(object_8)) + "byte"; }
		operator size_t () const { return key_; }
		size_t key_;
	};

	template <size_t byte>
	struct dummy {
		dummy() {
			for (size_t i = 0; i < byte; i++) {
				data_[i] = i;
			}
		}
		dummy(dummy const &rhs) {
			*this = rhs;
		}

		dummy & operator=(dummy const &rhs) {
			for (size_t i = 0; i < byte; i++) {
				data_[i] = rhs.data_[i];
			}
			return *this;
		}

		size_t data_[byte];
	};

	
	struct object_16 : public object_8 {
		object_16() {}
		object_16(size_t key) : object_8(key) {}
		object_16(object_16 const &rhs) : object_8(rhs), value_(rhs.value_) {}
		static std::string to_string() { return std::to_string(sizeof(object_16)) + "byte"; }
		size_t value_;
	};
	
	struct object_32 : public object_8 {
		object_32() {}
		object_32(size_t key) : object_8(key) {}
		object_32(object_32 const &rhs) : object_8(rhs), value_(rhs.value_) {}
		static std::string to_string() { return std::to_string(sizeof(object_32)) + "byte"; }
		size_t value_;
		dummy<2> dummy_;
	};

	struct object_64 : public object_8 {
		object_64() {}
		object_64(size_t key) : object_8(key) {}
		object_64(object_64 const &rhs) : object_8(rhs), value_(rhs.value_) {}
		static std::string to_string() { return std::to_string(sizeof(object_64)) + "byte"; }
		size_t value_;
		dummy<6> dummy_;
	};

	struct object_vector {
		object_vector(): value_(2) {}
		object_vector(size_t num): num_(num) {
			for (size_t i = 0; i < 5; ++i) {
				key_.push_back(num % 100);
				num = std::hash<size_t>()(num);
			}
		}

		object_vector(object_vector const &rhs): 
			key_(rhs.key_), value_(rhs.value_), num_(rhs.num_) {}
		object_vector(object_vector &&rhs) : 
			key_(std::move(rhs.key_)), value_(rhs.value_), num_(rhs.num_) {}

		object_vector & operator= (object_vector const &rhs) {
			key_ = rhs.key_;
			value_ = rhs.value_;
			num_ = rhs.num_;
			return *this;
		}

		object_vector & operator= (object_vector &&rhs) {
			key_ = std::move(rhs.key_);
			value_ = rhs.value_;
			num_ = rhs.num_;
			return *this;
		}

		bool operator< (object_vector const &rhs) const { return key_ < rhs.key_; }
		bool operator== (object_vector const &rhs) const { return key_ == rhs.key_; }
		operator size_t () const {
			return num_;
		}
		static std::string to_string() { return std::to_string(sizeof(object_vector)) + "byte (vector)"; }
		std::vector<size_t> key_;
		size_t value_;
		size_t num_;
		 
	};
}

#endif