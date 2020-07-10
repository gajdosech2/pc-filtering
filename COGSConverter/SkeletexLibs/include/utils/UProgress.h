/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <functional>
#include <Utils/API.h>

namespace uprogress
{
  //! Reset percentage value and enable/disable it.
  UTILS_API void Reset(bool percentage_enabled = true);

  //! Check whether percentage is enabled at this time.
  UTILS_API bool IsEnabled();

  //! Set amount of percentage to defined value. Clamped to range <0,1>.
  UTILS_API void Set(float percentage_value);

  //! Returns current percentage value of progress.
  UTILS_API float Get();

  //! Increment progress value by an amount of percentage. Clamped to range <0,1>.
  UTILS_API void Add(float percentage_increment);

  //! Subscribes a function that will be called whenever a percentage has changed.
  UTILS_API void Subscribe(std::function<void()> subscriber);
}
