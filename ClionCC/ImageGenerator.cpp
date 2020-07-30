#include "ImageGenerator.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <pngwriter.h>

const std::string IMAGES_ROOT = "";

ImageGenerator::ImageGenerator(std::string file_name, std::vector<std::vector<PointFeatures>> *data)
{
	this->data_ = data;
	this->file_name_ = file_name;
}

bool ImageGenerator::GenerateBinaryMap()
{
	if (data_->size() == 0 || data_[0].size() == 0)
	{
		return false;
	}
	size_t height = data_->size();
	size_t width = (*data_)[0].size();
	std::ofstream image;
	image.open(IMAGES_ROOT + file_name_ + "_bitmap.pbm");
	image << "P1" << std::endl;
	image << width << " " << height << std::endl;
	for (uint32_t i = 0; i < height; i++)
	{
		for (uint32_t j = 0; j < width; j++)
		{
			image << ((*data_)[i][j].intensity > EMPTY_POINT) << " ";
		}
		image << std::endl;
	}
	image.close();
	return true;
}

bool ImageGenerator::GenerateDepthMap(float max_depth, float min_depth)
{
	if (data_->size() == 0 || data_[0].size() == 0)
	{
		return false;
	}
	int gray_levels = 255;
	size_t height = data_->size();
	size_t width = (*data_)[0].size();
	std::ofstream image;
	image.open(IMAGES_ROOT + file_name_ + "_depthmap.pgm");
	image << "P2" << std::endl;
	image << width << " " << height << std::endl;
	image << gray_levels << std::endl;
	float offset = 0;
	if (min_depth < 0) {
		offset = -min_depth;
		max_depth += offset;
		min_depth = 0;
	}
	float a = (gray_levels * 2) / (max_depth - min_depth);
	float b = (gray_levels * 2) - a * max_depth;
	for (uint32_t i = 0; i < height; i++)
	{
		for (uint32_t j = 0; j < width; j++)
		{
			int normalized_depth = (int)(a * ((*data_)[i][j].pos_z + offset) + b);
			normalized_depth = std::max(0, std::min(normalized_depth, 255));
			image << normalized_depth << " ";
		}
		image << std::endl;
	}
	image.close();
	return true;
}

bool ImageGenerator::GenerateGrayMap(float max_intensity, float min_intensity)
{
	if (data_->size() == 0 || data_[0].size() == 0)
	{
		return false;
	}
	int gray_levels = 255;
	size_t height = data_->size();
	size_t width = (*data_)[0].size();
	std::ofstream image;
	image.open(IMAGES_ROOT + file_name_ + "_graymap.pgm");
	image << "P2" << std::endl;
	image << width << " " << height << std::endl;
	image << gray_levels << std::endl;
	float a = (gray_levels * 2) / (max_intensity - min_intensity);
	float b = (gray_levels * 2) - a * max_intensity;
	for (uint32_t i = 0; i < height; i++)
	{
		for (uint32_t j = 0; j < width; j++)
		{
			int normalized_intensity = (int)(a * (*data_)[i][j].intensity + b);
			normalized_intensity = std::max(0, std::min(normalized_intensity, 255));
			image << normalized_intensity << " ";
		}
		image << std::endl;
	}
	image.close();
	return true;
}

bool ImageGenerator::GenerateNormalMap(float max_normal, float min_normal)
{
	size_t height = data_->size();
	size_t width = (*data_)[0].size();
	std::ofstream image;
	image.open(IMAGES_ROOT + file_name_ + "_normalmap.ppm");
	image << "P3" << std::endl;
	image << width << " " << height << std::endl;
	image << 255 << std::endl;
	float a = (255) / (max_normal - min_normal);
	float b = 255 - a * max_normal;
	for (uint32_t i = 0; i < height; i++)
	{
		for (uint32_t j = 0; j < width; j++)
		{
			int normalized_x = (int)(a * (*data_)[i][j].normal_x + b);
			int normalized_y = (int)(a * (*data_)[i][j].normal_y + b);
			int normalized_z = (int)(a * (*data_)[i][j].normal_z + b);
			image << normalized_x << " " << normalized_y << " " << normalized_z << " ";
		}
		image << std::endl;
	}
	image.close();
	return true;
}

bool ImageGenerator::GenerateAll(float max_depth, float min_depth, float max_intensity, float min_intensity, float max_normal, float min_normal)
{
	if (data_->size() == 0 || data_[0].size() == 0)
	{
		return false;
	}
	size_t height = data_->size();
	size_t width = (*data_)[0].size();

	std::ofstream binary_map;
	std::ofstream normal_map;
	std::ofstream intensity_map;

	binary_map.open(IMAGES_ROOT + file_name_ + "_bitmap.pbm");
	binary_map << "P1" << std::endl;
	binary_map << width << " " << height << std::endl;

	normal_map.open(IMAGES_ROOT + file_name_ + "_normalmap.ppm");
	normal_map << "P3" << std::endl;
	normal_map << width << " " << height << std::endl;
	normal_map << 255 << std::endl;

	intensity_map.open(IMAGES_ROOT + file_name_ + "_intensitymap.pgm");
	intensity_map << "P2" << std::endl;
	intensity_map << width << " " << height << std::endl;
	intensity_map << 255 << std::endl;

	float a_normal = (255) / (max_normal - min_normal);
	float b_normal = 255 - a_normal * max_normal;

	float a_gray = (255 * 2) / (max_intensity - min_intensity);
	float b_gray = (255 * 2) - a_gray * max_intensity;

	for (uint32_t i = 0; i < height; i++)
	{
		for (uint32_t j = 0; j < width; j++)
		{
			binary_map << ((*data_)[i][j].intensity <= EMPTY_POINT) << " ";

			int normalized_x = std::max(0,(int)(a_normal * (*data_)[i][j].normal_x + b_normal));
			int normalized_y = std::max(0,(int)(a_normal * (*data_)[i][j].normal_y + b_normal));
			int normalized_z = std::max(0,(int)(a_normal * (*data_)[i][j].normal_z + b_normal));
			normal_map << normalized_x << " " << normalized_y << " " << normalized_z << " ";

			int normalized_intensity = (int)(a_gray * (*data_)[i][j].intensity + b_gray);
			normalized_intensity = std::max(0, std::min(normalized_intensity, 255));
			intensity_map << normalized_intensity << " ";
		}
		binary_map << std::endl;
		normal_map << std::endl;
		intensity_map << std::endl;
	}
	binary_map.close();
	normal_map.close();
	intensity_map.close();
	return true;
}

bool ImageGenerator::GeneratePNGs(float max_depth, float min_depth, float max_intensity, float min_intensity, float max_normal, float min_normal, std::string out)
{
    if (data_->empty() || data_[0].empty())
    {
        return false;
    }
    size_t height = data_->size();
    size_t width = (*data_)[0].size();

    if (!out.empty())
    {
        out = out + "/";
    }

    //pngwriter binary_map(width,height,0, (out + file_name_ + "_bitmap.png").c_str());
    pngwriter intensity_map(width,height,0, (out +  file_name_ + "_intensitymap.png").c_str());
    pngwriter normal_map(width,height,0, (out + file_name_ + "_normalmap.png").c_str());

    double a_normal = (1.0) / (max_normal - min_normal);
    double b_normal = 1.0 - a_normal * max_normal;

    double a_gray = (1.0 * 2) / (max_intensity - min_intensity);
    double b_gray = (1.0 * 2) - a_gray * max_intensity;

    for (uint32_t i = 0; i < height; i++)
    {
        for (uint32_t j = 0; j < width; j++)
        {
            int binary = ((*data_)[i][j].intensity > EMPTY_POINT) * 65535;
            //binary_map.plot(j, height - i, binary, binary, binary);

            double normalized_x = std::max(0.0,(a_normal * (*data_)[i][j].normal_x + b_normal));
            double normalized_y = std::max(0.0,(a_normal * (*data_)[i][j].normal_y + b_normal));
            double normalized_z = std::max(0.0,(a_normal * (*data_)[i][j].normal_z + b_normal));
            normal_map.plot(j, height - i, normalized_x, normalized_y, normalized_z);

            double normalized_intensity = (a_gray * (*data_)[i][j].intensity + b_gray);
            normalized_intensity = std::max(0.0, std::min(normalized_intensity, 1.0));
            intensity_map.plot(j, height - i, normalized_intensity, normalized_intensity, normalized_intensity);
        }
    }
    //binary_map.close();
    normal_map.close();
    intensity_map.close();
    return true;
}

bool ImageGenerator::GenerateBinaryPNG(std::string out) {
    if (data_->empty() || data_[0].empty())
    {
        return false;
    }
    size_t height = data_->size();
    size_t width = (*data_)[0].size();

    if (!out.empty())
    {
        out = out + "/";
    }

    pngwriter binary_map(width,height,0, (out + file_name_ + "_bitmap.png").c_str());

    for (uint32_t i = 0; i < height; i++)
    {
        for (uint32_t j = 0; j < width; j++)
        {
            int binary = ((*data_)[i][j].intensity > EMPTY_POINT) * 65535;
            binary_map.plot(j, height - i, binary, binary, binary);
        }
    }
    binary_map.close();
    return true;
}

std::vector<std::vector<int>> ImageGenerator::ReadPNG(const char* file) {

    pngwriter img;
    img.readfromfile(file);
    std::vector<std::vector<int>> data;

    for (uint32_t i = 0; i < img.getheight(); i++)
    {
        std::vector<int> row;
        for (uint32_t j = 0; j < img.getwidth(); j++)
        {
            row.push_back(img.read(j, img.getheight() - i - 1));
        }
        data.push_back(row);
    }

    return data;
}
