/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <string>
#include <COGS/DataType.h>

namespace cogs
{
  //! Type used to store information about property.
  struct Property
  {
    //! Type used to identify cloud property record.
    using PropertyKey = std::string;
    //! Unique property identifier.
    PropertyKey key;
    //! Number of bytes for each point.
    size_t bytes_per_point{ 0 };
    //! Data type of property. Optional.
    DataType type{ DataType::UNKNOWN };
    //! Pointer to property data buffer.
    void *data{ nullptr };
    //! Compares if this data type is equivalent to other. Data and key are not tested.
    bool HasEquivalentType(const Property &other) const;
  };
}
