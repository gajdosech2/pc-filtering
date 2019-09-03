/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <string>

#include <glm/glm.hpp>

#include <GLW/ShaderProgramList.h>
#include <COGS/Color.h>

#include <Visualizer/Scene.h>



namespace vis
{

  //! Defined alignment origin of the text.
  enum class TextAlignment : uint8_t
  {
    top_left,
    top_center,
    top_right,
    center_left,
    center,
    center_right,
    bottom_left,
    bottom_center,
    bottom_right
  };


  /*!
    \brief Responsible for rendering of a text.

    TextRenderer has an active state which includes font, color, alignment etc.
    and all Print calls use current active state for the rendering.

    Instance of this object stores all print calls and state changes,
    and renders texts automatically at the same time.
  */
  class VIS_API TextRenderer
  {
    friend class Renderer;
  public:
    TextRenderer();
    TextRenderer(const TextRenderer &) = delete;
    ~TextRenderer();
    //! Load TTF font file from disk and returns unique identifier of the font.
    size_t LoadFont(const std::string &font_file, float font_height);
    //! Sets an active font by its identifier.
    void SetFont(const size_t font_id);
    //! Sets active color for the text.
    void SetColor(const cogs::Color4b &color);
    //! Sets active text alignment for the text.
    void SetAlignment(TextAlignment alignment);
    //! Prints specified text with the alignment origin at the specified position.
    void Print(glm::ivec2 pos, const std::string &text);
    //! Sets active rendering context for the text. Not to be called by a user.
    void SetSceneContext(const vis::PScene &scene);

  private:
    struct Impl;
    std::unique_ptr<Impl> m;

    glm::ivec2 AlignCoords(const std::string &text, glm::ivec2 coords) const;
    void LoadShaderPrograms(glw::ProgramList *program_list);
    void Render(glw::ProgramList *program_list, const glm::uvec2 &resolution);
  };

}
