/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <GLW/Texture.h>
#include <GLW/UniformBuffer.h>
#include <HIRO_DRAW/ElementRenderer.h>
#include <HIRO_DRAW/ShaderApi.h>



namespace hiro::draw
{

  //! Stylization of the rendered geometry.
  struct HIRO_DRAW_API GeometryStyle : public Style
  {
    //! Available modes for rendering geometry.
    enum class RenderMode : int32_t
    {
      points = 0, //!< Renders only vertices.
      wires = 1, //!< Renders only edges.
      wired_points = 2, //!< Renders vertices and edges.
      faces = 3, //!< Renders only solid faces.
      wired_faces = 4 //!< Renders solid faces with edges on top.
    };

    //! Available stylization presets.
    enum class PresetType
    {
      plaster, //!< White matte surface.
      golden, //!< Shiny yellowish surface.
      colorized //!< Every surface direction of different color.
    };

    //! Mode currently used for rendering geometry.
    RenderMode render_mode = RenderMode::faces;

    //! Material setting for the faces or points (depending on a selected render mode).
    hiro::shader::Material material;
    //! When enabled, faces and edges with normal vector turned away from the camera are hidden.
    bool back_face_culling{ false };

    //! Size of point in screen pixels. Used only for render modes that include points.
    float point_size{ 10.0f };

    //! Color source used to colorize faces and points.
    ColorSource color_source{ ColorSource::material };
    //! Albedo texture. To use, set color source to ColorSource::albedo_map.
    glw::PTexture2D albedo_texture{ nullptr };

    //! Normal source used to calculate the lighting.
    FacesStyle::NormalSource normal_source{ FacesStyle::NormalSource::flat_normals };
    //! Normal texture. To use, set normal source to NormalSource::normal_map.
    glw::PTexture2D normal_texture{ nullptr };
    //! When enabled, normal vector for every point is rendered.
    bool display_normals{ false };
    //! Color of normal vector tips. Used only when normals are displayed.
    cogs::Color3f normal_color{ cogs::color::BLACK };
    //! Length of normal vectors. Used only when normals are displayed.
    float normal_length{ 0.1f };

    //! Color of the edges. Used only for render modes that include edges.
    cogs::Color3f edge_color{ cogs::color::BLACK };
    //! Width of the edges. Used only for render modes that include edges.
    float edge_width{ 1.0f };

    //! When enabled, color and lighting is picked from matcap sampler texture.
    bool use_matcap{ false };
    //! Matcap sampler. Used only when matcap rendering is enabled.
    glw::PTexture2D matcap_sampler{ nullptr };

    //! Creates a new geometry style from the defined preset.
    static hiro::draw::GeometryStyle GetPreset(PresetType type);

    //! Imports state from stream.
    bool ReadFromStream(std::istream &str) override;
    //! Exports state to stream.
    void WriteToStream(std::ostream &str) override;
  };



  /*! Renderer used for rendering geometry.

    Geometry can be set to this renderer by two ways. Eider by specifying geometry name,
    or by specifying your own geometry via vertex buffer.

    | Right | Center | Left  |
    | ----: | :----: | :---- |
    | 10    | 10     | 10    |
    | 1000  | 1000   | 1000  |

    When specifying your own geometry, vertices should have following attributes:

    | Location | Type | Content            | Required                                             |
    | -------- | ---- | ------------------ | ---------------------------------------------------- |
    | 0        | vec3 | position           | yes                                                  |
    | 1        | vec3 | normal             | when normal source is NormalSource::point_normals    |
    | 2        | vec2 | texture coordinate | when using any type of texture                       |
    | 3        | vec3 | custom color       | when color source is ColorSource::custom_colors      |
    | 4        | vec4 | tangent            | when normal source is NormalSource::normal_texture   |

  */
  class HIRO_DRAW_API GeometryRenderer : public ElementRenderer
  {
  public:
    //! Creates empty geometry renderer without any geometry.
    GeometryRenderer() = default;
    //! Creates a renderer with the geometry.
    GeometryRenderer(GeometryName geometry_name);
    //! Creates a renderer with the custom geometry.
    GeometryRenderer(glw::PArrayObject &vao);

    GeometryRenderer(const GeometryRenderer &source) = delete;
    GeometryRenderer &operator=(const GeometryRenderer &source) = delete;
    GeometryRenderer(GeometryRenderer &&) noexcept = delete;
    GeometryRenderer &operator=(GeometryRenderer &&) noexcept = delete;
    virtual ~GeometryRenderer() = default;

    //! Replaces the geometry in the renderer.
    void Set(GeometryName geometry_name);
    //! Replaces the geometry in the renderer.
    void Set(glw::PArrayObject &vao);

    //! Test whether specified style is compatible with the object.
    bool IsCompatibileWithStyle(const Style *style) override;

  protected:
    //! Defines behavior on face rendering.
    virtual void OnRenderFaces() override;
    //! Defines behavior on point rendering.
    virtual void OnRenderPoints() override;
    //! Defines behavior on normal rendering.
    virtual void OnRenderNormals() override;
    //! Defines behavior on simple rendering.
    virtual void OnRenderSimple() override;

  private:
    //! Array buffer used to store attributes of currently rendered geometry.
    glw::PArrayObject vao_;
  };



  //! Shared pointer to an object of the type GeometryStyle.
  using PGeometryStyle = std::shared_ptr<GeometryStyle>;
  //! Shared pointer to an object of the type GeometryRenderer.
  using PGeometryRenderer = std::shared_ptr<GeometryRenderer>;

}
