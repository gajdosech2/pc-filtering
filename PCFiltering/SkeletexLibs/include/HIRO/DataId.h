/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <string>
#include <HIRO/API.h>



namespace  hiro
{
  //! Identifier structure of data object.
  struct HIRO_API DataId
  {
    //! Denotes invalid identifier.
    static DataId INVALID;

    std::string name { "" };
    std::string category { "" };
    DataId() = default;
    DataId(const std::string &name, const std::string &categ);
    DataId(const std::string &string_id);
    //! Converts identifier to string by merging name and category separated by a colon.
    std::string ToString() const;
    //! Checks if this is a valid data identifier.
    bool IsValid() const;
    //! Makes this identifier invalid.
    void Invalidate();
    //! Returns name stripped of directory hierarchy.
    std::string GetNameWithoutPath() const;
    //! Returns only directory hierarchy of a name.
    std::string GetNamePath() const;
    //! Compares if two identifiers are equivalent.
    bool operator==(const DataId &other) const;
    //! Compares if two identifiers are different.
    bool operator!=(const DataId &other) const;
    //! Compares ordering of two identifiers.
    bool operator<(const DataId &other) const;
  };

}