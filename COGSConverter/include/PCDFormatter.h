#pragma once
#include <string>
#include "ScanFormatter.h"

class PCDFormatter : public ScanFormatter
{
public:
	void GenerateInput(std::string out_path) override
	{
		data_.TransformToSpace(utils::PHOXI_CAMERA_SPACE);

		std::vector<glm::vec3> points;
		for (uint32_t y = 0; y < data_.GetHeight(); y++)
		{
			for (uint32_t x = 0; x < data_.GetWidth(); x++) {
				if (data_.IsPointAt(x, y) && rand() % 8 == 0)
				{
					auto id = data_.GetPointAt(x, y);
					points.push_back(data_.GetPositions()[id]);
					
				}
			}
		}

		std::ofstream file;
		file.open(out_path + file_name_ + ".pcd");

		file <<
			"# .PCD v.7 - Point Cloud Data file format" << std::endl <<
			"VERSION .7" << std::endl <<
			"FIELDS x y z" << std::endl <<
			"SIZE 4 4 4" << std::endl <<
			"TYPE F F F" << std::endl <<
			"COUNT 1 1 1" << std::endl <<
			"WIDTH " << points.size() << std::endl <<
			"HEIGHT 1" << std::endl <<
			"VIEWPOINT 0 0 0 1 0 0 0" << std::endl <<
			"POINTS " << points.size() << std::endl <<
			"DATA ascii" << std::endl
			;

		for (uint32_t i = 0; i < points.size(); i++)
		{
			file << std::fixed << points[i].x << " " << points[i].y << " " << points[i].z << std::endl;
		}

		file.close();
	};

	void GenerateTruth(std::string truth_path, std::string out_path) override {};
	void ProcessPrediction(std::string original_path, std::string prediction_path, std::string out_path = "") override {};
};