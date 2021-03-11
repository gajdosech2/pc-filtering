#include "ScanImager.h"
#include "FormattingUtilities.h"
#include "lodepng.h"
#include <limits>
#include <glm/gtc/matrix_transform.hpp>

void ScanImager::GenerateInput(std::string out_path)
{
    FindNormalizingValues();
    if (data_.HasNormals())
    {
        GenerateNormalMap(out_path);
    }
    GenerateIntensityMap(out_path);
    GenerateDepthMap(out_path);
    GenerateBinaryMap(data_, out_path, "_binarymap.png");
}

void ScanImager::GenerateTruth(std::string truth_path, std::string out_path)
{
    cogs::Scan truth;
    truth.Import(truth_path);
    FormattingUtilities::Trim(truth, true);
    GenerateBinaryMap(truth, out_path);
}

void ScanImager::ProcessPrediction(std::string original_path, std::string prediction_path, std::string out_path)
{
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, prediction_path);
    if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    cogs::Scan scan;
    scan.Import(original_path);
    auto trim_values = FormattingUtilities::FindTrimValues(scan);
    std::vector<uint32_t> to_delete;
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++)
        {
            auto val = image[4 * width * y + 4 * x + 0];
            if (scan.IsPointAt(x + trim_values.min_x, y + trim_values.min_y) && val == 0)
            {
                to_delete.push_back(scan.GetPointAt(x + trim_values.min_x, y + trim_values.min_y));
            }
        }
    }
    scan.Erase(to_delete);
    PrepareFileName(original_path);
    scan.Export(out_path + file_name_ + "_processed.cogs");
}

void ScanImager::GenerateNormalMap(std::string out_path)
{
    if (!out_path.empty()) out_path = out_path + "/";

    const auto width = data_.GetWidth();
    const auto height = data_.GetHeight();

    const float a = (254) / (max_normal_ - min_normal_);
    const float b = (254) - a * max_normal_;

    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++)
        {
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
            image[4 * width * y + 4 * x + 0] = nx + 1;
            image[4 * width * y + 4 * x + 1] = ny + 1;
            image[4 * width * y + 4 * x + 2] = nz + 1;
            image[4 * width * y + 4 * x + 3] = 255;
        }
    }
    unsigned error = lodepng::encode(out_path + file_name_ + "_normalmap.png", image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void ScanImager::GenerateIntensityMap(std::string out_path)
{
    if (!out_path.empty()) out_path = out_path + "/";

    const auto width = data_.GetWidth();
    const auto height = data_.GetHeight();
   
    const float a = (254 * 2) / (max_intensity_ - min_intensity_);
    const float b = (254 * 2) - a * max_intensity_;

    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++) 
        {
            int n_i = 0;
            if (data_.IsPointAt(x, y))
            {
                auto id = data_.GetPointAt(x, y);
                n_i = std::min(254, (int)(a * data_.GetIntensities()[id] + b));
            }
            image[4 * width * y + 4 * x + 0] = n_i + 1;
            image[4 * width * y + 4 * x + 1] = n_i + 1;
            image[4 * width * y + 4 * x + 2] = n_i + 1;
            image[4 * width * y + 4 * x + 3] = 255;
        }
    }
    unsigned error = lodepng::encode(out_path + file_name_ + "_intensitymap.png", image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void ScanImager::GenerateDepthMap(std::string out_path)
{
    if (!out_path.empty()) out_path = out_path + "/";

    const auto width = data_.GetWidth();
    const auto height = data_.GetHeight();

    const float a = (254) / (max_camdist_ - min_camdist_);
    const float b = (254) - a * max_camdist_;

    const auto cam_pos = data_.GetCameraPosition();

    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++) {

            int n_d = 0;
            if (data_.IsPointAt(x, y))
            {
                auto id = data_.GetPointAt(x, y);
                n_d = 254 - std::min(254, (int)(a * glm::distance(cam_pos, data_.GetPositions()[id]) + b));
            }
            image[4 * width * y + 4 * x + 0] = n_d + 1;
            image[4 * width * y + 4 * x + 1] = n_d + 1;
            image[4 * width * y + 4 * x + 2] = n_d + 1;
            image[4 * width * y + 4 * x + 3] = 255;
        }
    }
    unsigned error = lodepng::encode(out_path + file_name_ + "_depthmap.png", image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void ScanImager::GenerateBinaryMap(cogs::Scan scan, std::string out_path, std::string file_suffix)
{
    if (!out_path.empty()) out_path = out_path + "/";
    
    const auto width = scan.GetWidth();
    const auto height = scan.GetHeight();

    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++)
    { 
        for (unsigned x = 0; x < width; x++) 
        {
            int value = scan.IsPointAt(x, y) * 255;
            image[4 * width * y + 4 * x + 0] = value;
            image[4 * width * y + 4 * x + 1] = value;
            image[4 * width * y + 4 * x + 2] = value;
            image[4 * width * y + 4 * x + 3] = 255;
        }
    }
    unsigned error = lodepng::encode(out_path + file_name_ + file_suffix, image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void ScanImager::GenerateCombinedMap(std::string out_path)
{
    if (!out_path.empty()) out_path = out_path + "/";

    const float  a_n = (254) / (max_normal_ - min_normal_);
    const float b_n = 254 - a_n * max_normal_;

    const float a_d = (254) / (max_camdist_ - min_camdist_);
    const float b_d = 254 - a_d * max_camdist_;

    const auto width = data_.GetWidth();
    const auto height = data_.GetHeight();
    const auto cam_pos = data_.GetCameraPosition();

    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++) 
        {
            int nx, ny, nz, d;
            if (data_.IsPointAt(x, y))
            {
                auto id = data_.GetPointAt(x, y);
                nx = std::max(0, (int)(a_n * data_.GetNormals()[id].x + b_n));
                ny = std::max(0, (int)(a_n * data_.GetNormals()[id].y + b_n));
                nz = std::max(0, (int)(a_n * data_.GetNormals()[id].z + b_n));
                d = 254 - std::min(254, (int)(a_d * glm::distance(cam_pos, data_.GetPositions()[id]) + b_d));
            }
            else
            {
                nx = ny = nz = d = 0;
            }
            image[4 * width * y + 4 * x + 0] = nx + 1;
            image[4 * width * y + 4 * x + 1] = ny + 1;
            image[4 * width * y + 4 * x + 2] = nz + 1;
            image[4 * width * y + 4 * x + 3] = d + 1;
        }
    }
    unsigned error = lodepng::encode(out_path + file_name_ + "_datamap.png", image, width, height);
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void ScanImager::FindNormalizingValues()
{
    min_intensity_ = std::numeric_limits<float>::max();
    max_intensity_ = std::numeric_limits<float>::min();
    min_normal_ = std::numeric_limits<float>::max();
    max_normal_ = std::numeric_limits<float>::min();
    min_camdist_ = std::numeric_limits<float>::max();
    max_camdist_ = std::numeric_limits<float>::min();
    
    const auto cam_pos = data_.GetCameraPosition();
    for (unsigned y = 0; y < data_.GetHeight(); y++)
    {
        for (unsigned x = 0; x < data_.GetWidth(); x++)
        {
            if (data_.IsPointAt(x, y))
            {
                auto id = data_.GetPointAt(x, y);               
                auto intensity = data_.GetIntensities()[id];
                auto position = data_.GetPositions()[id];

                min_intensity_ = std::min(min_intensity_, intensity);
                max_intensity_ = std::max(max_intensity_, intensity);
                min_depth_ = std::min(min_depth_, position.z);
                max_depth_ = std::max(max_depth_, position.z);

                if (data_.HasNormals())
                {
                    auto normal = data_.GetNormals()[id];
                    min_normal_ = std::min(min_normal_, normal.x);
                    min_normal_ = std::min(min_normal_, normal.y);
                    min_normal_ = std::min(min_normal_, normal.z);
                    max_normal_ = std::max(max_normal_, normal.x);
                    max_normal_ = std::max(max_normal_, normal.y);
                    max_normal_ = std::max(max_normal_, normal.z);
                }

                auto distance = glm::distance(position, cam_pos);
                min_camdist_ = std::min(min_camdist_, distance);
                max_camdist_ = std::max(max_camdist_, distance);
            }
        }
    }
}
