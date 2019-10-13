#pragma once
#include <string>
#include <map>

class PredictionProcessor {
public:
	static void ProcessPrediction(std::string prediction_file_path, std::string cogs_file_path);

private:
	static std::map<int, int> ReadPredictionFile(std::string prediction_file_path);
};