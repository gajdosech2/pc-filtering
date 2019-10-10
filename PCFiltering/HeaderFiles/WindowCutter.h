#pragma once
#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include <COGS/Scan.h>
#include <string>

class WindowCutter
{
public:
  bool Import(std::string path);
  void Cut(unsigned int left_up_x, unsigned int left_up_y, unsigned int down_right_x, unsigned int down_right_y);
  bool Export();
  void Trim();

private:
  cogs::Scan scan;
  std::string dataset_path;
};