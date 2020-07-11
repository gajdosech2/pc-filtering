/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/


#include <vector>
#include <set>
#include <memory>
#include <limits>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <utils/GeometryStructures.h>
#include <utils/Couple.h>

#include <GEOM/API.h>

#undef min
#undef max

namespace geom
{
  GEOM_API Aabb3 GetAabb(const ConicalCapsule &conical_capsule);

  GEOM_API std::pair<glm::vec2, glm::vec2> GetBoundBox2D(std::vector<float> &vertices);
  GEOM_API std::pair<glm::vec2, glm::vec2> GetBoundBox2D(const std::vector<glm::vec2> &vertices);
  GEOM_API std::pair<glm::vec3, glm::vec3> GetBoundBox3D(std::vector<float> &vertices);

  template <typename T>
  utils::Couple<glm::tvec2<T>> GetAabbCouple(const std::vector<glm::tvec2<T>> &vertices)
  {
    utils::Couple<glm::tvec2<T>> bound(
        glm::tvec2<T>((std::numeric_limits<T>::max)()),
        glm::tvec2<T>(-(std::numeric_limits<T>::lowest)())
      );
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
      bound.a = glm::min(vertices[i], bound.a);
      bound.b = glm::max(vertices[i], bound.b);
    }
    return bound;
  }

  //std::pair<glm::vec3, glm::vec3> GetBoundBox3D(std::vector<float>& vertices);

  template <typename T>
  utils::Couple<glm::tvec3<T>> GetBoundBox3D(const std::vector<glm::tvec3<T>> &vertices)
  {
    utils::Couple<glm::tvec3<T>> bound(
        glm::tvec3<T>((std::numeric_limits<T>::max)()),
        glm::tvec3<T>(-(std::numeric_limits<T>::lowest)())
      );
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
      bound.a = glm::min(vertices[i], bound.a);
      bound.b = glm::max(vertices[i], bound.b);
    }
    return bound;
  }

  GEOM_API Line3 ToLine(const LineSegment3 &segment);

  GEOM_API Plane ToPlane(const HalfPlane &halfplane);

  GEOM_API Plane ToPlane(const Triangle3 &triangle);

}
