#pragma once
#include <string>
#include "ScanFormatter.h"


class ScanImager : public ScanFormatter
{
public:
	void GenerateInput(std::string out_path) override;
	void GenerateTruth(std::string truth_path, std::string out_path) override;
	void ProcessPrediction(std::string original_path, std::string prediction_path, std::string out_path="") override;

protected:
	void GenerateNormalMap(std::string out_path);
	void GenerateIntensityMap(std::string out_path);
	void GenerateDepthMap(std::string out_path);
	void GenerateZMap(std::string out_path);
	void GenerateCombinedMap(std::string out_path);
	void GenerateBinaryMap(cogs::Scan scan, std::string out_path, std::string file_suffix = "_truthmask.png");
	void FindNormalizingValues();

	float min_intensity_, max_intensity_, min_normal_, max_normal_, min_z_, max_z_, min_camdist_, max_camdist_;
};