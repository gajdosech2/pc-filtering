/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <utils/GeometryStructures.h>

#include <Visualizer/Object.h>



namespace vis
{

  struct VIS_API LineFieldStyle : public ObjectStyle
  {
    enum class ColorSource
    {
      fixed = 0,
      length = 1,
      custom = 2,
    };
    uint32_t custom_cl_buf = 0u;
    cogs::Color3f default_cl = cogs::color::GRAY50;
    ColorSource color_source;
    float thickness = 1.0f;
    virtual bool LoadState(std::istream &str) override;
    virtual void SaveState(std::ostream &str) override;
  };


  class VIS_API LineFieldObj : public Object
  {
  public:
    LineFieldObj() = default;
    LineFieldObj(const std::vector<geom::LineSegment3> &line_array);
    LineFieldObj(const LineFieldObj &) = delete;
    virtual ~LineFieldObj() = default;
    bool IsCompatibileWithStyle(const ObjectStyle *style) override;

    void SetLines(const std::vector<geom::LineSegment3> &line_array);

    //! Creates a buffer with given colors, returns id of the created buffer. If creation failed, returns -1.
    int32_t AddColors(std::vector<cogs::Color3f> data);
    size_t GetLineCount() const;

  protected:
    void LoadRequiredShaderPrograms(glw::ProgramList *programs) override;
    void Render(const std::string &program) override;

  private:
    size_t line_count_ = 0u;
    glw::PArrayObject line_buffer_;
    size_t color_by_length_buf = 0u;
  };
  using PLineFieldStyle = std::shared_ptr<LineFieldStyle>;
  using PLineFieldObj = std::shared_ptr<LineFieldObj>;

}


