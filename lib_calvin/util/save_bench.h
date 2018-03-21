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

	void save_bench(std::string title, std::string comment, std::string unit,
		std::vector<std::string> tags, 
		std::map<std::string, std::vector<double>> results, 
		std::vector<std::string> testCases);
}

#endif