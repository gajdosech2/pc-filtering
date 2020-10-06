#pragma once
#include <stdint.h>
#include <string>

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

    uint32_t min_x, max_x, min_y, max_y;
};

class FormattingUtilities
{
public:
    static TrimValues FindTrimValues(cogs::Scan &scan);
    static void Trim(cogs::Scan &scan, TrimValues trim_values);
    static void Trim(cogs::Scan &scan, bool use_last = false);
    static void Pad(cogs::Scan &scan, int size);

    inline static TrimValues last;
};