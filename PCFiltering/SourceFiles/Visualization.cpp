#pragma once
#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "..\HeaderFiles\Visualization.h"
#include <HIRO/HIRO.h>
#include <HIRO/modules/PointCloudResource.h>
#include <HIRO/modules/GeometryResource.h>
#include <HIRO/modules/MeshResource.h>
#include <HIRO/modules/TextureResource.h>
#include <COGS/Scan.h>
#include <sstream>

void Visualization::Visualize(std::string dataset, int size)
{
  hiro::SetAssetDirectory("SkeletexLibs/assets");
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
    auto pc_unit = std::make_shared<hiro::modules::PointCloudResource>(s_name.str(), scan);
    hiro::AddResource(pc_unit);
  }

  while (hiro::IsOpen())
  {
    hiro::Update();
  }
}

void Visualization::Hiro()
{
    hiro::SetAssetDirectory("SkeletexLibs/assets");
    hiro::SetIntermediateDirectory("Intermediate/");

    hiro::Initialize();
    
    cogs::Scan scan;
    scan.Import("DataSets/bmw/bmw_22.cogs");
    scan.Transform(glm::mat4(10));
    auto pc_unit = std::make_shared<hiro::modules::PointCloudResource>("PointCloud", scan);
    hiro::AddResource(pc_unit);

    //hiro::Debug()->Log("HIRO Debugger showcase!");
    //const geom::Aabb3 box{ glm::vec3(-0.5f), glm::vec3(0.5f) };
    //hiro::Debug()->Box(box, cogs::color::LIME, 60);

    //const geom::Sphere sphere{ glm::vec3(0.0f), 0.3f };
    //hiro::Debug()->Sphere(sphere, cogs::color::YELLOW, 60);

    //std::vector<geom::LineSegment3> lines{ geom::LineSegment3(0,-1,0, 0,1,0) };
    //hiro::Debug()->Lines(lines, cogs::color::LIME, 60);

    //auto resource = std::make_shared<hiro::modules::GeometryResource>(
    //    "Single Resource",
    //    hiro::draw::GeometryName::sphere_s2
    //    );
    //hiro::AddResource(resource);

    cogs::Mesh mesh;
    mesh.Import("DataSets/other/cog.fbx");
    auto mesh_unit = std::make_shared<hiro::modules::MeshResource>("Model", mesh);
    hiro::AddResource(mesh_unit);

    auto tex1 = glw::Texture2D::CreateFromFile("DataSets/other/geo.png");
    auto tex_unit1 = std::make_shared<hiro::modules::TextureResource>("Image", tex1);
    hiro::AddResource(tex_unit1);


    while (hiro::IsOpen())
    {
       hiro::Update();
    }
}
