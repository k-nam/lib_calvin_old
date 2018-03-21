#include "save_bench.h"
#include "json/json.hpp"
#include <ctime>

using namespace std;

std::string lib_calvin_util::getDatetime() {
	time_t rawtime;
	time(&rawtime);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", &timeinfo);
	return buffer;
}

void lib_calvin_util::save_bench(string title, string comment, string unit,
		vector<string> tags, map<string, vector<double>> results,
		vector<string> testCases) {
	using json = nlohmann::json;
	json benchData;
	benchData["title"] = title;
	benchData["datetime"] = getDatetime();
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