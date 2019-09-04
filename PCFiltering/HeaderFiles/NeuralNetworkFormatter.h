
#include <stdint.h>
#include <vector>
#include <string>
#include "PointFeatures.h"

namespace cogs
{
  class Scan;
}

class NeuralNetworkFormatter
{
public:
  NeuralNetworkFormatter() : min_intensity_(INFINITY), max_intensity_(-INFINITY), min_normal_(INFINITY), max_normal_(-INFINITY) {};
  NeuralNetworkFormatter(std::string path);
  bool Import(std::string path);
  bool GenerateBinaryMap();
  bool GenerateGrayMap();
  bool GenerateNormalMap();
  void GenerateDataFiles();
  void GeneratePredictionFile();
  void Trim();
  void Pad(int size);
private:
  void PrepareFileName(std::string path);
  std::vector<std::vector<PointFeatures>> data_;
  std::string file_name;
  float min_intensity_;
  float max_intensity_;
  float min_normal_;
  float max_normal_;
};