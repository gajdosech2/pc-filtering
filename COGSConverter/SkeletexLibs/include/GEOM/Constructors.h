/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#include <GEOM/API.h>
#include <Utils/GeometryStructures.h>

namespace geom
{
  GEOM_API Aabb3 GetAabb(const ConicalCapsule &conical_capsule);

  GEOM_API Line3 ToLine(const LineSegment3 &segment);

  GEOM_API Plane ToPlane(const HalfPlane &halfplane);

  GEOM_API Plane ToPlane(const Triangle3 &triangle);
}
