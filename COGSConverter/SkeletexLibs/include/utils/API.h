#pragma once

#ifdef _WIN32
# ifdef UTILS_STATIC
#   define UTILS_API
# elif UTILS_EXPORT
#   define UTILS_API __declspec(dllexport)
# else
#   define UTILS_API __declspec(dllimport)
# endif
#else
# define UTILS_API __attribute__ ((visibility("default")))
#endif