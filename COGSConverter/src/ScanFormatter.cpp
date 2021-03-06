#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "ScanFormatter.h"
#include "FormattingUtilities.h"

bool ScanFormatter::Import(std::string path, bool trim, bool transform)
{
    data_.Import(path);
    if (transform)
    {
        data_.ChangeCameraBasisDefinition(utils::COGS_CAMERA_SPACE);
        auto view_matrix = data_.GetCameraParams().view;
        data_.Transform(view_matrix, utils::COGS_CAMERA_SPACE);
    }
    if (trim)
    {
        FormattingUtilities::Trim(data_);
    }
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
