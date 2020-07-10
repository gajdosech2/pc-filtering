#pragma once

#ifdef _WIN32
# ifdef COGS_STATIC
#   define COGS_API
# elif COGS_EXPORT
#   define COGS_API __declspec(dllexport)
# else
#   define COGS_API __declspec(dllimport)
# endif
#else
# define COGS_API __attribute__ ((visibility("default")))
#endif

//! Namespace of "Computer Graphic Structures" library, which implement many core classes and structures.
namespace cogs {}