/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <Utils/GeometryStructures.h>
#include <HIRO_DRAW/Renderer.h>



namespace hiro::draw
{

  //! Stylization of the rendered line field.
  struct HIRO_DRAW_API LineFieldStyle : public Style
  {
    //! Possible sources of line color information.
    enum class ColorSource
    {
      fixed = 0,  //!< Line colors are uniform and set by default_cl parameter.
      length = 1, //!< Color is set by relative length of line.
      custom = 2, //!< Line colors are set using custom color buffer.
    };
    //! Specifies thickness of a lines.
    float thickness{ 1.0f };
    //! Specifies, from where line colors should be obtained.
    ColorSource color_source{ ColorSource::fixed };
    //! Default fixed color for the lines. Used when color source set to ColorSource::fixed.
    cogs::Color3f default_cl { cogs::color::GRAY50 };
    //! Name of buffer used to get custom colors. Used when color source set to ColorSource::custom.
    glw::ArrayObject::BufferName custom_cl_buf{ glw::ArrayObject::INVALID_NAME };
    //! Imports state from stream.
    virtual bool ReadFromStream(std::istream &str) override;
    //! Exports state to stream.
    virtual void WriteToStream(std::ostream &str) override;
  };



  //! Renderer used for rendering array of lines.
  class HIRO_DRAW_API LineFieldRenderer : public Renderer
  {
  public:
    //! Creates a renderer with an empty line array.
    LineFieldRenderer() = default;
    //! Creates a renderer with the specified line array.
    LineFieldRenderer(const std::vector<geom::LineSegment3> &line_array);

    LineFieldRenderer(const LineFieldRenderer &source) = delete;
    LineFieldRenderer &operator=(const LineFieldRenderer &source) = delete;
    LineFieldRenderer(LineFieldRenderer &&) noexcept = delete;
    LineFieldRenderer &operator=(LineFieldRenderer &&) noexcept = delete;
    virtual ~LineFieldRenderer() = default;

    //! Replace the currently used line array with a new one.
    void SetLines(const std::vector<geom::LineSegment3> &line_array);
    //! Returns number of lines in currently set line array.
    size_t GetLineCount() const;
    /*!
      \brief Creates a buffer with given colors.
      \returns The name of the created buffer. If creation failed -1.
    */
    glw::ArrayObject::BufferName AddColors(std::vector<cogs::Color3f> data);

    //! Test whether specified style is compatible with the object.
    bool IsCompatibileWithStyle(const Style *style) override;

  protected:
    //! Specifies whet happens when simple rendering is done.
    void OnRenderSimple() override;

  private:
    //! Current number of lines.
    size_t line_count_{ 0u };
    //! Vertex buffer containing line array.
    glw::PArrayObject line_buffer_;
    //! Buffer that holds colors by length. Used when color source set to ColorSource::length.
    glw::ArrayObject::BufferName color_by_length_buf { glw::ArrayObject::INVALID_NAME };
  };



  //! Shared pointer to an object of the type LineFieldStyle.
  using PLineFieldStyle = std::shared_ptr<LineFieldStyle>;
  //! Shared pointer to an object of the type LineFieldRenderer.
  using PLineFieldRenderer = std::shared_ptr<LineFieldRenderer>;

}
