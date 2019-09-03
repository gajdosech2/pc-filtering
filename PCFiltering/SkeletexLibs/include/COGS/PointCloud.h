/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <vector>
#include <set>
#include <unordered_map>
#include <optional>
#include <variant>

#include <glm/glm.hpp>
#include <utils/GeometryStructures.h>

#include <COGS/DataType.h>
#include <COGS/PointCloudProperty.h>


namespace cogs
{
  /*!
    \brief
      Management class for a set of data points in a 3D space.

    Class stores point data separated in data buffers. Every data record is called a property,
    and by default, only a single property exists - PointCloud::POSITIONS.
    User can specify custom properties which will be assigned to all points in the cloud.
  */
  class COGS_API PointCloud
  {
  public:
    //! Common identifier for point positions.
    static const cogs::PointCloudProperty::Key POSITIONS;
    //! Common identifier for point normals.
    static const cogs::PointCloudProperty::Key NORMALS;
    //! Common identifier for point colors.
    static const cogs::PointCloudProperty::Key COLORS;
    //! Common identifier for point texture coordinates.
    static const cogs::PointCloudProperty::Key UVS;
    //! Common identifier for point intensities.
    static const cogs::PointCloudProperty::Key INTENSITIES;

    //! Creates a cloud with no points.
    PointCloud();
    //! Destroys cloud and frees all property data.
    virtual ~PointCloud();

    //! Make a deep copy of the source PointCloud.
    PointCloud(const PointCloud &source);
    //! Make a deep copy of the source PointCloud.
    PointCloud &operator=(const PointCloud &source);
    //! Move-construct PointCloud.
    PointCloud(PointCloud &&) noexcept = default;
    //! Move-construct PointCloud.
    PointCloud &operator=(PointCloud &&) noexcept = default;

    //! Imports point cloud from a file.
    virtual bool Import(const std::string &filename);
    //! Exports point clouds to a file.
    virtual bool Export(const std::string &filename) const;

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
    void Clear();
    /*!
      \brief
        Removes points with specific ids from cloud.
        Sorts and validates input ids for further use in subclasses.
      \note
        Method may change ordering of points.
        Memory for erased points is not released, you should call ShrinkToFit afterwards,
        to free excess memory.
    */
    virtual void Erase(std::vector<uint32_t> ids_to_erase);
    /*!
      \brief
        Increments size of this by the size of pc and copies property data available in both.
      \note
        Properties not available in this will not be copied.
        To add also properties not currently available in this,
        use PointCloud::ClonePropertiesOf before calling this function.
    */
    virtual void Append(const PointCloud &pc);

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
        Creates PointCloud::NORMALS property buffer if it was not added already.
      \note
        Do not add this property using PointCloud:AddProperty method.
    */
    const PointCloudProperty &AddNormals();

    /*!
      \brief
        Checks whether color property exists.
      \note
        Equivalent to PointCloud::HasProperty(PointCloud::COLORS);
    */
    bool HasColors() const;
    /*!
      \brief
        When color property exists, returns the pointer to data, nullptr otherwise.
      \note
        Equivalent to PointCloud::GetData<COGS::Color3f>(PointCloud::COLORS);
    */
    struct Color3f *GetColors();
    /*!
      \brief
      When color property exists, returns the pointer to data, nullptr otherwise.
      \note
      Equivalent to PointCloud::GetData<COGS::Color3f>(PointCloud::COLORS);
    */
    const struct Color3f *GetColors() const;
    /*!
      \brief
        Creates PointCloud::COLORS property buffer if it was not added already.
      \note
        Do not add this property using PointCloud:AddProperty method.
    */
    const PointCloudProperty &AddColors ();

    /*!
      \brief
        Checks whether intensity property exists.
      \note
        Equivalent to PointCloud::HasProperty(PointCloud::INTENSITIES);
    */
    bool HasIntensities() const;
    /*!
      \brief
        When intensity property exists, returns the pointer to data, nullptr otherwise.
      \note
        Equivalent to PointCloud::GetData<float>(PointCloud::INTENSITIES);
    */
    float *GetIntensities();
    /*!
      \brief
        When intensity property exists, returns the pointer to data, nullptr otherwise.
      \note
        Equivalent to PointCloud::GetData<float>(PointCloud::INTENSITIES);
    */
    const float *GetIntensities() const;
    /*!
      \brief
        Creates PointCloud::INTENSITIES property buffer if it was not added already.
      \note
        Do not add this property using PointCloud:AddProperty method.
    */
    const PointCloudProperty &AddIntensities();

    //! Returns all available properties.
    const std::vector<PointCloudProperty> &GetProperties() const;

    /*!
      \brief
        Creates new property buffer.
      \param key
         Property identifier. If it already exists, verifies it is of the same type. otherwise throws.
      \param type
        Specific type of data in buffer.
      \throws
        std::runtime_error if the property key already exists and contains a different property then specified.
      \return
        Newly created property.
    */
    const PointCloudProperty &AddProperty(const PointCloudProperty::Key &key, DataType type);
    /*!
      \brief
        Creates new property buffer.
      \param key
         Property identifier. If it already exists, verifies it is of the same type. otherwise throws.
      \param bytes_per_point
        Number of bytes reserved for each point.
      \throws
        std::runtime_error if the property key already exists and contains a different property then specified.
      \return
        Newly created property.
    */
    const PointCloudProperty &AddProperty(const PointCloudProperty::Key &key, size_t bytes_per_point);
    //! Remove a single property by key..
    void RemoveProperty(const PointCloudProperty::Key &key);
    //! Adds all properties, which are not already present in this.
    void ClonePropertiesOf(const PointCloud &pc);
    //! Checks whether a specific property exists.
    bool HasProperty(const PointCloudProperty::Key &key) const;
    //! Returns property with specified key if it exists, std::nullopt otherwise.
    std::optional<PointCloudProperty> GetProperty(const PointCloudProperty::Key &key);
    //! Returns property with specified key if it exists, std::nullopt otherwise.
    std::optional<const PointCloudProperty> GetProperty(const PointCloudProperty::Key &key) const;

    //! When property exists, returns the pointer to data, nullptr otherwise.
    virtual void *GetVoidData(const PointCloudProperty::Key &key);
    //! When property exists, returns the pointer to data, nullptr otherwise.
    const void *GetVoidData(const PointCloudProperty::Key &key) const;

    //! When property exists, returns the pointer to data of a template type, nullptr otherwise.
    template <typename T> T *GetData(const PointCloudProperty::Key &key);
    //! When property exists, returns the pointer to data of a template type, nullptr otherwise.
    template <typename T> const T *GetData(const PointCloudProperty::Key &key) const;

    //! Transforms point positions and normals
    virtual void Transform(const glm::mat4 &transform);

  protected:

    //! Make a deep copy of the source PointCloud.
    void MakeCloneOf(const PointCloud &source);
    //! Manages memory during EraseProperty method
    static void MoveChunkToGap(const size_t chunk_size, const size_t bytes_per_point, const size_t gap_size, char *data);

  private:

    uint32_t capacity_;
    uint32_t size_;
    std::vector<PointCloudProperty> properties_;
    std::unordered_map<PointCloudProperty::Key, size_t> property_map_;

    //! Erases values from single property.
    void ErasePropertyValues(const std::vector<uint32_t> &sorted_ids_to_erase, PointCloudProperty &prop_to_erase);
    //! Creates new property record without initialization. In the case of duplicate property, throws std::runtime_error.
    PointCloudProperty &AddProperty(const PointCloudProperty::Key &key, const std::variant<DataType, size_t> type_or_size);
  };



  template <typename T>
  const T *cogs::PointCloud::GetData(const PointCloudProperty::Key &key) const
  {
    return reinterpret_cast<const T *>(GetVoidData(key));
  }

  template <typename T>
  T *cogs::PointCloud::GetData(const PointCloudProperty::Key &key)
  {
    return reinterpret_cast<T *>(GetVoidData(key));
  }

  COGS_API geom::AABB3 GetPointCloudAABB(const PointCloud *cloud);
  COGS_API void ErasePointsOutsideAABB(const geom::AABB3 &aabb, PointCloud *cloud);

}

