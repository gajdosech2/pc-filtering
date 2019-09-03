/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <Visualizer/Scene.h>
#include <COGS/Frustum.h>
#include <HIRO/API.h>



namespace hiro
{

  /*!
    \brief Provides interface to viewport camera which is used for rendering a scene.
  */
  class HIRO_API Camera
  {
  public:

    struct Pose
    {
      glm::vec3 target;
      glm::vec3 position;
      glm::vec3 up;
    };

    const uint32_t id; //! Index of Viewarea object which owns this Camera.
    const vis::PScene scene; //! Pointer to renderer scene.

    Camera(const uint32_t c_id, const vis::PScene &scene_ptr);
    Camera(const Camera &cp);
    ~Camera();

    //! Returns the camera position.
    glm::vec3 GetPosition() const;

    //! Set position of camera. The view is automatically aligned
    void SetPosition(const glm::vec3 &camera_position);

    //! Returns point of interest for camera.
    glm::vec3 GetTarget() const;

    //! Sets up point of interest for camera, which will automatically look at the point.
    //! If camera mode is set to one of orbital modes, camera will orbit the point.
    void SetTarget(const glm::vec3 &poi);

    //! Returns up vector of camera orientation.
    glm::vec3 GetUp() const;

    //! Aligns up direction of camera to match .
    void AlignUp(const glm::vec3 &up);

    //! Changes distance of camera to target point. Distance is multiplied by defined factor.
    void Zoom(const float factor);

    //! Get whether rotation and camera up vector are fixed.
    bool IsFixedRotationEnabled() const;

    //! Set whether to fix rotation and up vector to axis specified by SetFixedRotationAxis function. Default value false.
    void SetFixedRotationEnabled(bool state);

    //! Returns fixed rotation axis and up vector.
    const glm::vec3 &GetFixedRotationAxis() const;

    //! Set fixed rotation axis and up vector to specified. Used when enabled using SetFixedRotationEnabled function. Default value (0,1,0).
    void SetFixedRotationAxis(const glm::vec3 &axis);

    //! Updates camera position.
    void Update(double delta_time, const std::set<int32_t> &pressed_keys);

    //! Rotates the camera.
    void ScreenRotate(const glm::vec2 &last_mouse_pos, const glm::vec2 &mouse_pos);

    //! Rotates around its view direction, counter-clockwise.
    void ScreenRoll(const glm::vec2 &last_mouse_pos, const glm::vec2 &mouse_pos);

    //! Moves camera position by a specified 3D vector.
    void ScreenMove(const glm::vec2 &last_mouse_pos, const glm::vec2 &mouse_pos);

    //! Moves forward.
    void ScreenDolly(const glm::vec2 &last_mouse_pos, const glm::vec2 &mouse_pos);

    //! Sets up optional callback function, which will be called each time a scene camera changes it's state.
    void SubscribeChange(const std::function<void(void)> &listener);

    //! Sets up view to exactly match the one of specified camera.
    void CopyView(const hiro::Camera &source_cam);

    //! Loads all state values from stream.
    void LoadState(std::istream &str);

    //! Saves all state values to stream.
    void SaveState(std::ostream &str);

    //! Resets all camera variables and sets camera position to a predefined value.
    void Reset();

    //! Returns current Pose.
    Pose GetPose();

    //! Sets Pose for Camera.
    void SetPose(const Pose &pose);

    //! Compute the camera's view frustum.
    cogs::Frustum ComputeViewFrustum() const;

  private:
    void CorrectCameraPositionAgainstPoi();
    void CorrectUpVectorIfRequired();
    glm::vec3 MoveInProjectionSpace(const glm::vec3 &point, const glm::vec2 &proj_move_origin, const glm::vec2 &proj_move_end);

    struct Impl;
    std::unique_ptr<Impl> m;
  };


  hiro::Camera::Pose Interpolate(hiro::Camera::Pose p1, hiro::Camera::Pose p2, const float t);
  hiro::Camera::Pose InterpolateCubic(hiro::Camera::Pose p1, hiro::Camera::Pose p2, hiro::Camera::Pose p3, hiro::Camera::Pose p4, const float t);

}
