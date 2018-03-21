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
	benchData["date"] = time;
	benchData["env"] = lib_calvin_util::environment;
	benchData["tags"] = tags;
	benchData["comment"] = comment;

	json data;
	data["results"] = results;
	data["test_cases"] = testCases;
	data["unit"] = unit;
	benchData["data"] = data;

	cout << benchData.dump();

	ofstream jsonFile(saveFilePath + to_string(std::time(nullptr)) + ".json");
	jsonFile << benchData.dump();
	jsonFile.close(); 
}