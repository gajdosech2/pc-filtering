/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <vector>

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

#include <COGS/Color.h>

#include <Visualizer/API.h>


namespace vis
{

  /*!
    \brief Definition of scene light (GPU compatible).
  */
  struct VIS_API Light
  {
    //! Lighting position or direction (defined by homogeneous coordinate).
    glm::vec4 position;
    //! Generated color influence of light.
    cogs::Color3f color;
    //! Ambient color influence of light.
    float ambient_intensity;
    //! Whether is light position defined in camera space.
    float in_camera_space;
    glm::vec3 padding;

    static Light CreateLight(
      const glm::vec4 &position,
      const cogs::Color3f &color,
      float ambient_intensity,
      bool in_camera_space);

    static Light CreatePointLight(
      const glm::vec3 &position,
      const cogs::Color3f &color,
      float ambient_intensity);

    static Light CreateDirectionalLight(
      const glm::vec3 &direction,
      const cogs::Color3f &color,
      float ambient_intensity);

    //! A point light whose position is at the position of the camera, oriented in direction of view.
    static Light CreateHeadLight(const cogs::Color3f &color);
  };

}