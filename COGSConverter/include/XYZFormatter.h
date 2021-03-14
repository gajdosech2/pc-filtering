#pragma once
#include <string>
#include "ScanFormatter.h"


class XYZFormatter : public ScanFormatter
{
public:
    void GenerateInput(std::string out_path) override;
    void Subsampled(int point_count);
    void GenerateTruth(std::string truth_path, std::string out_path) override;
    void ProcessPrediction(std::string original_path, std::string prediction_path, std::string out_path = "") override {};
};