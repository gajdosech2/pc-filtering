/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once

#include <Utils/GeometryStructures.h>

#include <GEOM/API.h>

namespace geom
{
  // inflate each side of rectangle by specified margin
  template <typename T>
  void InflateRectangle(geom::TRect<T> &rect, const T margin)
  {
    rect.x -= margin;
    rect.y -= margin;
    rect.width += 2 * margin;
    rect.height += 2 * margin;
  }

  // deflate each side of rectangle by specified margin
  template <typename T>
  void DeflateRectangle(geom::TRect<T> &rect, const T margin)
  {
    rect.x += margin;
    rect.y += margin;
    rect.width -= 2 * margin;
    rect.height -= 2 * margin;
  }


  /*
      Returns parameter telling interpolation parameter of point projection on line.
      Resulting parameter of value 0 tells the projection lays on segment line point1, and value 1 tells the projection lays on segment line point2.
      Anything between 0 and 1 tells the point projection lays directly on line segment */
  GEOM_API float GetProjectionParameter(const glm::vec3 &point, const geom::LineSegment3 &line_segment);

  /*
      Returns parameter telling interpolation parameter of point projection on line.
      Resulting parameter of value 0 tells the projection lays on segment line point1, and value 1 tells the projection lays on segment line point2.
      Anything between 0 and 1 tells the point projection lays directly on line segment */
  GEOM_API float GetProjectionParameterClamped(const glm::vec3 &point, const geom::LineSegment3 &line_segment);

  //! Calculates the oriented distance of point to a plane. The value is positive if the point lies in the half-space pointed to by the plane normal.
  GEOM_API float GetOrientedDistance(const glm::vec3 &point, const Plane &plane);

  // calculates a distance of point to a line
  GEOM_API float GetDistance(const glm::vec3 &point, const geom::LineSegment3 &line);

  //! Calculate the distance of a point from plane.
  GEOM_API float GetDistance(const glm::vec3 &point, const Plane &plane);

  GEOM_API float GetDistance(const glm::vec3 &point, const geom::Line3 &line);

  // distance of point to the capsule surface
  // returns negative distance when point is inside capsule, zero when it is on surface and positive value otherwise
  GEOM_API float GetDistance(const glm::vec3 &point, const ConicalCapsule &conical_capsuile);

  GEOM_API glm::vec3 GetProjection(const glm::vec3 &point, const ConicalCapsule &capsule);

  GEOM_API glm::vec3 GetProjection(const glm::vec3 &point, const LineSegment3 &line_segment);

  GEOM_API glm::vec3 GetProjection(const glm::vec3 &point, const Line3 &line);

  // return whether a primitive contains specified point
  GEOM_API bool Contains(const Aabb3 &aabb, const glm::vec3 &point);

  //float LineLineClosestsPoints(const geom::LineSegment3 &line1, const geom::LineSegment3 &line2, glm::vec3 *point1, glm::vec3 *point2);

  GEOM_API bool GetIntersection(const Line2 &line1, const Line2 &line2, glm::vec2 *out_intersection_point);
  GEOM_API bool GetIntersection(const geom::Line3 &line, const geom::Plane &plane, glm::vec3 *out_intersection_point);
  GEOM_API bool GetIntersection(const geom::LineSegment3 &linesegment, const geom::Plane &plane, glm::vec3 *out_intersection_point);
  GEOM_API bool GetIntersection(const geom::Line3 &line, const geom::HalfPlane &half_plane, glm::vec3 *out_intersection_point);
  GEOM_API bool GetIntersection(const geom::LineSegment3 &linesegment, const geom::HalfPlane &half_plane, glm::vec3 *out_intersection_point);
  GEOM_API bool GetIntersection(const geom::Triangle3 &triangle, const geom::HalfPlane &halfplane, geom::LineSegment3 *out_intersection = nullptr);
  GEOM_API bool GetIntersection(const geom::Line3 &line, const geom::Aabb3 &aabb);

  GEOM_API glm::vec3 Barycentric(const glm::vec3 &p, const Triangle3 &triangle);

  struct GEOM_API CollisionResult
  {
  public:
    bool is_collision;
    glm::vec3 point1;
    glm::vec3 point2;
    float param1;
    float param2;
    float depth;
  };

  GEOM_API CollisionResult GetCollision(const geom::Capsule &capsule_1, const geom::Capsule &capsule_2);

  GEOM_API bool IsTriangleFacingToPoint(const glm::vec3 &point, const geom::Triangle3 &triangle);
}