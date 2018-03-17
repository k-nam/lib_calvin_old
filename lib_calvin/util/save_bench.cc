#include "save_bench.h"
#include "json/json.hpp"
#include <ctime>

using namespace std;

void lib_calvin_util::save_bench(string title, string time, string comment, string unit,
		vector<string> tags, map<string, vector<double>> results,
		vector<string> testCases) {

	using json = nlohmann::json;
	json benchData;
	benchData["title"] = title;
	benchData["time"] = time;
	benchData["test_environment"] = lib_calvin_util::environment;
	benchData["comment"] = comment;
	benchData["unit"] = unit;
	benchData["tags"] = tags;
	benchData["results"] = results;
	benchData["test_cases"] = testCases;

	cout << benchData.dump();

	ofstream jsonFile(saveFilePath + to_string(std::time(nullptr)) + ".json");
	jsonFile << benchData.dump();
	jsonFile.close(); 
}