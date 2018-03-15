#ifndef LIB_CALVIN__UTIL__SAVE_BENCH_H
#define LIB_CALVIN__UTIL__SAVE_BENCH_H

#include <string>
#include <vector>
#include <fstream>

namespace lib_calvin_util
{
	std::string const saveFilePath = "../../../bench_result/";

	// time is # secs from Jan 1, 1970
	void save_bench(std::string title, float time, std::string testEnvironment, std::string comment,
		std::vector<std::string> tags, std::vector<std::string> algorithmNames,
		std::vector<std::vector<double>> results, 
		std::vector<std::string> testCases, std::string unit);
}

#endif