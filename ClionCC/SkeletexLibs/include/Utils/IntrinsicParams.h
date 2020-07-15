#pragma once
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>

namespace utils
{
  /*!
    \brief Holds camera intrinsic projection parameters.

    More information about intrinsic camera parameters can be found on Wikipedia
    https://en.wikipedia.org/wiki/Camera_resectioning and distortion coefficients
    at https://docs.opencv.org/2.4/doc/tutorials/calib3d/camera_calibration/camera_calibration.html
  */
  struct IntrinsicParams
  {
    //! Focal length in terms of pixels.
    float fx, fy;
    //! Principal point coordinates. Ideally in the center of the image.
    float cx, cy;

    IntrinsicParams() = default;
    IntrinsicParams(float t_fx, float t_fy, float t_cx, float t_cy)
      : fx(t_fx), fy(t_fy), cx(t_cx), cy(t_cy) {}

    //! Calculates horizontal field of view. In radians.
    float GetFovX() const
    {
      return 2.0f * glm::atan(cx / fx);
    }
    //! Calculates vertical field of view. In radians.
    float GetFovY() const
    {
      return 2.0f * glm::atan(cy / fy);
    }
  };

  //! Returns intrinsic cx, cy parameters for defined projection depth and field of view angles.
  inline glm::vec2 GetPrincipalPoint(float projection_depth, float fovx, float fovy)
  {
    return glm::vec2(
        projection_depth * glm::tan(0.5f * fovx),
        projection_depth * glm::tan(0.5f * fovy)
      );
  }

  //! Calculates intrinsic projection parameters from resolution and field of view angles.
  inline IntrinsicParams GetIntrinsicParamsFromResolution(const glm::uvec2 &resolution, float fovx, float fovy)
  {
    const auto cx = 0.5f * static_cast<float>(resolution.x);
    const auto cy = 0.5f * static_cast<float>(resolution.y);
    const auto fx = cx / glm::tan(0.5f * fovx);
    const auto fy = cy / glm::tan(0.5f * fovy);
    return IntrinsicParams(fx, fy, cx, cy);
  }

  //! Calculates intrinsic projection parameters from projection depth and field of view angles.
  inline IntrinsicParams GetIntrinsicParamsFromDepth(float projection_depth, float fovx, float fovy)
  {
    const auto c = GetPrincipalPoint(projection_depth, fovx, fovy);
    const auto fx = c.x / glm::tan(0.5f * fovx);
    const auto fy = c.y / glm::tan(0.5f * fovy);
    return IntrinsicParams(fx, fy, c.x, c.y);
  }

}