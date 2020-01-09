#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "PointFeatures.h"

namespace cogs
{
  class Scan;
}


struct TrimValues
{
  TrimValues()
  {
    min_x = max_x = min_y = max_y = -1;
  }

  TrimValues(uint32_t min_x, uint32_t max_x, uint32_t min_y, uint32_t max_y)
  {
    this->min_x = min_x;
    this->max_x = max_x;
    this->min_y = min_y;
    this->max_y = max_y;
  }

  uint32_t min_x;
  uint32_t max_x;
  uint32_t min_y;
  uint32_t max_y;
};

class DataFormatter
{
public:
  DataFormatter() : min_intensity_(INFINITY), max_intensity_(-INFINITY), min_normal_(INFINITY), max_normal_(-INFINITY), min_depth_(INFINITY), max_depth_(-INFINITY) {};
  DataFormatter(std::string path);
  bool Import(std::string path);
  void GenerateImageFiles();
  void GenerateDataFiles(int tile_size, int step_size = 1);
  void GenerateTruthFile(std::string truth_path, int tile_size, int step_size = 1);
  void GenerateSegmentationMask(std::string truth_path);
  TrimValues FindTrimValues();
  void Trim(TrimValues trim_values);
  void Trim();
  void Pad(int size);
  std::vector<std::vector<PointFeatures>> GetData() { return data_; };
  TrimValues GetLastTrimValues() { return last_trim_values_; }
private:
  std::vector<PointFeatures> GetNeighborhood(int i, int j, int tile_size);
  void PrepareFileName(std::string path);
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