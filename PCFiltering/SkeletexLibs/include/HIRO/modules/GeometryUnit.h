#pragma once
#include <Visualizer/GeometryObj.h>
#include <HIRO/modules/MeshUnit.h>



namespace hiro
{
  namespace modules
  {
    class HIRO_API GeometryUnit : public hiro::modules::MeshUnit
    {
    public:
      GeometryUnit(const std::string &name, vis::GeometryName primitive);
      hiro::PDataInstance Instantiate() override;
    };
  }
}
