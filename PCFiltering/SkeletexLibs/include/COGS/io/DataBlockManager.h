#pragma once
#include <memory>
#include <unordered_map>

#include <COGS/API.h>
#include <COGS/io/DataBlock.h>



namespace cogs
{
  namespace io
  {
    /*!
      \brief Container class for specific data block factory objects.

      Used for registering new data block factories and creation of data blocks.

      By default class contains predefined factories for data blocks,
      but can be extended to support custom, user defined ones.
    */
    class COGS_API DataBlockManager
    {
    public:
      //! Identifier of predefined point cloud data block class.
      static const DataBlockType POINTCLOUD_DTB_TYPE;
      //! Identifier of predefined scan data block class.
      static const DataBlockType SCAN_DTB_TYPE;
      //! Identifier of predefined mesh data block class.
      static const DataBlockType MESH_DTB_TYPE;
      //! Identifier of predefined triangulation data block class.
      static const DataBlockType TRIANGULATION_DTB_TYPE;

      //! Creates manager with predefined data block factories.
      DataBlockManager();
      ~DataBlockManager() = default;
      DataBlockManager(const DataBlockManager &) = delete;
      DataBlockManager &operator=(const DataBlockManager &) = delete;
      DataBlockManager(DataBlockManager &&) = delete;
      DataBlockManager &operator=(DataBlockManager &&) & = delete;

      //! Checks if current instance has a factory for defined datablock_type already.
      bool HasFactoryFor(const DataBlockType &datablock_type);

      //! Creates new instance of data block using corresponding factory.
      std::unique_ptr<DataBlock> Create(const DataBlockType &datablock_type);

      /*!
        \brief Registers new factory type T for the defined datablock_type.
        \throws std::invalid_argument If factory for the type exists already.
      */
      template <typename T>
      void AddFactoryFor(const DataBlockType &datablock_type);
    private:
      std::unordered_map<DataBlockType, std::unique_ptr<DataBlockFactory>> factories_;

    };

    template <typename T>
    void cogs::io::DataBlockManager::AddFactoryFor(const DataBlockType &datablock_type)
    {
      if (HasFactoryFor(datablock_type))
      {
        throw std::invalid_argument("Cannot create DataBlockFactory for type '"
          + datablock_type + "'. Another factory for that type already exists.");
      }
      factories_[datablock_type] = std::make_unique<T>();
    }

  }
}

