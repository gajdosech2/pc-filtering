/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <set>
#include <memory>

#include <glm/glm.hpp>

#include <GLW/ArrayObject.h>
#include <GLW/UniformBuffer.h>
#include <GLW/ShaderProgramList.h>
#include <COGS/Color.h>
#include <COGS/MaterialModel.h>
#include <COGS/Transform.h>

#include <Visualizer/PrimitiveDatabase.h>



namespace vis
{
  class TextRenderer;

  //! Properties of simple phong material.
  struct VIS_API Material
  {
    Material() = default;
    Material(const cogs::Color3f &col);
    cogs::Color3f color = cogs::color::GRAY70;
    float specular = 0.5f;
    float shininess = 60.0f;
    float metalic = 0.0f;
  };

  //! Definition of basic rendering style properties (GPU compatible).
  struct VIS_API StyleData
  {
    cogs::Color3f color = cogs::color::GRAY70;
    float is_metalic = 0;
    float specular_intensity = 1.0f;
    float shininess = 60.0f;
    uint32_t use_lightning = 1;
    uint32_t force_flatshading = 0;
    cogs::Color3f edge_color = cogs::color::GRAY30;
    uint32_t render_edges = 0;
    float edge_thickness = 0.00001f;
    uint32_t fragcolor = 0u;
    uint32_t cull_backfaces = false;
    float __padding;
  };

  //! Base class for setting rendering style of an arbitrary render object.
  struct VIS_API ObjectStyle
  {
    bool visible = true;
    cogs::Transform transform;
    virtual ~ObjectStyle() = default;
    virtual bool LoadState(std::istream &str);
    virtual void SaveState(std::ostream &str);
  protected:
    bool ReadVersion(std::istream &str, const uint32_t expected_version);
  };
  using PObjectStyle = std::shared_ptr<ObjectStyle>;


  //! Base class for an arbitrary renderer object.
  class VIS_API Object
  {
    friend class Renderer;
  public:
    Object();
    //! Test whether specified style is compatible with object.
    virtual bool IsCompatibileWithStyle(const ObjectStyle *style) = 0;

  protected:

    void SetStyleData(const StyleData &sdata);

    //! Override this method to load specialized shader programs for your object.
    virtual void LoadRequiredShaderPrograms(glw::ProgramList *programs);
    //! Override this method to calculate updated data before render call.
    virtual void Update();
    //! Override this method to specify custom rendering behavior.
    virtual void Render(const std::string &program);

    virtual void Render2D(const std::string &program);

    virtual void FillTextRenderer(vis::TextRenderer *t_renderer);

    //! Obtain currently set object style of specific type. Can be used only in Update-Render stage!
    template <typename StyleType>
    const StyleType *GetStyle() { return static_cast<const StyleType *>(style_); }

  private: // changes to this section are made only by Renderer
    glw::UniformBuffer<StyleData> styledata;
    const ObjectStyle *style_;
  };


  using PObject = std::shared_ptr<Object>;

}