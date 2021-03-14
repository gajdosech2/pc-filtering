#include "ScanSegmentation.h"
#include "FormattingUtilities.h"
#include "lodepng.h"

void ScanSegmentation::GenerateInput(std::string out_path)
{
    data_.TransformToSpace(utils::COGS_CAMERA_SPACE);
    FindNormalizingValues();
    GenerateNormalMap(out_path);
}

void ScanSegmentation::GenerateTruth(std::string labels_path, std::string out_path)
{
    std::vector<unsigned char> label_image;
    unsigned label_width, label_height;
    unsigned error = lodepng::decode(label_image, label_width, label_height, labels_path);

    auto trim_values = FormattingUtilities::last;
    if (trim_values.min_x == -1)
    {
        trim_values.min_x = 0;
        trim_values.min_y = 0;
        trim_values.max_x = label_width - 1;
        trim_values.max_y = label_height - 1;
    }

    unsigned new_width = trim_values.max_x - trim_values.min_x + 1;
    unsigned new_height = trim_values.max_y - trim_values.min_y + 1;
    std::vector<unsigned char> image;
    image.resize(new_width * new_height * 4);
    for (unsigned y = 0; y < new_height; y++)
    {
        for (unsigned x = 0; x < new_width; x++) 
        {
            auto label_x = x + trim_values.min_x;
            auto label_y = y + trim_values.min_y;
            auto label_value = label_image[4 * label_width * label_y + 4 * label_x + 0];

            unsigned value = (label_value >= 2) ? 255 : 0;
            image[4 * new_width * y + 4 * x + 0] = value;
            image[4 * new_width * y + 4 * x + 1] = value;
            image[4 * new_width * y + 4 * x + 2] = value;
            image[4 * new_width * y + 4 * x + 3] = 255;
        }
    }

    error = lodepng::encode(out_path + file_name_ + "_segmask.png", image, new_width, new_height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
