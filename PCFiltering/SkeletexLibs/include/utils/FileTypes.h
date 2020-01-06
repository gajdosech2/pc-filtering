/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once

#include <string>
#include <vector>
#include <map>

#include <Utils/ExtSTD.h>


//! Definitions and functions for file types supported by codebase.
namespace file_types
{
  /*!
    \brief
      Enum class that specifies (geometry) file types that we recognize.

      Recognized file formats:
        .praw       - Photoneo raw scan file          Photoneo
        .cogs       - Skeletex file format            Skeletex
        .ply        - "Polygon File Format"                         "https://en.wikipedia.org/wiki/PLY_(file_format)"
        .obj        -                                 Wavefront     "https://en.wikipedia.org/wiki/Wavefront_.obj_file"
        .dae        - "Digital Asset Exchange File"   Collada       "https://fileinfo.com/extension/dae"
        .stl        - "stereolithography"                           "https://en.wikipedia.org/wiki/STL_(file_format)"
        .fbx        - "Autodesk Filmbox"              Autodesk      "https://en.wikipedia.org/wiki/FBX"
        .bvh        - "Biovision Hierarchy"                         "https://en.wikipedia.org/wiki/Biovision_Hierarchy"
  */
  enum class Type : uint8_t
  {
    praw,
    cogs,
    ply,
    obj,
    dae,
    stl,
    fbx,
    bvh,
  };



  //! A map of all file types and their extensions.
  static const std::map<Type, std::string> file_extensions =
  {
    { Type::praw, "praw" },
    { Type::cogs, "cogs" },
    { Type::ply, "ply" },
    { Type::obj, "obj" },
    { Type::dae, "dae" },
    { Type::stl, "stl" },
    { Type::fbx, "fbx" },
    { Type::bvh, "bvh" },
  };

  //! All file types defined.
  static const std::vector<Type> all_types = std_ext::Transformed(
      file_extensions,
      [](const std::pair<Type, std::string> &pair)->Type { return pair.first; });

  /*!
    \brief    Recognized file types that can have a scan.
    \warning  Whether they do have a scan or not is not guaranteed!
  */
  static const std::vector<Type> supported_scan_types
  {
    Type::praw,
    Type::cogs,
    Type::ply,
  };

  /*!
    \brief    Recognized file types that can have a scan and that we can export.
  */
  static const std::vector<Type> exportable_scan_types
  {
    Type::cogs,
    Type::ply,
  };

  /*!
    \brief    Recognized file types that can have a mesh.
    \warning  Whether they do have a mesh or not is not guaranteed!
  */
  static const std::vector<Type> supported_mesh_types
  {
    Type::cogs,
    Type::fbx,
    Type::dae,
    Type::stl,
    Type::ply,
  };

  /*!
    \brief    Recognized file types that can have a mesh and that we can export.
  */
  static const std::vector<Type> exportable_mesh_types
  {
    Type::cogs,
    Type::stl,
    Type::ply,
  };

  /*!
    \brief    Cogs (internal) types.
  */
  static const std::vector<Type> cogs_types
  {
    Type::cogs,
  };


  //! Get extension from a file type.
  static inline const std::string Ext(const Type type)
  {
    if (!std_ext::Contains(type, file_extensions))
    {
      throw std::runtime_error("FileTypes: Specified file type has no extension defined.");
    }
    return file_extensions.at(type);
  }
}
