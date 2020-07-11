#pragma once
#include <string>
#include <vector>
#include "PointFeatures.h"

class ImageGenerator {

public:
	ImageGenerator(std::string file_name, std::vector<std::vector<PointFeatures>> *data);
	bool GenerateBinaryMap();
	bool GenerateDepthMap(float max_depth, float min_depth);
	bool GenerateGrayMap(float max_intensity, float min_intensity);
	bool GenerateNormalMap(float max_normal, float min_normal);
	bool GenerateAll(float max_depth, float min_depth, float max_intensity, float min_intensity, float max_normal, float min_normal);
	bool GeneratePNGs(float max_depth, float min_depth, float max_intensity, float min_intensity, float max_normal, float min_normal, std::string out);

private:
	std::string file_name_;
	std::vector<std::vector<PointFeatures>> *data_;
};