#include "bench.h"
#include "json/json.hpp"
#include "random.h"
#include <ctime>

using namespace std;

std::string lib_calvin_benchmark::getDatetime() {
	time_t rawtime;
	time(&rawtime);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", &timeinfo);
	return buffer;
}

void lib_calvin_benchmark::save_bench(std::string category, std::string subCategory,
									  string title, string comment,
									  vector<vector<string>> algorithms,
									  vector<vector<double>> results, vector<string> testCases, string unit, 
									  int order) {
	using json = nlohmann::json;
	json benchData;
	benchData["category"] = category;
	benchData["sub_category"] = subCategory;
	benchData["title"] = title;
	benchData["datetime"] = getDatetime();
	benchData["env"] = environment;
	benchData["comment"] = comment;

	json data;
	data["algorithms"] = algorithms;
	data["results"] = results;
	data["test_cases"] = testCases;
	data["unit"] = unit;
	benchData["data"] = data;

	benchData["order"] = order;

	//cout << benchData.dump();
	lib_calvin::random_number_generator gen;

	ofstream jsonFile(saveFilePath + to_string(std::time(nullptr)) + "_" +
					  std::to_string(gen()) + ".json");
	jsonFile << benchData.dump();
	jsonFile.close();
}

