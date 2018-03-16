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

	// time is # secs from Jan 1, 1970
	void save_bench(std::string title, std::string time, std::string comment, std::string unit,
		std::vector<std::string> tags, 
		std::map<std::string, std::vector<double>> results, 
		std::vector<std::string> testCases);
}

#endif