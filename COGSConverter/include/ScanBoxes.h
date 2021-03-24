#pragma once
#include <string>
#include "ScanImager.h"


class ScanBoxes : public ScanImager
{
public:
	int class_id = 0;
	void GenerateInput(std::string out_path) override;
	void GenerateTruth(std::string truth_path, std::string out_path="") override;
};