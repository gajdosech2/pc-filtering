#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "XYZFormatter.h"
#include "FormattingUtilities.h"
#include <COGS/Scan.h>
#include <fstream>
#include <iomanip>

const bool SUBSAMPLE = true;

void XYZFormatter::GenerateInput(std::string out)
{
    if (SUBSAMPLE)
    {
        Subsampled(8192);
    }
    else
    {
        size_t height = data_.GetHeight();
        size_t width = data_.GetWidth();
        std::ofstream data_file;
        data_file.open(file_name_ + ".xyz");
        for (uint32_t y = 0; y < height; y++)
        {
            for (uint32_t x = 0; x < width; x++)
            {
                if (data_.IsPointAt(x, y))
                {
                    auto id = data_.GetPointAt(x, y);
                    auto pos = data_.GetPositions()[id];
                    data_file << pos.x << " " << pos.y << " " << pos.z << std::endl;
                }
            }
        }
        data_file.close();
    }

}

void XYZFormatter::Subsampled(int point_count)
{
    size_t height = data_.GetHeight();
    size_t width = data_.GetWidth();
    std::ofstream data_file;
    data_file.open(file_name_ + ".xyz");
    std::set<uint32_t> present;
    for (int i = 0; i < point_count; i++)
    {
        uint32_t x = rand() % width;
        uint32_t y = rand() % height;
        while (!data_.IsPointAt(x, y) || present.find(data_.GetPointAt(x, y)) != present.end())
        {
            x = rand() % width;
            y = rand() % height;
        }
        auto id = data_.GetPointAt(x, y);
        present.insert(id);
        auto pos = data_.GetPositions()[id];
        data_file << pos.x << " " << pos.y << " " << pos.z << std::endl;
    }
    data_file.close();
}

void XYZFormatter::GenerateTruth(std::string truth_path, std::string out)
{
    cogs::Scan truth;
    truth.Import(truth_path);
    FormattingUtilities::Trim(truth, true);

    std::ofstream outliers;
    outliers.open(file_name_ + ".outliers");
    size_t height = data_.GetHeight();
    size_t width = data_.GetWidth();

    int o = 0;
    int t = 0;
    for (uint32_t i = 0; i < height; i++)
    {
        for (uint32_t j = 0; j < width; j++)
        {
            if (data_.IsPointAt(j, i))
            {
                o++;
                bool present = (truth.IsPointAt(j, i));
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
