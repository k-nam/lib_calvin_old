#ifndef LIB_CALVIN__SORT__BENCH_H
#define LIB_CALVIN__SORT__BENCH_H

#include <map>
#include <vector>

namespace lib_calvin
{
	void sort_bench();

}

namespace lib_calvin_sort
{
	size_t const benchTestSize = 1000 * 1000;
	size_t const benchNumIter = 1;

	template <typename T>
	std::map<std::string, double> sortBenchTemplate();

	template <typename T>
	double sortBenchTemplateSub(void(*sorter)(T *first, T *last, std::less<T>));

	std::map<std::string, std::vector<double>>
		combineResults(std::vector<std::map<std::string, double>>);
}
#endif
