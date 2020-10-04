#pragma once
#include <string>
#include "ScanFormatter.h"

class ScanImager : public ScanFormatter
{
public:
	void GenerateInput(std::string out_path) override;
	void GenerateTruth(std::string out_path, std::string truth_path) override;
	void ProcessPrediction(std::string original_file, std::string prediction_path) override;

private:
	void GenerateNormalMap(std::string out_path);
	void GenerateIntensityMap(std::string out_path);
	void GenerateBinaryMap(std::string out_path, cogs::Scan scan);
	void FindNormalizingValues();
	float min_intensity_, max_intensity_, min_normal_, max_normal_, min_depth_, max_depth_;
};