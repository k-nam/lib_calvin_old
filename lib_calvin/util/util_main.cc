#include <iostream>
#include <memory>
#include "save_bench.h"

int main(int argc, char* argv[])
{
	using namespace std;

	string title = "stable sorting";
	string time = "12312321";
	string comment = "Not bad~";
	vector<string> tags = { };
	vector<double> result1 = { 1, 2 };
	vector<double> result2 = { 1.5, 2.5 };
	map<string, vector<double>> results;
	results["lib_calvin"] = result1;
	results["std::stable_sort"] = result2;
	vector<string> testCases = { "small arrya", "big array" };
	auto unit = "N/sec";


	lib_calvin_util::save_bench(title, comment, unit,
		tags, results, testCases);

	return 0;
}

