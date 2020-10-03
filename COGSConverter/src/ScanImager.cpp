#include "ScanImager.h"
#include "FormattingUtilities.h"
#include "lodepng.h"

void ScanImager::GenerateInput(std::string out_path)
{
    FindNormalizingValues();
    GenerateNormalMap(out_path);
    GenerateIntensityMap(out_path);
}

void ScanImager::GenerateTruth(std::string out_path, std::string truth_path)
{
    cogs::Scan truth;
    truth.Import(truth_path);
    FormattingUtilities::Trim(truth, true);
    GenerateBinaryMap(out_path, truth);
}

void ScanImager::ProcessPrediction(std::string original_file, std::string prediction)
{
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, prediction);
    if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    cogs::Scan scan;
    scan.Import(original_file);
    auto trim_values = FormattingUtilities::FindTrimValues(scan);
    std::vector<uint32_t> to_delete;
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            auto val = image[4 * width * y + 4 * x + 0];
            if (scan.IsPointAt(x + trim_values.min_x, y + trim_values.min_y) && val == 0)
            {
                to_delete.push_back(scan.GetPointAt(x + trim_values.min_x, y + trim_values.min_y));
            }
        }
    }
    scan.Erase(to_delete);
    scan.Export("processed_" + original_file);
}

void ScanImager::GenerateNormalMap(std::string out_path)
{
    if (!out_path.empty())
    {
        out_path = out_path + "/";
    }

    auto a = (255) / (max_normal_ - min_normal_);
    auto b = 255 - a * max_normal_;

    auto width = data_.GetWidth();
    auto height = data_.GetHeight();
    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++) {
            int nx, ny, nz;
            if (data_.IsPointAt(x, y))
            {
                auto id = data_.GetPointAt(x, y);
                nx = std::max(0, (int)(a * data_.GetNormals()[id].x + b));
                ny = std::max(0, (int)(a * data_.GetNormals()[id].y + b));
                nz = std::max(0, (int)(a * data_.GetNormals()[id].z + b));
            }
            else
            {
                nx = ny = nz = 0;
            }
            image[4 * width * y + 4 * x + 0] = nx;
            image[4 * width * y + 4 * x + 1] = ny;
            image[4 * width * y + 4 * x + 2] = nz;
            image[4 * width * y + 4 * x + 3] = 255;
        }
    }
    unsigned error = lodepng::encode(out_path + file_name_ + "_normalmap.png", image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void ScanImager::GenerateIntensityMap(std::string out_path)
{
    if (!out_path.empty())
    {
        out_path = out_path + "/";
    }

    int gray_levels = 255;
    auto width = data_.GetWidth();
    auto height = data_.GetHeight();
   
    float a = (gray_levels * 2) / (max_intensity_ - min_intensity_);
    float b = (gray_levels * 2) - a * max_intensity_;

    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++) {
            int n_i;
            if (data_.IsPointAt(x, y))
            {
                auto id = data_.GetPointAt(x, y);
                n_i = std::min(255, (int)(a * data_.GetIntensities()[id] + b));
            }
            else
            {
                n_i = 0;
            }
            image[4 * width * y + 4 * x + 0] = n_i;
            image[4 * width * y + 4 * x + 1] = n_i;
            image[4 * width * y + 4 * x + 2] = n_i;
            image[4 * width * y + 4 * x + 3] = 255;
        }
    }
    unsigned error = lodepng::encode(out_path + file_name_ + "_intensitymap.png", image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void ScanImager::GenerateBinaryMap(std::string out_path, cogs::Scan scan)
{
    if (!out_path.empty())
    {
        out_path = out_path + "/";
    }
    auto width = scan.GetWidth();
    auto height = scan.GetHeight();
    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++)
    { 
        for (unsigned x = 0; x < width; x++) {
            int value = scan.IsPointAt(x, y) * 255;
            image[4 * width * y + 4 * x + 0] = value;
            image[4 * width * y + 4 * x + 1] = value;
            image[4 * width * y + 4 * x + 2] = value;
            image[4 * width * y + 4 * x + 3] = 255;
        }
    }
    unsigned error = lodepng::encode(out_path + file_name_ + "_truth.png", image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void ScanImager::FindNormalizingValues()
{
    for (uint32_t i = 0; i < data_.GetHeight(); i++)
    {
        for (uint32_t j = 0; j < data_.GetWidth(); j++)
        {
            if (data_.IsPointAt(j, i))
            {
                auto id = data_.GetPointAt(j, i);
                auto normal = data_.GetNormals()[id];
                auto intensity = data_.GetIntensities()[id];
                auto position = data_.GetPositions()[id];

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
        }
    }
}
