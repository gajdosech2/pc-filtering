/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <COGS/PointCloud.h>

#include <Visualizer/Object.h>



namespace vis
{

  struct VIS_API PointCloudStyle : public ObjectStyle
  {
    //! Defines source data for generating surface color.
    enum Texture
    {
      //! defined material properties
      CS_MATERIAL = 0,
      //! vertex 3D position
      CS_POSITIONS = 1,
      //! vertex normal vector
      CS_NORMALS = 2,
      //! custom provided color
      CS_CUSTOM_COLORS = 3,
      //! vertex normal vector, normalized for range <0,1>
      CS_NORMALS_N = 4
    };
    Material material;
    bool back_face_culling = false;
    float point_size = 10.0f;
    bool use_lighting = true;
    Texture color_source = CS_MATERIAL;

    // normals
    bool display_normals = false;
    cogs::Color3f normal_color = cogs::color::BLACK;
    float normal_length = 0.1f;

    StyleData GetMeshStyleData() const;
    bool LoadState(std::istream &str) override;
    void SaveState(std::ostream &str) override;
  };


  class VIS_API PointCloudObject : public Object
  {
  public:
    PointCloudObject(const cogs::PointCloud &pc);
    PointCloudObject(const PointCloudObject &object_ptr) = delete;
    virtual ~PointCloudObject() = default;
    bool IsCompatibileWithStyle(const ObjectStyle *style) override;
    void SetColors(const std::vector<cogs::Color3f> &colors);

  protected:
    PointCloudObject() = default;
    void Reset(const cogs::PointCloud &pc);
    void Render(const std::string &program) override;
    void LoadRequiredShaderPrograms(glw::ProgramList *programs) override;
    glw::PArrayObject array_object_;

  private:
    glw::ArrayObject::BufferName color_buffer_ = glw::ArrayObject::invalid_name;
  };


  using PPointCloudObject = std::shared_ptr<PointCloudObject>;
  using PPointCloudStyle = std::shared_ptr<PointCloudStyle>;

}


