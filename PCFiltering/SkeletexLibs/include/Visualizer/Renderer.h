/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <array>

#include <GLW/FrameBuffer.h>
#include <GLW/UniformBuffer.h>
#include <utils/GeometryStructures.h>

#include <Visualizer/Object.h>
#include <Visualizer/Scene.h>
#include <Visualizer/Light.h>
#include <Visualizer/ShaderDefinitions.h>
#include <Visualizer/PrimitiveDatabase.h>
#include <Visualizer/TextRenderer.h>



namespace vis
{

  /*!
    \brief Class responsible for rendering loop implementation and its optimization.
  */
  class VIS_API Renderer
  {
  public:
    Renderer(const glm::uvec2 &size);

    //! Creates instance with shared shader program list.
    Renderer(const glm::uvec2 &size, const glw::PProgramList &programlist_ptr);

    //! Set up size of output buffer, targeted for render.
    void SetFramebufferSize(const glm::uvec2 &size);

    //! Set up directory from where render objects load their shaders.
    void SetShaderBuildDirectory(const std::string &directory);

    //! Set up initial buffer color. Only used if enabled in Render method.
    void SetBackgroundColor(const cogs::Color4f &color);

    //! Returns handle to the TextRenderer class.
    vis::TextRenderer *GetTextRenderer();

    //! Add a scene to the list of scenes to be rendered. Returns scene index.
    uint32_t AddScene(const PScene scene);

    //! Returns scene from the list of scenes to be rendered, by scene index.
    PScene GetScene(const uint32_t scene_id);

    //! Add light to the renderer, returns index of the added light .
    uint32_t AddLight(const Light &light);

    //! Loads all not-loaded shaders required by renderer and all objects from added scenes.
    void LoadShaders();

    //! Forces reload of all shaders required by renderer and all objects from added scenes.
    void ReloadShaders();

    //! Render all objects from all added scenes.
    void Render();

    //! Set number of samples to be used for MSAA. Default is disabled (0 or 1).
    bool SetMsaaSamples(uint8_t sample_count);

    //! Return number of samples currently used for MSAA.
    uint32_t GetMsaaSamples();

    glm::uvec2 GetFrameBufferSize();

  private:

    glm::uvec2 framebuffer_size_;

    cogs::Color4f back_color_ = cogs::color::WHITE;

    struct MatAndInv
    {
      MatAndInv(const glm::mat4 &mat);
      glm::mat4 matrix;
      glm::mat4 inverse;
    };
    struct SceneData
    {
      glm::mat4 projection;
      glm::mat4 projection_inverse;
      int32_t camera_id;
      glm::vec3 padding0;
      glm::ivec4 light_ids;
      glm::ivec2 resolution;
      glm::vec2 padding1;
    };

    TextRenderer text_renderer_;

    glw::PProgramList programs_;
    glw::PFrameBuffer g_buffer_;
    glw::PFrameBuffer h_buffer_;
    uint8_t custom_sample_count_ = 4;

    std::vector<PScene> scenes_;

    glw::UniformBuffer<MatAndInv> cameraview_buffer_;

    uint32_t added_light_count_ = 0u;
    glw::UniformBuffer<Light> light_buffer_;

    glw::UniformBuffer<SceneData> scenedata_buffer_;
    glw::PArrayObject grid_buffer_;

    void CreateGridBuffer();

    void UpdateCameraBuffers();
    void UpdateSceneDataBuffers();

    void PrepareSceneForRender(const uint32_t scene_id);

    void RenderGrid(uint32_t scne_id);
    void RenderAxes(uint32_t i);

    SceneData GetSceneData(const uint32_t scene_id);

    void LoadSceneShaders(const uint32_t scene_id);

    //! Create G-Buffer for deferred rendering.
    void CreateGBuffer();

    //! Create an intermediate buffer between Screenbuffer and G-Buffer.
    void CreateHBuffer();

    //! Use deferred rendering lighting path.
    void MergeGBuffer();

    //! Create a single multisampled texture from G-Buffer.
    void MergeHBuffer();

    //! Bind the G-Buffer to render into.
    void InitDeferredRendering();

    void Render3D(std::vector<uint8_t> &used_layers);

    //! Swap buffers G-Buffer, H-Buffer, Screenbuffer
    void MergeBuffers(const gl::GLint &fbo_bound);

    void Render2D(std::vector<uint8_t> &used_layers);
  };

}
