#pragma once
#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "..\HeaderFiles\Visualization.h"
#include <HIRO/HIRO.h>
#include <HIRO/modules/GeometryUnit.h>
#include <HIRO/modules/PointCloudUnit.h>
#include <COGS/Scan.h>

void Visualization::Visualize()
{
	hiro::SetResourceDirectory("SkeletexLibs/hiro_resources");
	hiro::SetIntermediateDirectory("Intermediate/");

	hiro::Initialize();

	cogs::Scan scan1;
	scan1.Import("DataSets/apple/apple_02.cogs");
	scan1.Transform(glm::mat4(20));
	auto pc_unit1 = std::make_shared<hiro::modules::PointCloudUnit>("PointCloudUnit1", scan1);
	hiro::AddUnit(pc_unit1);

	cogs::Scan scan2;
	scan2.Import("DataSets/apple/truth_02.cogs");
	scan2.Transform(glm::mat4(20));
	auto pc_unit2 = std::make_shared<hiro::modules::PointCloudUnit>("PointCloudUnit2", scan2);
	hiro::AddUnit(pc_unit2);

	auto geometry_unit = std::make_shared<hiro::modules::GeometryUnit>("GeometryUnit", vis::GeometryName::sphere_s2);
	hiro::AddUnit(geometry_unit);

	while (hiro::IsOpen())
	{
		hiro::Update();
	}
}
