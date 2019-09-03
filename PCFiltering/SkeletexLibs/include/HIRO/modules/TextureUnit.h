#pragma once
#include <string>
#include <GLW/Texture.h>
#include <Visualizer/BillboardObj.h>
#include <HIRO/DataUnit.h>



namespace hiro
{

  namespace modules
  {

    class HIRO_API TextureUnit : public DataUnit
    {
    public:
      TextureUnit(const std::string &name, const glw::PTexture2D &tex);
      virtual PDataInstance Instantiate() override;
      void SetTexture(const glw::PTexture2D &texture);
      vis::PBillboardObj GetObjectPtr() const;
    private:
      vis::PBillboardObj billboard_object;
    };

    using PTextureUnit = std::shared_ptr<TextureUnit>;

    HIRO_API PTextureUnit AddTexture(const std::string &name, const glw::PTexture2D &tex);

  }

}