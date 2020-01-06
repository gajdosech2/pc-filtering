/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <COGS/PointCloud.h>

namespace cogs
{
  //! Used internally to store the nodes of the octree.
  struct OctreeNode;

  //! Space-partitioning data structure for point clouds.
  class COGS_API Octree
  {
  public:
    //! When an octree node contains this number of points or less, it does not subdivide further.
    static const uint8_t POINTS_THRESHOLD = 64;
    //! Octree destructor.
    ~Octree();
    //! Default constructor.
    Octree() = default;
    /*!
      \brief
        Constructs Octree over the given cloud.
      \param cloud
        Octree will be constructed over points of this cloud.
    */
    Octree(const cogs::PointCloud &cloud);
    /*!
      \brief
        Constructs Octree over the given vector of points.
      \param positions
        Octree will be constructed over these points.
    */
    Octree(const std::vector<glm::vec3> &positions);
    /*!
      \brief
        Adds points of the input cloud into the Octree.
      \param positions
        Points of this cloud will be included in the octree.
    */
    void Include(const cogs::PointCloud &cloud);
    /*!
      \brief
        Adds points of input clouds into the Octree.
      \param positions
        Points from all the clouds in this vector will be included in the octree.
    */
    void Include(const std::vector<const cogs::PointCloud *> &clouds);
    /*!
      \brief
        Get the string representation of this octree.
      \return
        String representation of this octree.
    */
    std::string ToString() const;
    /*!
      \brief
        Whether any of the points exist in the range from the given location.
      \param location
        Location in the world around which will be searched.
      \param radius
        Radius range of the search area.
      \return
        True if any point is in range, false otherwise.
    */
    bool HasPointIn(const glm::vec3 &location, float_t radius) const;
    /*!
      \brief
        Whether any of the points exist inside the input AABB.
      \param search_range
        Area of this axis aligned box will be searched for points.
      \return
        True if any point is included in the input AABB, false otherwise.
    */
    bool HasPointIn(const geom::Aabb3 &search_range) const;
    /*!
      \brief
        Get indices of points in the radius from the given location.
      \param location
        Location in the world around which will be searched.
      \param radius
        Radius range of the search area.
      \return
        Vector of point indices in the area.
    */
    std::vector<uint32_t> Find(const glm::vec3 &location, float_t radius) const;
    /*!
      \brief
        Get indices of points that are inside the input AABB.
      \param search_range
        Area of this axis aligned box will be searched for points.
      \param output
        Used as an output parameter to fill with points.
    */
    std::vector<uint32_t> Find(const geom::Aabb3 &search_range) const;

  private:
    //! Reference to the root of this octree.
    OctreeNode *root_ = nullptr;
    //! Initializes the root and constructs the tree.
    void InitializeRoot(const std::vector<glm::vec3> &positions);
    //! Find the maximum coordinate of the given positions.
    float_t MaximumExtent(const std::vector<glm::vec3> &positions) const;
    //! Find the minimum coordinate of the given positions.
    float_t MinimumExtent(const std::vector<glm::vec3> &positions) const;
  };
}