/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <Visualizer/PointCloudObject.h>
#include <GLW/Texture.h>



namespace cogs { struct Mesh; }

namespace vis
{

  struct VIS_API MeshStyle : public PointCloudStyle
  {
    enum Mode
    {
      MODE_POINTS = 0,
      MODE_WIRES = 1,
      MODE_POINTS_AND_WIRES = 2,
      MODE_FACES = 3
    };

    enum TextureMesh
    {
      TEX_CHECKERBOARD = 5,
      ALBEDO_TEXTURE = 6
    };

    enum class PresetType
    {
      plaster,
      golden,
      colorized
    };

    bool force_flatshading = true;
    bool display_edges = false;
    cogs::Color3b edge_color = cogs::color::BLACK;
    uint8_t edge_width = 1u;
    Mode render_mode = MODE_FACES;

    glw::PTexture2D albedo_texture = nullptr;
    glw::PTexture2D normal_texture = nullptr;

    StyleData GetMeshStyleData() const;
    bool LoadState(std::istream &str) override;
    void SaveState(std::ostream &str) override;

    static vis::MeshStyle GetPreset(PresetType type);
  };



  class VIS_API MeshObject : public PointCloudObject
  {
  public:
    MeshObject(const cogs::Mesh &mesh);
    MeshObject(const MeshObject &object_ptr) = delete;
    virtual ~MeshObject() = default;
    bool IsCompatibileWithStyle(const ObjectStyle *style) override;

  protected:
    MeshObject() = default;
    void LoadRequiredShaderPrograms(glw::ProgramList *programs) override;
    void BindRenderData(const vis::MeshStyle *style);
    void Render(const std::string &program) override;
    void RenderEdges(const MeshStyle *style);
  };


  using PMeshStyle = std::shared_ptr<MeshStyle>;
  using PMeshObject = std::shared_ptr<MeshObject>;

}


