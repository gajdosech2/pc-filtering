#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "FormattingUtilities.h"
#include <COGS/Scan.h>

TrimValues FormattingUtilities::FindTrimValues(cogs::Scan &scan)
{
    uint32_t min_x = scan.GetWidth();
    uint32_t min_y = scan.GetHeight();
    uint32_t max_x = 0;
    uint32_t max_y = 0;

    for (uint32_t i = 0; i < scan.GetHeight(); i++)
    {
        for (uint32_t j = 0; j < scan.GetWidth(); j++)
        {
            if (scan.IsPointAt(j, i))
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

void FormattingUtilities::Trim(cogs::Scan &scan, TrimValues trim_values)
{
    last = trim_values;
    cogs::Scan new_scan(scan);
    const glm::uvec2 new_resolution(trim_values.max_x - trim_values.min_x + 1, trim_values.max_y - trim_values.min_y + 1);
    new_scan.SetResolution(new_resolution);
    new_scan.AddIntensities();
    new_scan.AddNormals();

    for (uint32_t i = 0; i < new_resolution.y; i++)
    {
        for (uint32_t j = 0; j < new_resolution.x; j++)
        {
            if (scan.IsPointAt(j + trim_values.min_x, i + trim_values.min_y))
            {
                auto old_id = scan.GetPointAt(j + trim_values.min_x, i + trim_values.min_y);
                auto new_id = new_scan.AddPointAt(j, i);

                new_scan.GetPositions()[new_id] = scan.GetPositions()[old_id];
                new_scan.GetNormals()[new_id] = scan.GetNormals()[old_id];
                new_scan.GetIntensities()[new_id] = scan.GetIntensities()[old_id];
            }
        }
    }
    scan = new_scan;
}

void FormattingUtilities::Trim(cogs::Scan &scan, bool use_last)
{
    if (use_last)
    {
        Trim(scan, last);
    }
    else
    {
        Trim(scan, FindTrimValues(scan));
    }
}

void FormattingUtilities::Pad(cogs::Scan &scan, int size)
{
}


