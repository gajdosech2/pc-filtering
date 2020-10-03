#pragma once
#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <COGS/Scan.h>


class ScanFormatter
{
public:
	ScanFormatter() = default;
	ScanFormatter(std::string path) { Import(path); };
	bool Import(std::string path);

	virtual void GenerateInput(std::string out) = 0;
	virtual void GenerateTruth(std::string out, std::string truth) = 0;
	virtual void ProcessPrediction(std::string original_file, std::string prediction) = 0;

protected:
	void PrepareFileName(std::string path);
	cogs::Scan data_;
	std::string file_name_;
};