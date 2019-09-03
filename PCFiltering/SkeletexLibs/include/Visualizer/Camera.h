/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <functional>
#include <glm/glm.hpp>
#include <utils/GeometryStructures.h>
#include <utils/ExtGLM.h>
#include <Visualizer/API.h>



namespace vis
{

  /*!
    \brief A wrapper over view matrix, providing intuitive control.

    Position - defines position of camera on CameraMode::MOVE_FREE and target point on other modes.
    Displacement - distance to camera position, non zero only when mode is not CameraMode::MOVE_FREE.
  */
  class VIS_API Camera
  {
  public:

    using CallbackOnChange = std::function<void(void)>;

    //! Creates orbiting camera looking from position (0,0,10), up vector directing positive Y axis.
    Camera();

    //! Sets up view to exactly match the one of specified camera.
    void CopyView(const Camera &source_camera);

    //! Returns current view matrix.
    const glm_ext::TransMat4 &GetViewMatrix() const;

    //! Returns forward vector of camera orientation.
    glm::vec3 GetForward() const;

    //! Returns right vector of camera orientation.
    glm::vec3 GetRight() const;

    //! Returns up vector of camera orientation.
    glm::vec3 GetUp() const;

    //! Set position of camera.
    void SetPosition(const glm::vec3 &camera_position);

    //! Sets up up vector to match specified.
    void AlignUpVector(const glm::vec3 &vector);

    //! Sets up forward vector to match specified.
    void LookInDirection(const glm::vec3 &new_forward);

    //! Sets look direction of camera so that it looks from the actual position at defined point of interest.
    void LookAt(const glm::vec3 &point_of_interest, const glm::vec3 &up);

    //! Set position of camera and sets look direction of camera so that it looks from the actual position at defined point of interest.
    void SetPositionAndLookAt(const glm::vec3 &camera_position, const glm::vec3 &point_of_interest, const glm::vec3 &up);

    //! Returns actual position of camera.
    glm::vec3 GetPositon() const;

    //! Rotates camera by a specified quaternion.
    void Rotate(const glm::quat &rot);

    //! Moves forward.
    void Dolly(const float distance);

    //! Moves right.
    void Crab(const float distance);

    //! Moves up.
    void Boom(const float distance);

    //! Rotates towards left side.
    void Pan(const float angle);

    //! Rotates towards top side.
    void Tilt(const float angle);

    //! Rotates around its view direction, counter-clockwise.
    void Roll(const float angle);

    //! Moves camera position by a specified 3D vector.
    void Move(const glm::vec3 &move_vector);

    //! Loads all state values from stream.
    void LoadState(std::istream &str);

    //! Saves all state values to stream.
    void SaveState(std::ostream &str);

    //! Subscribes specified function to be called whenever camera changes.
    void SubscribeChange(const CallbackOnChange &listener);

    //! Resets all camera values to default.
    void Reset();

  protected:
    glm::vec3 position_ = glm::vec3(0.0f);
    glm::vec3 basis_z_ = glm::vec3(0.0f, 0.0f, -1.0f); // camera backward direction
    glm::vec3 basis_y_ = glm::vec3(0.0f, 1.0f, 0.0f); // camera up direction
    glm_ext::TransMat4 view_matrix_;
    std::vector<CallbackOnChange> listeners_;
    void UpdateCameraSpace();
  };

}