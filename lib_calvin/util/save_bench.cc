#include "save_bench.h"
#include "json/json.hpp"
#include <ctime>

void lib_calvin_util::save_bench(std::string title, float time, std::string testEnvironment, std::string comment,
		std::vector<std::string> tags, std::vector<std::string> algorithmNames,
		std::vector<std::vector<double>> results,
		std::vector<std::string> testCases, std::string unit) {

	using json = nlohmann::json;
	json benchData;
	benchData["title"] = title;
	benchData["time"] = time;
	benchData["test_environment"] = testEnvironment;
	benchData["comment"] = comment;
	benchData["tags"] = tags;
	benchData["algorithmNames"] = algorithmNames;
	benchData["results"] = results;
	benchData["test_cases"] = testCases;
	benchData["unit"] = unit;

	std::cout << benchData.dump();

	std::ofstream jsonFile(saveFilePath + std::to_string(std::time(nullptr)) + ".json");
	jsonFile << benchData.dump() << ";";
	jsonFile.close();
}