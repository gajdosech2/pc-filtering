#pragma once
#include <iostream>
#include <HIRO/Exportable.h>

namespace hiro
{

  //!
  class FileStorage : public Exportable
  {
  public:

    /*!
      \brief Loads object state from file immediately.

      The file name that is loaded is specified by method GetFileName.
      The method ReadFromStream will be called only if file version match GetFileVersion.
    */
    bool Load();
    //! Notifies the object that is should be saved. See SaveIfRequested method.
    void RequestSave();
    //!
    /*!
      \brief Saves the file if it was requested before using RequestSave.

      The file that is loaded is specified by method GetFileName.
      The method WriteToStream will be called.
    */
    void SaveIfRequested();

  protected:
    //! Should return a unique file name for this object.
    virtual std::string GetFileName() const = 0;
    //! Should return a file version for this object.
    virtual uint32_t GetFileVersion() const = 0;

    //! Returns fill file name with directory.
    std::string GetFullFileName() const;
    //! Override this method to specify custom data to be loaded.
    bool ReadFromStream(std::istream &str) override;
    //! Override this method to specify custom data to be stored.
    void WriteToStream(std::ostream &str) const override;

  private:
    bool was_save_requested_ {false};
  };

}