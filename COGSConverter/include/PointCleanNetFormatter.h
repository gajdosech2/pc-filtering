#pragma once
#include <stdint.h>
#include <string>
#include "ScanFormatter.h"


class PointCleanNetFormatter : public ScanFormatter
{
public:
    void GenerateInput(std::string out) override;
    void GenerateTruth(std::string truth, std::string out) override;
    void ProcessPrediction(std::string original_path, std::string prediction_path, std::string out_path = "") override;
};