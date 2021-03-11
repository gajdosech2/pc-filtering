#pragma once
#include <string>
#include "ScanImager.h"

class ScanBoxes : public ScanImager
{
private:
	const int GRID_S = 16;
	const int CELL_S = 128;
	const int DIM = GRID_S * CELL_S;

	// should rescale the output images into DIMxDIM
public:
	void GenerateInput(std::string out_path) override;
	void GenerateCombinedMap(std::string out_path);
	void GenerateTruth(std::string truth_path, std::string out_path="") override;
};