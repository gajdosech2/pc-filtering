#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "ScanFormatter.h"
#include "FormattingUtilities.h"

bool ScanFormatter::Import(std::string path)
{
    std::cout << "going to import" << std::endl;
    data_.Import(path);
    std::cout << "going to trim" << std::endl;
    FormattingUtilities::Trim(data_);
    std::cout << "going to prepare name" << std::endl;
    PrepareFileName(path);
    return true;
}

void ScanFormatter::PrepareFileName(std::string path)
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
