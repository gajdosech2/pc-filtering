#pragma once
#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "..\HeaderFiles\SyntheticDataGenerator.h"
#include <COGS/Scan.h>
#include <direct.h>
#include <stdlib.h>

const std::string DATASETS_ROOT = "DataSets/";

void SyntheticDataGenerator::GenerateData1()
{
	cogs::Scan scan_dirty;
	cogs::Scan scan_clean;
	int SIZE = 50;

	scan_dirty.Reserve(SIZE * SIZE);
	scan_dirty.SetResolution(glm::uvec2(SIZE, SIZE));
	auto positions_dirty = scan_dirty.GetPositions();
	scan_dirty.AddNormals();
	scan_dirty.AddIntensities();
	auto normals_dirty = scan_dirty.GetNormals();

	scan_clean.Reserve(SIZE * SIZE);
	scan_clean.SetResolution(glm::uvec2(SIZE, SIZE));
	auto positions_clean = scan_clean.GetPositions();
	scan_clean.AddNormals();
	scan_clean.AddIntensities();
	auto normals_clean = scan_clean.GetNormals();

	for (uint32_t y = 0; y < SIZE; y++)
	{
		for (uint32_t x = 0; x < SIZE; x++)
		{
			auto dirty_id = scan_dirty.AddPointAt(x, y);
			if (rand() % 27 == 0) {
				positions_dirty[dirty_id] = glm::vec3(x / 1000.0f, y / 1000.0f, 0.01f);
				normals_dirty[dirty_id] = glm::vec3(0, 0, 1);
			}
			else {
				positions_dirty[dirty_id] = glm::vec3(x / 1000.0f, y / 1000.0f, 0.0f);
				normals_dirty[dirty_id] = glm::vec3(0, 0, 1);

				auto clean_id = scan_clean.AddPointAt(x, y);
				positions_clean[clean_id] = glm::vec3(x / 1000.0f, y / 1000.0f, 0.0f);
				normals_clean[clean_id] = glm::vec3(0, 0, 1);
			}

		}
	}
	Export(scan_dirty, "synthetic_03");
	Export(scan_clean, "synthetic_04");
}

void SyntheticDataGenerator::GenerateData2()
{
	cogs::Scan scan_dirty;
	cogs::Scan scan_clean;
	int SIZE = 50;

	scan_dirty.Reserve(SIZE * SIZE);
	scan_dirty.SetResolution(glm::uvec2(SIZE, SIZE));
	auto positions_dirty = scan_dirty.GetPositions();
	scan_dirty.AddNormals();
	scan_dirty.AddIntensities();
	auto normals_dirty = scan_dirty.GetNormals();

	scan_clean.Reserve(SIZE * SIZE);
	scan_clean.SetResolution(glm::uvec2(SIZE, SIZE));
	auto positions_clean = scan_clean.GetPositions();
	scan_clean.AddNormals();
	scan_clean.AddIntensities();
	auto normals_clean = scan_clean.GetNormals();

	for (uint32_t y = 0; y < SIZE; y++)
	{
		for (uint32_t x = 0; x < SIZE; x++)
		{
			auto dirty_id = scan_dirty.AddPointAt(x, y);
			if (x < SIZE/2) {
				positions_dirty[dirty_id] = glm::vec3(x / 1000.0f, y / 1000.0f, 0.01f);
				normals_dirty[dirty_id] = glm::vec3(0, 0, 1);
			}
			else {
				positions_dirty[dirty_id] = glm::vec3(x / 1000.0f, y / 1000.0f, 0.0f);
				normals_dirty[dirty_id] = glm::vec3(0, 0, 1);

				auto clean_id = scan_clean.AddPointAt(x, y);
				positions_clean[clean_id] = glm::vec3(x / 1000.0f, y / 1000.0f, 0.0f);
				normals_clean[clean_id] = glm::vec3(0, 0, 1);
			}

		}
	}
	Export(scan_dirty, "synthetic_05");
	Export(scan_clean, "synthetic_06");
}

void SyntheticDataGenerator::Export(cogs::Scan scan, std::string name)
{
	auto folder_path = DATASETS_ROOT + "/synthetic/";
	bool outer_dir_created = _mkdir(folder_path.c_str());
	auto file_path = folder_path + "/" + name + ".cogs";
	scan.Export(file_path);
}
