#pragma once
#include <HIRO/Version.h>

#ifdef _WIN32
# ifdef HIRO_EXPORT
#   define HIRO_API __declspec(dllexport)
# else
#   define HIRO_API __declspec(dllimport)
# endif
#else
# define HIRO_API __attribute__ ((visibility("default")))
#endif

//! Global HIRO engine namespace.
namespace hiro
{
  //! Default data modules included with HIRO build.
  namespace modules {}

  //! Control structures for customization of automatically generated gui.
  namespace gui {}

}