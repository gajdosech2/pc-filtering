/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once

#include <string>



namespace utils
{
  //! A simple parent class to guarantee that all who derive from it will have a human-readable name.
  template<typename T>
  class NamedClass
  {
  public:
    [[nodiscard]]
    virtual const char *GetClassName() const
    {
      return T::CLASS_NAME;
    }
  };

}
