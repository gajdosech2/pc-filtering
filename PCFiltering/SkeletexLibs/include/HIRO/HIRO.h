/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <memory>
#include <HIRO/Resource.h>
#include <HIRO/Debugger.h>



namespace hiro
{

  /*!
    \brief Information about currently active Resource objects.

    Active Resource objects are the ones that were used to
    create at least one Gadget ant it still exists.
  */
  struct ActiveResourceInfo
  {
    //! Pointer to an active data resource.
    std::shared_ptr<hiro::Resource> resource;
    //! Index of view where the resource is active.
    uint32_t viewarea;
    //! Defines whether the resource is selected. Otherwise it is pinned.
    bool is_selected;
  };



  /*!
    \brief
      Sets directory where hiro stores temporal cache data.
    \warning
      Needs to be called before initialization using hiro::Initialize().
  */
  HIRO_API void SetIntermediateDirectory(const std::string &dir);

  /*!
    \brief
      Sets directory where are sub-folders "resources" and "shaders" present.
    \warning
      Needs to be called before initialization using hiro::Initialize().
  */
  HIRO_API void SetAssetDirectory(const std::string &dir);

  //! Initializes the library. From this moment the library can be fully used.
  HIRO_API void Initialize();

  /*!
    \brief
      Initializes the library with custom look visuals of application on startup.
    \note
      Not not available in educational version of HIRO library.
  */
  HIRO_API void Initialize(const std::string &custom_look);

  //! Terminates the library. Call to release all HIRO related data.
  HIRO_API void Terminate();

  //! Check whether hiro window has not been closed.
  HIRO_API bool IsOpen();

  /*!
    \brief
      Updates window and redraws if required.

    Ensure that this function is called regularly in your application.
    If not, it may cause window freezing and lag.
  */
  HIRO_API void Update();

  //! Add specified resource to the list of available resources.
  HIRO_API void AddResource(const PResource &resource);

  //! Remove all resources from the list of available resources.
  HIRO_API void RemoveAllResources();

  //! Remove all resources whose name includes prefix from the list of available resources
  void RemoveResourcesWithPrefix(const std::string &prefix);

  /*!
    \brief Returns information about all resources currently active in context.

    Active Resource objects are the ones that were used to
    create at least one Gadget ant it still exists.
  */
  HIRO_API std::vector<ActiveResourceInfo> GetActiveResources();

  /*!
    \brief Returns internal visual debugger.

    The visual debugger can be used for rendering primitive debug objects giving visual feedback.
    When the library is not initialized, this function returns nullptr.
  */
  HIRO_API std::shared_ptr<hiro::Debugger> Debug();

}
