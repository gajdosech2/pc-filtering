#pragma once
#include <fstream>
#include <memory>

namespace cogs
{
  //! Definitions for cogs file format.
  namespace io
  {
    //! Defines position index of the data block in a file.
    using DataBlockIndex = size_t;

    /*!
      \brief
        Identifies data block instances.
        Each class derived from cogs::io::DataBlockType should have unique type identifier.
    */
    using DataBlockType = std::string;

    //! Definition of required interface for derived data blocks.
    class DataBlock
    {
    public:
      //! Returns unique identifier of class. Usually class name.
      virtual DataBlockType GetType() const = 0;

      //! Version of export function.
      virtual uint8_t GetExportVersion() const = 0;

      //! Checks if specified version is supported by import function.
      virtual bool IsImportVersionSupported(uint8_t version) const = 0;

      //! Executes import form specified stream.
      virtual void Import(std::istream &in_s) = 0;

      //! Executes export to specified stream.
      virtual void Export(std::ostream &out_s) = 0;
    };

    //! Factory for data block type.
    struct DataBlockFactory
    {
    public:
      //! Creates new instance of data block type.
      virtual std::unique_ptr<DataBlock> Create() const = 0;
    };

  }
}