#include <iostream>
#include <memory>

#include "save_bench.h"

int main(int argc, char* argv[])
{
	using namespace std;

	string title = "stable sorting";
	float time = 12312321;
	string testEnvironment = "Intel 4790K";
	string comment = "Not bad~";
	vector<string> tags = { };
	vector<string> algorithmNames = { "lib_calvin", "std::stable_sort", "boost" };
	vector<vector<double>> results = { { 1.5, 10.0 },{ 2, 11 },{ 1.7, 10.5 } };
	vector<string> testCases = { "small arrya", "big array" };
	auto unit = "N/sec";


	lib_calvin_util::save_bench(title, time, testEnvironment, comment,
		tags, algorithmNames, results,
		testCases, unit);

	return 0;
}

