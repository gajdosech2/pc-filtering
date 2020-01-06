/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <string>

#include <GLW/Texture.h>

#include <HIRO_DRAW/renderers/BillboardRenderer.h>
#include <HIRO/Resource.h>



namespace hiro::modules
{

  class HIRO_API TextureResource : public Resource
  {
  public:
    TextureResource(const std::string &name, const glw::PTexture2D &tex);
    virtual PGadget CreateGadget() override;
    void SetTexture(const glw::PTexture2D &texture);
    hiro::draw::PBillboardRenderer GetObjectPtr() const;
  private:
    hiro::draw::PBillboardRenderer billboard_object;
  };

  using PTextureResource = std::shared_ptr<TextureResource>;

  HIRO_API PTextureResource AddTexture(const std::string &name, const glw::PTexture2D &tex);

}
