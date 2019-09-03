/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <deque>

#include <utils/GeometryStructures.h>

#include <Visualizer/Camera.h>
#include <Visualizer/Object.h>
#include <Visualizer/Light.h>



namespace vis
{

  enum class GroundGrid
  {
    none = 0,
    xy = 1,
    yz = 2,
    zx = 3
  };

  /*!
    \brief Basic unit to specify single rendering viewport, contains multiple object instances.

    A scene represents single viewport and can hold multiple object instances for vis::Renderer.
    Additionally, it contains internal vis::Camera object and projection matrix.
  */
  class VIS_API Scene
  {
    friend class Renderer;
  public:

    bool render_world_axes = true; //! Whether to render world axes in this scene view.
    GroundGrid render_ground_grid = GroundGrid::none;
    float ground_grid_scale = 1.0f;

    Scene() = default;
    Scene(int32_t x, int32_t y, int32_t width, int32_t height);
    Scene(const Scene &) = delete;

    /*!
      \brief
          For a 3D point, transfers screen movement into world, in parallel to camera projection plane.
      \param point
          Point for which the resulting movement vector will be calculated.
      \param proj_move_origin
          Screen-space movement start.
      \param proj_move_end
          Screen-space movement end.
      \return
          World-space movement vector.
    */
    glm::vec3 MoveInProjectionSpace(const glm::vec3 &point, const glm::vec2 &proj_move_origin, const glm::vec2 &proj_move_end);

    //! Returns camera object.
    vis::Camera *GetCamera();

    //! Returns number of instances added to the scene.
    uint32_t GetInstanceCount();

    /*!
      \brief
          Adds new instance to the scene.
      \param layer
          Specifies rendering order. Objects on larger layers are rendered on top. Default layer is 0.
      \return
          A non-negative index of instance. This index is unique for each instance in a single scene.
          If instance addition fails, function returns -1.
    */
    int32_t AddInstance(const PObject object, const PObjectStyle style, const uint8_t layer = 0);

    //! Removes instance by its index.
    void RemoveInstance(int32_t index);

    //! Removes all instances.
    void ClearInstances();

    //! Add index of renderer light into scene, which should influence rendered objects.
    void AddLight(const uint32_t &light);

    //! Set indices of renderer lights into scene, which should influence rendered objects.
    void SetLights(const std::vector<uint32_t> &lights);

    //! Setup scene projection matrix into perspective projection.
    void SetProjectionPersp(float fov, float near_plane = 0.01f, float far_plane = 1000.0f);

    //! Setup scene projection matrix into orthographic projection.
    void SetProjectionOrtho(float sizing, float near_plane = 0.01f, float far_plane = 1000.0f);

    //! Setup scene projection matrix into orthographic projection.
    void SetProjectionCustom(const glm::mat4 &proj_matrix);

    //! Setup scene projection matrix based on camera intrinsic parameters
    void SetProjectionCameraParams(const glm::uvec2 &resolution, float fx, float fy, float cx, float cy, float near_plane = 0.01f, float far_plane = 1000.0f);

    //! Set scene projection matrix to custom matrix
    void SetProjMatrix(const glm::mat4 &proj_mat);

    //! Returns current projection matrix of a scene.
    const glm_ext::TransMat4 &GetProjectionMat() const;

    //! Returns current view and projection matrix of a scene.
    glm_ext::TransMat4 GetViewProjMat() const;

    //! Returns current view matrix of a scene.
    glm_ext::TransMat4 GetViewMat() const;

    //! Sets up rendering viewport for a scene.
    void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

    //! Returns rendering viewport for a scene.
    const geom::IRect &GetViewport() const;

    //! Returns near plane distance.
    float GetNearDistance() const { return proj_near_plane_; }
    //! Returns far plane distance.
    float GetFarDistance() const { return proj_far_plane_; }

    //! Returns the camera's field of view angle in x (camera right) direction.
    float GetFovX() const;
    //! Returns the camera's field of view angle in y (camera up) direction.
    float GetFovY() const;

    //! Compute the camera's aspect ratio (w/h);
    float GetAspectRatio() const;

  private:

    struct VIS_API Instance
    {
    public:
      int32_t index;
      PObject object;
      PObjectStyle style;
      uint8_t layer;
    };

    enum class ProjectionType
    {
      perspective = 0,
      ortho = 1,
      custom = 2
    };

    int32_t next_index_ = 0; //! index of instance which will be added next
    std::deque<Instance> instances_; //! all instances which will be rendered in this scene

    geom::IRect viewport_; //! scene rendering viewport

    Camera camera_; //! camera view object

    ProjectionType proj_type_ = ProjectionType::perspective; //! whether uses orthographic projection
    float proj_ortho_sizing_ = 1.0f; //! scale of orthographic projection volume
    float proj_fov_ = QUAT_PI; //! field of view in radians
    float proj_near_plane_ = 0.01f; //! near projection volume plane
    float proj_far_plane_ = 1000.0f; //! far projection volume plane
    glm_ext::TransMat4 projection_; //! projection matrix

    std::vector<uint32_t> lights_; //! renderer light indices, which are used to lit scene objects

    bool data_has_changed_ = true; //! flag used by renderer to be informed on scene member change

    //! Recalculates projection matrix based on projection parameters and viewport.
    void RecalculateProjection();
  };

  using PScene = std::shared_ptr<Scene>;


}