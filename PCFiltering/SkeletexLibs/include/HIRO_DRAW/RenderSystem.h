/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <array>

#include <GLW/FrameBuffer.h>
#include <GLW/UniformBuffer.h>
#include <Utils/GeometryStructures.h>

#include <HIRO_DRAW/Renderer.h>
#include <HIRO_DRAW/Scene.h>
#include <HIRO_DRAW/ShaderApi.h>
#include <HIRO_DRAW/PrimitiveDatabase.h>
#include <HIRO_DRAW/TextRenderer.h>



namespace hiro::draw
{

  /*!
    \brief Class responsible for rendering loop implementation and its optimization.
  */
  class HIRO_DRAW_API RenderSystem
  {
  public:
    RenderSystem(const glm::uvec2 &size);

    //! Creates instance with shared shader program list.
    RenderSystem(const glm::uvec2 &size, const glw::PProgramList &programlist_ptr);

    //! Set up size of output buffer, targeted for render.
    void SetFramebufferSize(const glm::uvec2 &size);

    //! Set up directory from where render objects load their shaders.
    void SetShaderBuildDirectory(const std::string &directory);

    //! Set up initial buffer color. Only used if enabled in Render method.
    void SetBackgroundColor(const cogs::Color3f &color);

    //! Returns handle to the TextRenderer class.
    hiro::draw::TextRenderer *GetTextRenderer();

    //! Add a scene to the list of scenes to be rendered. Returns scene index.
    uint32_t AddScene(const PScene &scene);

    //! Removes the scene from the list of scenes to be rendered.
    void RemoveScene(const PScene &scene);

    //! Returns scene from the list of scenes to be rendered, by scene index.
    PScene GetScene(const uint32_t scene_id);

    //! Add light to the renderer, returns index of the added light .
    uint32_t AddLight(const hiro::shader::Light &light);

    //! Loads all not-loaded shaders required by renderer and all objects from added scenes.
    void LoadShaders();

    //! Forces reload of all shaders required by renderer and all objects from added scenes.
    void ReloadShaders();

    //! Render all objects from all added scenes.
    void Render();

    //! Set whether The renderer should use MSAA. Disabled by default.
    void SetMsaaEnabled(bool enabled);

    //! Checks whether is MSAA currently in use.
    bool IsMsaaEnabled();

    //! Set number of samples to be used for MSAA. Default is 4.
    bool SetMsaaSamples(uint8_t sample_count);

    //! Returns number of samples currently used for MSAA.
    uint32_t GetMsaaSamples();

    //! Returns current resolution of render frame buffer.
    glm::uvec2 GetFrameBufferSize();

    /*!
      \brief Returns 4-channel texture of position and shininess parameters.

      Stored data is of type float32. First 3 channels are position x, y, z.
      The last channels is material shininess parameter.
    */
    glw::PTexture2D GetPositionsAndShininessTex() const;

    /*!
      \brief Returns 4-channel texture of normal and intensity parameters.

      Stored data is of type float32. First 3 channels are normal vector x, y, z.
      The last channels is material specular intensity parameter.
    */
    glw::PTexture2D GetNormalsAndSpecularTex() const;

    /*!
      \brief Returns 4-channel texture of color and lighting parameters.

      Stored data is of type uint8. First 3 channels are albedo color r,g,b.
      The last channels is boolean parameter telling whether lighting is use for the pixel.
    */
    glw::PTexture2D GetColorsAndLightingTex() const;

  private:

    glm::uvec2 framebuffer_size_;

    cogs::Color3b back_color_ = cogs::color::WHITE;

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
    glw::FrameBuffer::Settings g_buf_settings_;
    glw::PFrameBuffer g_buffer_;
    glw::PFrameBuffer h_buffer_;
    bool is_msaa_enabled_ = true;
    uint8_t msaa_samples_ = 4;

    std::vector<PScene> scenes_;

    glw::UniformBuffer<MatAndInv> cameraview_buffer_;

    uint32_t added_light_count_ = 0u;
    glw::UniformBuffer<hiro::shader::Light> light_buffer_;

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

    //! Return non-multi-sampled copy of g_buffer.
    glw::PFrameBuffer GetNonMSgbuffer() const;

    //! Create G-Buffer for deferred rendering.
    void CreateGBuffer();

    //! Create an intermediate buffer between Screen buffer and G-Buffer.
    void CreateHBuffer();

    //! Use deferred rendering lighting path.
    void MergeGBuffer();

    //! Create a single multi-sampled texture from G-Buffer.
    void MergeHBuffer();

    //! Bind the G-Buffer to render into.
    void InitDeferredRendering();

    void Render3D(std::vector<uint8_t> &used_layers);

    //! Swap buffers G-Buffer, H-Buffer, Screen buffer
    void MergeBuffers(const gl::GLint &fbo_bound);

    void Render2D(std::vector<uint8_t> &used_layers);
  };

}
