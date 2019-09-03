#pragma once

#ifdef _WIN32
# ifdef VIS_STATIC
#   define VIS_API
# elif VIS_EXPORT
#   define VIS_API __declspec(dllexport)
# else
#   define VIS_API __declspec(dllimport)
# endif
#else //_WIN32
# define VIS_API __attribute__ ((visibility("default")))
#endif //_WIN32


//! Namespace of visualization library. Implements higher level of OpenGL rendering.
namespace vis {}