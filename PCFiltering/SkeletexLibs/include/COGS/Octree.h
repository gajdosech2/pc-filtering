/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <COGS/PointCloud.h>

namespace cogs
{
  /*!
    \struct OctreeNode
    \brief
      Used internally to store the nodes of the octree.
  */
  struct OctreeNode;

  /*!
    \class Octre
    \brief
      Space-partitioning data structure for point clouds.
  */
  class COGS_API Octree
  {
  public:
    //! \property Node of the octree does not further subdivide, if it contains number of points below this threshold.
    static const uint8_t POINTS_THRESHOLD = 64;
    //! \brief Octree destructor.
    ~Octree();
    //! \brief Default constructor.
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
    void Include(const std::vector<const cogs::PointCloud *> clouds);
    /*!
      \brief
        Get the string representation of this octree.
      \return
        String representation of this octree.
    */
    std::string ToString() const;
    /*!
      \brief
        Get points indices of the octree node at the given location.
      \details
        Find the leaf node of the octree which contains the given location and get indices of all the points contained in this leaf node.
      \param location
        Location in the world that will be searched.
      \return
        Vector of points indices in the node. Empty if the location is outside of this octree.
    */
    std::vector <uint32_t> FindPoints(const glm::vec3 &location) const;
    /*!
      \brief
        Get indices of points in the radius from the given location.
      \param location
        Location in the world that will be searched.
      \param radius_sqr
        Squared radius of the search area.
      \note
        Method internally raises the radius_sqr to the power of 2 for performance reasons. Therefore remember to pass the radius squared!
      \return
        Vector of points indices in the area.
    */
    std::vector <uint32_t> GetPointsInRadius(const glm::vec3 &location, float_t radius_sqr) const;
    /*!
      \brief
        Whether any of the points is included in the input AABB.
      \param search_range
        Area of this axis aligned box will be searched for points.
      \return
        True if any of included points is included in the input AABB, false otherwise.
    */
    bool IsPointInRange(const geom::AABB3 &search_range) const;
    /*!
      \brief
        Fills vector of points that are included in the input AABB.
      \param search_range
        Area of this axis aligned box will be searched for points.
      \param output
        Used as an output parameter to fill with points.
    */
    void GetPointsInRange(const geom::AABB3 &search_range, std::vector<uint32_t> *output) const;

  private:
    //! \private \property Reference to the root of this octree.
    OctreeNode *root_ = nullptr;
    //! \private Initializes the root and constructs the tree.
    void InitializeRoot(const std::vector<glm::vec3> &positions);
    //! \private Find the maximum coordinate of the given positions.
    float_t MaximumExtent(const std::vector<glm::vec3> &positions) const;
    //! \private Find the minimum coordinate of the given positions.
    float_t MinimumExtent(const std::vector<glm::vec3> &positions) const;
  };
}