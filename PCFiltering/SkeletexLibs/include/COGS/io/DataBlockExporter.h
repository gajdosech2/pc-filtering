/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <vector>
#include <memory>

#include <COGS/io/DataBlockManager.h>



namespace cogs // forward declarations
{
  class PointCloud;
  class Scan;
  struct Mesh;
}



namespace cogs
{
  namespace io
  {
    /*!
      \brief Enables writing or cogs structures into files.

      Objects and data blocks for export are stored in the export list,
      and have to be valid until file is saved. Each object can be represented by one
      or multiple data blocks.

      By default, exporter supports only predefined data blocks, but can be altered
      to support also custom data blocks, when user-defined DataBlockManager
      is provided during class construction.

      For more information about cogs file io system, see page \ref cogs-file-format.
    */
    class COGS_API DataBlockExporter
    {
    public:

      //! Current exporter version.
      static uint8_t VERSION;

      //! Creates exporter with default DataBlockManager object.
      DataBlockExporter();

      //! Creates exporter with custom DataBlockManager object.
      DataBlockExporter(const std::shared_ptr<DataBlockManager> &datablock_manager);

      //! Exports currently added data to a file.
      bool SaveToFile(const std::string &filename);

      /*!
        \brief Adds the object to the export list.
        \param object_ref Reference to object. Must be valid until saved.
        \return Index of data block for added object type.
      */
      DataBlockIndex AddPointCloud(const cogs::PointCloud *object_ref);
      /*!
        \brief Adds the object to the export list.
        \param object_ref Reference to object. Must be valid until saved.
        \return Index of data block for added object type.
      */
      DataBlockIndex AddScan(const cogs::Scan *object_ref);
      /*!
        \brief Adds the object to the export list.
        \param object_ref Reference to object. Must be valid until saved.
        \return Index of data block for added object type.
      */
      DataBlockIndex AddMesh(const cogs::Mesh *object_ref);
      /*!
        \brief Creates and adds custom data block to the export list.
        \param type Type identifier of data block to create.
        \return Pointer to data block and its file index.
      */
      std::pair<DataBlock *, DataBlockIndex> AddCustom(DataBlockType type);
      /*!
        \brief Creates and adds custom data block to the export list.
        \param type Type identifier of data block to create.
        \return Pointer to data block, static casted to the template type T, and its file index.
      */
      template <typename T>
      std::pair<T *, DataBlockIndex> AddCustom(DataBlockType type)
      {
        auto val = AddCustom(type);
        return std::make_pair(
            static_cast<T *>(val.first),
            val.second
          );
      }

    private:
      std::shared_ptr<DataBlockManager> datablock_manager_;
      std::unordered_map<DataBlockType, DataBlockIndex> index_counter_;
      std::vector<std::shared_ptr<DataBlock>> datablocks_;
      void ExportHeader(std::ostream &out_s);
    };

  }
}
