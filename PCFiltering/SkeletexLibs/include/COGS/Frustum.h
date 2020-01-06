#pragma once

#include <glm/glm.hpp>

#include <Utils/GeometryStructures.h>
#include <Utils/Exceptions.h>

#include <COGS/API.h>



namespace cogs
{

  /*!
    \brief
      Definition of a 3-dimensional view frustum - a square-based 3D pyramid truncated by parallel planes.
  */
  struct COGS_API Frustum
  {
    //! Frustum position.
    glm::vec3 position;
    //! Frustum forward vector. Points inside the Frustum from its position.
    glm::vec3 forward;
    //! Frustum up vector.
    glm::vec3 up;

    //! Distance of near plane from the Frustum's position in the direction of its forward vector.
    float near_distance;
    //! Distance of far plane from the Frustum's position in the direction of its forward vector.
    float far_distance;

    //! Full horizontal field of view angle.
    float horizontal_fov;
    //! Full vertical field of view angle.
    float vertical_fov;



    //! Compute right vector from forward and up vectors.
    glm::vec3 Right() const;

    //! Compute near plane. The normal vector points inside the Frustum.
    geom::Plane GetNearPlane() const;

    //! Compute near plane. The normal vector points inside the Frustum.
    geom::Plane GetFarPlane() const;

    //! Compute top plane. The normal vector points inside the Frustum.
    geom::Plane GetTopPlane() const;

    //! Compute bottom plane. The normal vector points inside the Frustum.
    geom::Plane GetBottomPlane() const;

    //! Compute right plane. The normal vector points inside the Frustum.
    geom::Plane GetRightPlane() const;

    //! Compute left plane. The normal vector points inside the Frustum.
    geom::Plane GetLeftPlane() const;

    //! Compute horizontal plane. The normal vector is the up vector.
    geom::Plane GetHorizontalPlane() const;

    //! Compute vertical plane. The normal vector is the right vector.
    geom::Plane GetVerticalPlane() const;
  };
}
