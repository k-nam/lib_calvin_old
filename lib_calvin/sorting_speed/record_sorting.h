#pragma once

#include <vector>

struct record {
	size_t key_;
	double data1_;
	size_t data2_;

	bool operator< (record const &rhs) const {
		return key_ < rhs.key_;
	}

	bool operator== (record const &rhs) const {
		return key_ == rhs.key_;
	}
};

void record_sort_test();

void record_sort(std::vector<record> & input);

void record_sort_inplace(std::vector<record> & input);