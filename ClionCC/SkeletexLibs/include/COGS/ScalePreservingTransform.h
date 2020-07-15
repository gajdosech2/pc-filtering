/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <COGS/API.h>



namespace cogs
{

  /*!
    \brief
    Class used for effective management of transformation matrix
    by handling translation, rotation.

    Final transformation is computed in order: rotation > translation.
  */
  struct COGS_API ScalePreservingTransform
  {
    ScalePreservingTransform() {}
    ~ScalePreservingTransform() = default;

    glm::vec3 translation;
    glm::quat rotation;
  };
}
