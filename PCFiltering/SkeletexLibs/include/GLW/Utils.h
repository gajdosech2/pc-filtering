/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <string>
#include <utility>

#include <GLW/API.h>

namespace glw
{

  //! Returns all errors thrown by OpenGL since last time this function was called.
  GLW_API std::string GetError();


  //! Holder for OpenGL version.
  struct GLW_API Version
  {
    //! Get OpenGL version actually active on system.
    Version();

    //major and minor are macros in glibc <sys/sysmacros.h>
    //The dependency on <sys/sysmacros.h> should be removed in future releases og glibc.
    #undef major
    #undef minor
    //! Get specified OpenGL version.
    Version(const uint8_t maj, const uint8_t min) : major(maj), minor(min) {};

    //! Returns version in string form.
    std::string ToString() const;

    const uint8_t major;
    const uint8_t minor;
  };

  GLW_API bool operator <(const Version &a, const Version &b);
  GLW_API bool operator >(const Version &a, const Version &b);
  GLW_API bool operator <=(const Version &a, const Version &b);
  GLW_API bool operator >=(const Version &a, const Version &b);
  GLW_API bool operator ==(const Version &a, const Version &b);
  GLW_API bool operator !=(const Version &a, const Version &b);

}
