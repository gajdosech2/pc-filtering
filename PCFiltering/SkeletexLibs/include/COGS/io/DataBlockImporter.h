/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <unordered_map>
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
      \brief Enables reading of cogs structures into files.

      When file is open, only header is fetched, containing information
      about data available in file and it counts. When user gets specific object,
      it is imported directly from file. When file is closed, no data can be obtained.

      By default, exporter supports only predefined data blocks, but can be altered
      to support also custom data blocks, when user-defined DataBlockManager
      is provided during class construction.

      For more information about cogs file io system, see page \ref cogs-file-format.
    */
    class COGS_API DataBlockImporter
    {
    public:

      //! Current importer version.
      static uint8_t VERSION;

      //! Creates importer with default DataBlockManager object.
      DataBlockImporter();

      //! Creates importer with custom DataBlockManager object.
      DataBlockImporter(const std::shared_ptr<DataBlockManager> &datablock_manager);

      //! Opens file for reading and fetches file header information.
      bool OpenFile(const std::string &filename);

      //! Closes currently opened file, disabling getters.
      void CloseFile();

      //! Checks if a file is currently open.
      bool IsFileOpen() const;

      //! Returns number of available all point cloud objects.
      size_t GetNumOfPointClouds() const;

      //! Loads object with specified index from file. Returns success.
      bool GetPointCloud(cogs::PointCloud *out_cloud, DataBlockIndex id = 0);

      //! Returns number of available point cloud objects with scan information.
      size_t GetNumOfScans() const;

      //! Loads object with specified index from file. Returns success.
      bool GetScan(cogs::Scan *out_scan, DataBlockIndex id = 0);

      //! Returns number of available mesh objects.
      size_t GetNumOfMeshes() const;

      //! Loads object with specified index from file. Returns success.
      bool GetMesh(cogs::Mesh *out_mesh, DataBlockIndex id = 0);

      //! Returns number of available data blocks of specified type.
      size_t GetNumOfCustoms(DataBlockType type) const;

      //! Returns data block of specified type and index. Returns null when no such block exists.
      DataBlock *GetCustom(DataBlockType type, DataBlockIndex id = 0);

      //! Returns data block of specified type and index. Returns null when no such block exists.
      template <typename T>
      T *GetCustom(DataBlockType type, DataBlockIndex id = 0)
      {
        return static_cast<T *>(GetCustom(type, id));
      }

    private:

      struct DataBlockInfo
      {
        //! Export version of data block in the file.
        uint8_t version;
        //! Position of data block in the file.
        std::streampos position;
        //! Pointer to data block object.
        std::shared_ptr<DataBlock> block;
      };

      std::shared_ptr<DataBlockManager> datablock_manager_;
      std::ifstream file_;
      std::unordered_map<DataBlockType, std::vector<DataBlockInfo>> navigation_;
      bool ImportHeader(std::istream &in_s);
      bool ImportNavigation(std::istream &in_s);
    };

  }
}
