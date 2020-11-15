#pragma once
#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <COGS/Scan.h>


class ScanFormatter
{
public:
	ScanFormatter() = default;
	ScanFormatter(std::string path, bool trim = true) { Import(path, trim); };
	bool Import(std::string path, bool trim = true);

	virtual void GenerateInput(std::string out_path) = 0;
	virtual void GenerateTruth(std::string truth_path, std::string out_path) = 0;
	virtual void ProcessPrediction(std::string original_path, std::string prediction_path, std::string out_path="") = 0;

protected:
	void PrepareFileName(std::string path);
	cogs::Scan data_;
	std::string file_name_;
};