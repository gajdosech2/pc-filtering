#pragma once
#include <stdint.h>
#include <string>
#include "ScanFormatter.h"


class PointCleanNetFormatter : public ScanFormatter
{
public:
    void GenerateInput(std::string out) override;
    void GenerateTruth(std::string out, std::string truth) override;
};