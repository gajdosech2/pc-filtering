/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once

#include <string>
#include <Utils/StrongType.h>



namespace utils
{
  /*!
    \brief
      Template class implementation of a Named Strong Id (a Strong Type with an underlying unsigned integer type (size_t)).
    \note
      https://www.fluentcpp.com/2016/12/08/strong-types-for-strong-interfaces/
      Strong Id is an integral type (size_t) specific for the given usage (usually as an id of a class).
      It prevents implicit conversions to and from other types, as well as comparisons to other types.

    \tparam PhantomType An unused type specific for each Strong Type to ensure the Strong Type is different from every other type.
  */
  template <typename PhantomType>
  struct StrongId : public StrongType<size_t, PhantomType>
  {
    using StrongType<size_t, PhantomType>::StrongType;

    //! An id following this one. Incremented by 1.
    inline StrongId Next() const
    {
      return StrongId(this->template As<size_t>() + 1);
    }

    //! Convert the id to string.
    inline std::string ToString() const
    {
      return std::to_string(this->template As<size_t>());
    }
  };
}
