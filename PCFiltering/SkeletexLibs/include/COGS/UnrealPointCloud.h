/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <glm/glm.hpp>
#include "API.h"


namespace cogs
{
  /*!
    \brief Adapter for cogs::PointCloud structure ensuring compatibility with UnrealEngine.
  */
  class COGS_API UnrealPointCloud
  {
  public:

    //! Creates a cloud with no points.
    UnrealPointCloud();
    //! Destroys cloud and frees all property data.
    virtual ~UnrealPointCloud();

    //! Make a deep copy of the source UnrealPointCloud.
    UnrealPointCloud(const UnrealPointCloud &source);
    //! Make a deep copy of the source UnrealPointCloud.
    UnrealPointCloud &operator=(const UnrealPointCloud &source);
    //! Move-construct UnrealPointCloud.
    UnrealPointCloud(UnrealPointCloud &&) noexcept = default;
    //! Move-construct UnrealPointCloud.
    UnrealPointCloud &operator=(UnrealPointCloud &&) noexcept = default;

    //! Imports point cloud from a file.
    virtual bool Import(const char *filename);
    //! Exports point clouds to a file.
    virtual bool Export(const char *filename) const;

    //! Resizes point count to the new value without initialization of data of newly created points.
    virtual void Resize(uint32_t new_size);
    //! Returns number of points currently stored in the cloud.
    virtual uint32_t GetSize() const;
    //! Changes capacity of cloud. This method is not adding any points.
    virtual void Reserve(uint32_t new_capacity);
    //! Returns how much points can be added with currently reserved memory.
    uint32_t GetCapacity() const;
    //! Frees spare memory so that new capacity exactly matches size.
    virtual void ShrinkToFit();
    //! Removes all points from cloud.
    virtual void Clear();
    /*!
      \brief
        Removes points with specific ids from cloud.
      \param ids_to_erase
        Array of indices which should be erased.
      \param
        Number of indices provided in ids_to_erase array.
      \warning
        Method may change order of points.
    */
    virtual void Erase(const uint32_t *ids_to_erase, size_t count);
    /*!
      \brief
        Increments size of this by the size of pc and copies property data available in both.
      \note
        Properties not available in this will not be copied.
        To add also properties not currently available in this,
        use UnrealPointCloud::ClonePropertiesOf before calling this function.
    */
    virtual void Append(const UnrealPointCloud &pc);

    /*!
      \brief
        Returns pointer to position data.
      \note
        Equivalent to PointCloud::GetData<glm::vec3>(PointCloud::POSITIONS);
    */
    glm::vec3 *GetPositions();
    /*!
      \brief
        Returns pointer to position data.
      \note
        Equivalent to PointCloud::GetData<glm::vec3>(PointCloud::POSITIONS);
    */
    const glm::vec3 *GetPositions() const;

    /*!
      \brief
        Checks whether normal property exists.
      \note
        Equivalent to PointCloud::HasProperty(PointCloud::NORMALS);
    */
    bool HasNormals() const;
    /*!
      \brief
        When normal property exists, returns the pointer to data, nullptr otherwise.
      \note
        Equivalent to PointCloud::GetData<glm::vec3>(PointCloud::NORMALS);
    */
    glm::vec3 *GetNormals();
    /*!
      \brief
        When normal property exists, returns the pointer to data, nullptr otherwise.
      \note
        Equivalent to PointCloud::GetData<glm::vec3>(PointCloud::NORMALS);
    */
    const glm::vec3 *GetNormals() const;
    /*!
      \brief
        Creates PointCloud::NORMALS property buffer if it was is not added already.
      \note
        Do not add this property using PointCloud:AddProperty method.
    */
    void AddNormals();

    //! Transforms point positions and normals
    virtual void Transform(const glm::mat4 &transform);

  private:
    struct Impl;
    Impl *m;
  };

}

