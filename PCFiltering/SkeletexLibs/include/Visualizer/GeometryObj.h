/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <Visualizer/MeshObject.h>



namespace vis
{

  using GeometryStyle = MeshStyle;
  using PGeometryStyle = std::shared_ptr<GeometryStyle>;



  class VIS_API GeometryObj : public MeshObject
  {
  public:
    GeometryObj(GeometryName gtype);
    GeometryObj(const GeometryObj &object_ptr) = delete;
    virtual ~GeometryObj() = default;
    const vis::Geometry &Get();
  private:
    GeometryName name_;
  };



  using PGeometryObj = std::shared_ptr<GeometryObj>;

}