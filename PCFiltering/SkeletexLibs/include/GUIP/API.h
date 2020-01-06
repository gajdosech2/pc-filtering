#pragma once

#ifdef _WIN32
# ifdef GUIP_EXPORT
#   define GUIP_API __declspec(dllexport)
# else
#   define GUIP_API __declspec(dllimport)
# endif
#else
# define GUIP_API __attribute__ ((visibility("default")))
#endif

//! Proxy library for working with graphical user interface.
namespace guip
{

}