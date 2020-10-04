#pragma once
#include <string>
#include "ScanFormatter.h"

class ScanImager : public ScanFormatter
{
public:
	void GenerateInput(std::string out) override;
	void GenerateTruth(std::string out, std::string truth) override;
	void ProcessPrediction(std::string original_file, std::string prediction) override;

private:
	void GenerateNormalMap(std::string out);
	void GenerateIntensityMap(std::string out);
	void GenerateBinaryMap(std::string out, cogs::Scan scan);
	void FindNormalizingValues();
	float min_intensity_, max_intensity_, min_normal_, max_normal_, min_depth_, max_depth_;
};