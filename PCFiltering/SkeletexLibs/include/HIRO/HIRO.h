/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
/*!
  \file Main file providing basic functionality of HIRO library.
*/

#ifdef _WIN32
#include <Windows.h>
#endif

#include <memory>
#include <HIRO/DataUnit.h>



namespace hiro
{

  //! Defines the nature of how a unit was instanced.
  enum class ActiveUnitNature
  {
    automatic,  //! Unit was created automatically.
    selected    //! Unit was selected by user via gui.
  };

  //! Information about a currently active unit.
  struct ActiveUnitInfo
  {
    //! Active data unit.
    std::shared_ptr<hiro::DataUnit> unit;
    //! Index of view where the unit is active.
    uint32_t viewarea;
    //! Defines how the unit was instanced.
    ActiveUnitNature nature;
  };



  /*!
    Sets up directory where hiro stores temporal cache data.
    Needs to be called before initialization with hiro::Initialize().
  */
  HIRO_API void SetIntermediateDirectory(const std::string &dir);

  /*!
    Sets up directory where are sub-folders "resources" and "shaders" present.
    Needs to be called before initialization with hiro::Initialize().
  */
  HIRO_API void SetResourceDirectory(const std::string &dir);

  /*!
    Initializes the library with custom look visuals of application on startup.
    Not not available in educational version of HIRO library.
  */
  [[maybe_unused]] HIRO_API void Initialize(const std::string &custom_look);

  //! Initializes the library.
  HIRO_API void Initialize();

  //! Check whether hiro window has not been closed by user.
  HIRO_API bool IsOpen();

  //! Updates and redraws viewing window is required.
  HIRO_API void Update();

  //! Add data unit to the list of units.
  HIRO_API void AddUnit(const PDataUnit &data_unit);

  //! Remove all data units from the list of units.
  HIRO_API void RemoveAllDataUnits();

  //! Remove all data units whose name includes prefix.
  HIRO_API void RemoveUnitsWithPrefix(std::string prefix);

  //! Capture render canvas of specified area and save to specified file.
  HIRO_API void Screenshot(uint32_t viewarea_id, const std::string &file, const glm::uvec2 &resolution);

  //! Returns information about all units currently active in context.
  HIRO_API std::vector<ActiveUnitInfo> GetActiveUnits();

#ifdef _WIN32
  /*!
    Returns WIN API pointer to owned window.
    Not not available in educational version of HIRO library.
  */
  [[maybe_unused]] HIRO_API HWND GetWindowHandle();
#endif
}
