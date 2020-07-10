#pragma once
#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "..\HeaderFiles\DataFormatter.h"
#include "..\HeaderFiles\ImageGenerator.h"
#include <COGS/Scan.h>

DataFormatter::DataFormatter(std::string path) : min_intensity_(INFINITY), max_intensity_(-INFINITY), min_normal_(INFINITY), max_normal_(-INFINITY), min_depth_(INFINITY), max_depth_(-INFINITY)
{
  Import(path);
}

bool DataFormatter::Import(std::string path)
{
  cogs::Scan scan;
  scan.Import(path);
  uint32_t width = scan.GetWidth();
  uint32_t height = scan.GetHeight();
  auto positions = scan.GetPositions();
  auto normals = scan.GetNormals();
  auto intensities = scan.GetIntensities();
  data_.resize(height);
  for (uint32_t i = 0; i < height; i++)
  {
    std::vector<PointFeatures> row;
    row.resize(width);
    for (uint32_t j = 0; j < width; j++)
    {
      if (scan.IsPointAt(j, i))
      {
        auto index = scan.GetPointAt(j, i);
        auto normal = normals[index];
        auto intensity = intensities[index];
        auto position = positions[index];
        PointFeatures point_feature(index, position.x, position.y, position.z, normal.x, normal.y, normal.z, intensity);
        row[j] = point_feature;

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
        row[j] = PointFeatures();
      }
    }
    data_[i] = row;
  }
  PrepareFileName(path);
  return true;
}

void DataFormatter::PrepareFileName(std::string path)
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

TrimValues DataFormatter::FindTrimValues()
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

void DataFormatter::Trim(TrimValues trim_values)
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

void DataFormatter::Trim()
{
  Trim(FindTrimValues());
}

void DataFormatter::Pad(int size)
{
  int height = (int)data_.size();
  int width = (int)data_[0].size();
  int dif_x = size - width;
  int dif_y = size - height;
  if (dif_x < 0 || dif_y < 0)
  {
    return;
  }
  int left_pad = dif_x / 2;
  int right_pad = dif_x / 2;
  if (dif_x % 2 != 0)
  {
    right_pad = dif_x / 2 + 1;
  }
  int top_pad = dif_y / 2;
  int bottom_pad = dif_y / 2;
  if (dif_y % 2 != 0)
  {
    bottom_pad = dif_y / 2 + 1;
  }
  std::vector<std::vector<PointFeatures>> new_data;
  for (int i = -top_pad; i < height + bottom_pad; i++)
  {
    std::vector<PointFeatures> row;
    for (int j = -left_pad; j < width + right_pad; j++)
    {
      if (i < 0 || j < 0 || i >= height || j >= width)
      {
        row.push_back(PointFeatures());
      }
      else
      {
        row.push_back(data_[i][j]);
      }
    }
    new_data.push_back(row);
  }
  data_ = new_data;
}

void DataFormatter::GenerateSegmentationMask(std::string truth_path)
{
  DataFormatter truth(truth_path);
  auto truth_data = truth.GetData();
  if (truth_data.size() != data_.size() || truth_data[0].size() != data_[0].size())
  {
    truth.Trim(last_trim_values_);
    //truth.Pad((int)data_.size());
    truth_data = truth.GetData();
  }
  ImageGenerator image_generator(file_name_ + "_truth", &truth_data);
  image_generator.GenerateBinaryMap();
}

void DataFormatter::GenerateImageFiles()
{
  ImageGenerator image_generator(file_name_, &data_);
  image_generator.GenerateAll(max_depth_, min_depth_, max_intensity_, min_intensity_, max_normal_, min_normal_);
}
