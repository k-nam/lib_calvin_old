#ifndef LIB_CALVIN__UTIL__SAVE_BENCH_H
#define LIB_CALVIN__UTIL__SAVE_BENCH_H

#include <string>
#include <vector>
#include <map>
#include <fstream>

namespace lib_calvin_util
{
	std::string const saveFilePath = "../../../bench_result/";
	std::string const environment = "Intel 4790K with 16GB ram, Windows 10, Visual C++ v141";

	std::string getDatetime();

	// algorithms: [["qsort", "unstable", "std"], ["parallel_block_qsort", "unstable", "lib_calvin"], ...]
	void save_bench(std::string category, std::string subCategory, std::string title, std::string comment,
					std::vector<std::vector<std::string>> algorithms,
					std::vector<std::vector<double>> results,
					std::vector<std::string> testCases, std::string unit);
}

#endif