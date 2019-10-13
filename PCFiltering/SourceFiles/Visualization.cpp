#pragma once
#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "..\HeaderFiles\Visualization.h"
#include <HIRO/HIRO.h>
#include <HIRO/modules/GeometryUnit.h>
#include <HIRO/modules/PointCloudUnit.h>
#include <COGS/Scan.h>
#include <sstream>

void Visualization::Visualize(std::string dataset, int size)
{
  hiro::SetResourceDirectory("SkeletexLibs/hiro_resources");
  hiro::SetIntermediateDirectory("Intermediate/");

  hiro::Initialize();
  std::stringstream s_folder;
  s_folder << "DataSets/" << dataset << "/";
  std::string folder = s_folder.str();
  for (int i = 1; i <= size; i++)
  {
    cogs::Scan scan;
    std::stringstream s_import;
    s_import << folder << dataset << "_" << std::setfill('0') << std::setw(2) << i << ".cogs";
    scan.Import(s_import.str());
    scan.Transform(glm::mat4(20));
    std::stringstream s_name;
    s_name << "PointCloudUnit" << i;
    auto pc_unit = std::make_shared<hiro::modules::PointCloudUnit>(s_name.str(), scan);
    hiro::AddUnit(pc_unit);
  }

  while (hiro::IsOpen())
  {
    hiro::Update();
  }
}
