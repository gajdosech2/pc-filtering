#pragma once
#include <HIRO/DataUnit.h>



namespace hiro
{

  namespace modules
  {

    class HIRO_API CameraAnimUnit : public DataUnit
    {
    public:
      CameraAnimUnit(const std::string &name);
      ~CameraAnimUnit();
      PDataInstance Instantiate() override;
    };

  }

}

