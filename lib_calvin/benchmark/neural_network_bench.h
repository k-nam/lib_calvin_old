#ifndef LIB_CALVIN__NEURAL_NETWORK_BENCH_H
#define LIB_CALVIN__NEURAL_NETWORK_BENCH_H

#include <vector>
#include <string>

#include "neural_network.h"

namespace lib_calvin_benchmark
{
namespace neural_network
{

size_t const benchNumCases = 7;
std::vector<std::string> const benchTestCase = { "accuracy" };

std::string const unit = "% (higher is better)";
std::string const category = "Etc.";

static size_t currentBenchNum = 0;


}
}
#endif
