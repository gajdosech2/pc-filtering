/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once

namespace utils
{
  //! An object that has an id (ideally a strong id).
  template <typename IdType>
  class Identifiable
  {
  public:
    void SetId(IdType new_id)
    {
      id_ = new_id;
    }
    IdType GetId() const
    {
      return id_;
    }

  protected:
    IdType id_;
  };
}
