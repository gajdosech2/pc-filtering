/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <GLW/Texture.h>

#include <Visualizer/Object.h>



namespace vis
{

  struct VIS_API BillboardStyle : public ObjectStyle
  {
    bool use_nearest_filtering = false;
    virtual bool LoadState(std::istream &str) override;
    virtual void SaveState(std::ostream &str) override;
  };


  class VIS_API BillboardObj : public Object
  {
  public:
    BillboardObj(const std::string &texture_filename);
    BillboardObj(const glw::PTexture2D &texture);
    BillboardObj(const BillboardObj &) = delete;
    virtual ~BillboardObj() = default;
    bool IsCompatibileWithStyle(const ObjectStyle *style) override;

    void SetTexture(const glw::PTexture2D &texture);
    glw::PTexture2D GetTexture();

    void Render(const std::string &program) override;
    void LoadRequiredShaderPrograms(glw::ProgramList *programs) override;

  private:
    glw::PTexture2D texture_;
  };


  using PBillboardStyle = std::shared_ptr<BillboardStyle>;
  using PBillboardObj = std::shared_ptr<BillboardObj>;

}


