#include "..\HeaderFiles\WindowCutter.h"
#include <sstream>

bool WindowCutter::Import(std::string path)
{
  dataset_path = "DataSets/" + path;
  scan.Import(dataset_path);
}

void WindowCutter::Cut(unsigned int left_up_x, unsigned int left_up_y, unsigned int down_right_x, unsigned int down_right_y)
{
  uint32_t width = scan.GetWidth();
  uint32_t height = scan.GetHeight();

  if (down_right_x > width || down_right_y > height)
  {
    std::cout << "Coordinates are bigger than data!" << std::endl;
    return;
  }
  if (left_up_x > down_right_x || left_up_y > down_right_y)
  {
    std::cout << "Upper left corner must have smaller coordinates than bottom right!" << std::endl;
    return;
  }

  std::vector<unsigned int> ids_to_delete;
  for (uint32_t y = 0; y < height; y++)
  {
    for (uint32_t x = 0; x < width; x++)
    {
      if ((x < left_up_x || x > down_right_x || y < left_up_y || y > down_right_y) && scan.IsPointAt(x, y))
      {
        ids_to_delete.push_back(scan.GetPointAt(x, y));
      }
    }
  }
  scan.Erase(ids_to_delete);
  scan.ShrinkToFit();
}


bool WindowCutter::Export()
{
  std::stringstream s_export;
  s_export << dataset_path << "EXPORT" << ".cogs";
  return scan.Export(s_export.str());
}

void WindowCutter::Trim()
{
  uint32_t width = scan.GetWidth();
  uint32_t height = scan.GetHeight();
  uint32_t min_x = width;
  uint32_t min_y = height;
  uint32_t max_x = 0;
  uint32_t max_y = 0;
  for (uint32_t y = 0; y < height; y++)
  {
    for (uint32_t x = 0; x < width; x++)
    {
      if (scan.IsPointAt(x, y))
      {
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
      }
    }
  }

  std::cout << "min_x:" << min_x << std::endl;
  std::cout << "max_x:" << max_x << std::endl;
  std::cout << "min_y:" << min_y << std::endl;
  std::cout << "max_y:" << max_y << std::endl;
}