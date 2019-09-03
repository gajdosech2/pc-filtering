/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <glm/glm.hpp>

namespace cogs
{
  /*!
    \brief Parameters of a camera used to capture Scan.

    More information about intrinsic camera parameters can be found on Wikipedia
    https://en.wikipedia.org/wiki/Camera_resectioning and distortion coefficients
    at https://docs.opencv.org/2.4/doc/tutorials/calib3d/camera_calibration/camera_calibration.html
  */
  struct ScanCameraParams
  {
    //! Intrinsic parameter - x focal length in terms of pixels.
    float fx;
    //! Intrinsic parameter - y focal length in terms of pixels.
    float fy;
    //! Intrinsic parameter - x coordinate of a principal point.
    float cx;
    //! Intrinsic parameter - y coordinate of a principal point.
    float cy;
    //! Distortion radial coefficient k1.
    float k1 = 0.0f;
    //! Distortion radial coefficient k2.
    float k2 = 0.0f;
    //! Distortion radial coefficient k3.
    float k3 = 0.0f;
    //! Distortion tangential coefficient p1.
    float p1 = 0.0f;
    //! Distortion tangential coefficient p2.
    float p2 = 0.0f;
    //! Transformation to view space of camera.
    glm::mat4 view;
  };
}
