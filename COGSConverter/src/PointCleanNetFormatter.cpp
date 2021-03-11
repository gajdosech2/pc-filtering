#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "PointCleanNetFormatter.h"
#include "FormattingUtilities.h"
#include <COGS/Scan.h>
#include <fstream>
#include <iomanip>


void PointCleanNetFormatter::GenerateInput(std::string out)
{
    size_t height = data_.GetHeight();
    size_t width = data_.GetWidth();
    std::ofstream data_file;
    data_file.open(file_name_ + ".xyz");
    for (uint32_t i = 0; i < height; i++)
    {
        for (uint32_t j = 0; j < width; j++)
        {
            if (data_.IsPointAt(j, i))
            {
                auto id = data_.GetPointAt(glm::uvec2(j, i));
                auto pos = data_.GetPositions()[id];
                data_file << pos.x << " " << pos.y << " " << pos.z << std::endl;
            }
        }
    }
    data_file.close();
}

void PointCleanNetFormatter::GenerateTruth(std::string truth_path, std::string out)
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
