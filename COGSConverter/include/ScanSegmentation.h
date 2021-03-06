#pragma once
#include <string>
#include "ScanImager.h"


class ScanSegmentation : public ScanImager
{
public:
	void GenerateInput(std::string out_path) override;
	void GenerateTruth(std::string truth_path, std::string out_path="") override;
};