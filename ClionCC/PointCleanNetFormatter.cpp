#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "PointCleanNetFormatter.h"
#include <COGS/Scan.h>
#include <fstream>
#include <iomanip>

PointCleanNetFormatter::PointCleanNetFormatter(std::string path) : min_intensity_(INFINITY), max_intensity_(-INFINITY), min_normal_(INFINITY), max_normal_(-INFINITY), min_depth_(INFINITY), max_depth_(-INFINITY)
{
  Import(path);
}

bool PointCleanNetFormatter::Import(std::string path)
{
  cogs::Scan scan;
  scan.Import(path);
  uint32_t width = scan.GetWidth();
  uint32_t height = scan.GetHeight();
  auto positions = scan.GetPositions();
  auto normals = scan.GetNormals();
  auto intensities = scan.GetIntensities();
  for (uint32_t i = 0; i < height; i++)
  {
    std::vector<PointFeatures> row;
    for (uint32_t j = 0; j < width; j++)
    {
      if (scan.IsPointAt(j, i))
      {
        auto index = scan.GetPointAt(j, i);
        auto normal = normals[index];
        auto intensity = intensities[index];
        auto position = positions[index];
        PointFeatures point_feature(index, position.x, position.y, position.z, normal.x, normal.y, normal.z, intensity, j, i);
        row.push_back(point_feature);

        min_intensity_ = std::min(min_intensity_, intensity);
        max_intensity_ = std::max(max_intensity_, intensity);
        min_depth_ = std::min(min_depth_, position.z);
        max_depth_ = std::max(max_depth_, position.z);
        min_normal_ = std::min(min_normal_, normal.x);
        min_normal_ = std::min(min_normal_, normal.y);
        min_normal_ = std::min(min_normal_, normal.z);
        max_normal_ = std::max(max_normal_, normal.x);
        max_normal_ = std::max(max_normal_, normal.y);
        max_normal_ = std::max(max_normal_, normal.z);
      }
      else
      {
        row.push_back(PointFeatures(j, i));
      }
    }
    data_.push_back(row);
  }
  PrepareFileName(path);
  return true;
}

void PointCleanNetFormatter::PrepareFileName(std::string path)
{
  file_name_ = "";
  bool flag = false;
  for (int i = (int)path.size(); i >= 0; i--)
  {
    if (path[i] == '/' || path[i] == '\\')
    {
      break;
    }
    if (flag)
    {
      file_name_ = path[i] + file_name_;
    }
    if (path[i] == '.')
    {
      flag = true;
    }
  }
}

TrimValues PointCleanNetFormatter::FindTrimValues()
{
  size_t height = data_.size();
  size_t width = data_[0].size();
  uint32_t min_x = (uint32_t)width;
  uint32_t min_y = (uint32_t)height;
  uint32_t max_x = 0;
  uint32_t max_y = 0;
  for (uint32_t i = 0; i < height; i++)
  {
    for (uint32_t j = 0; j < width; j++)
    {
      if (data_[i][j].intensity > EMPTY_POINT)
      {
        min_y = std::min(min_y, i);
        max_y = std::max(max_y, i);
        min_x = std::min(min_x, j);
        max_x = std::max(max_x, j);
      }
    }
  }
  return TrimValues(min_x, max_x, min_y, max_y);
}

void PointCleanNetFormatter::Trim(TrimValues trim_values)
{
  last_trim_values_ = trim_values;
  std::vector<std::vector<PointFeatures>> new_data;
  for (uint32_t i = trim_values.min_y; i <= trim_values.max_y; i++)
  {
    std::vector<PointFeatures> row;
    for (uint32_t j = trim_values.min_x; j <= trim_values.max_x; j++)
    {
      row.push_back(data_[i][j]);
    }
    new_data.push_back(row);
  }

  data_ = new_data;
}

void PointCleanNetFormatter::Trim()
{
  Trim(FindTrimValues());
}

void PointCleanNetFormatter::GenerateInputFile()
{
    size_t height = data_.size();
    size_t width = data_[0].size();
    std::ofstream data_file;
    data_file.open(file_name_ + ".xyz");

    for (uint32_t i = 0; i < height; i++)
    {
        for (uint32_t j = 0; j < width; j++)
        {
            if (data_[i][j].intensity > EMPTY_POINT)
            {
                data_file << data_[i][j].pos_x << " "
                << data_[i][j].pos_y << " "
                << data_[i][j].pos_z << std::endl;
            }
        }
    }

    data_file.close();
}

void PointCleanNetFormatter::GenerateOutliersFile(std::string truth_path)
{
    PointCleanNetFormatter truth(truth_path);
    auto truth_data = truth.GetData();
    if (truth_data.size() != data_.size() || truth_data[0].size() != data_[0].size())
    {
        truth.Trim(last_trim_values_);
        truth_data = truth.GetData();
    }
    std::ofstream outliers;
    outliers.open(file_name_ + ".outliers");
    size_t height = data_.size();
    size_t width = data_[0].size();

    int o = 0;
    int t = 0;
    for (uint32_t i = 0; i < height; i++)
    {
        for (uint32_t j = 0; j < width; j++)
        {
            if (data_[i][j].intensity > EMPTY_POINT)
            {
                o++;
                bool present = (truth_data[i][j].intensity > EMPTY_POINT);
                if (present)
                {
                    t++;
                }
                outliers << !present << ".000000000000000000e+00" << std::endl;
            }
        }
    }
    std::cout << t << "/" << o << std::endl;
    outliers.close();
}
