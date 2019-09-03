/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <HIRO/DataInstance.h>



namespace hiro
{

  /*!
      \brief Data storage for each module. It instantiates DataInstance objects in order to visualize stored data.

      Each custom module implementation should override DataUnit, specifying type of DataInstance to instantiate.
      Custom DataUnit is then created inside shared pointer by user application, and registered using hiro::AddUnit function.
  */
  class HIRO_API DataUnit
  {
  public:
    const DataId kId;
    DataUnit(const std::string &name, const std::string &category);
    DataUnit(const DataId &dataid);
    //! Override this method to create custom DataInstace of your DataUnit.
    virtual PDataInstance Instantiate() = 0;
    //! Forces all existing DataInstance objects of this DataUnit to be destroyed and created again.
    void ResetInstances();
  };

  using PDataUnit = std::shared_ptr<DataUnit>;

}