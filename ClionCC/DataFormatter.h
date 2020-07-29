#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "PointFeatures.h"
#include "TrimValues.h"

namespace cogs
{
  class Scan;
}

class DataFormatter
{
public:
  DataFormatter() : min_intensity_(INFINITY), max_intensity_(-INFINITY), min_normal_(INFINITY), max_normal_(-INFINITY), min_depth_(INFINITY), max_depth_(-INFINITY) {};
  DataFormatter(std::string path);
  bool Import(std::string path);
  void PrepareFileName(std::string path);
  void GenerateImageFiles(std::string out = "");
  void GenerateSegmentationMask(std::string truth_path, std::string out = "");
  TrimValues FindTrimValues();
  void Trim(TrimValues trim_values);
  void Trim();
  void Pad(int size);
  std::vector<std::vector<PointFeatures>> GetData() { return data_; };
  TrimValues GetLastTrimValues() { return last_trim_values_; }
private:
  std::vector<std::vector<PointFeatures>> data_;
  TrimValues last_trim_values_;
  std::string file_name_;

  float min_intensity_;
  float max_intensity_;
  float min_normal_;
  float max_normal_;
  float min_depth_;
  float max_depth_;
};