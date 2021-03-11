#pragma once
#include <string>
#include "ScanFormatter.h"


class PointCleanNetFormatter : public ScanFormatter
{
public:
    void GenerateInput(std::string out_path) override;
    void GenerateTruth(std::string truth_path, std::string out_path) override;
    void ProcessPrediction(std::string original_path, std::string prediction_path, std::string out_path = "") override {};
};