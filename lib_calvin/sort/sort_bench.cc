#include <utility>
#include <vector>
#include <ctime>
#include <string>

#include "sort_bench.h"
#include "sort.h"
#include "sort_test.h"
#include "pdqsort.h"

void
lib_calvin::sort_bench() {
	using namespace std;
	using namespace lib_calvin_sort;
	string title = "Sorting";
	string comment = "block_qsort is my implementation of BlockQuickSort. pdqsort is the official one.";
	vector<string> tags = { "sorting" };
	float time = static_cast<float>(std::time(nullptr));
	//cout << to_string(time);
	string unit = "sec (lower is better)";	

	vector<map<string, double>> results;
	results.push_back(sortBenchTemplate<int>());
	results.push_back(sortBenchTemplate<SimpleStruct>());
	results.push_back(sortBenchTemplate<StringStruct>());
	
	vector<string> testCases = { "int", "16Byte (key:int)", "24Byte (key:string)" };
}

template <typename T>
std::map<std::string, double>
lib_calvin_sort::sortBenchTemplate() {
	using namespace std;
	typedef void(*sorter)(T *first, T *last, less<T>);
	vector<pair<string, sorter>> input;
	input.push_back(make_pair("qsort", introSort<T *, less<T>>));
	input.push_back(make_pair("pdqsort", pdqsort<T *, less<T>>));

	vector<string> algorithmNames;
	vector<sorter> algorithms;

	std::map<std::string, double> result;
	result["1"] = 1.5;
	return result;
}

template <typename T>
double
lib_calvin_sort::sortBenchTemplateSub(void(*sorter[])(T *first, T *last, std::less<T>)) {
	using namespace std;

}

std::map<std::string, double>
combineResults(std::vector<std::map<std::string, double>>) {

}