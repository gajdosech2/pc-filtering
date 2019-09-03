#pragma once
#include <optional>

#include <GLW/Texture.h>
#include <Visualizer/Object.h>
#include <Visualizer/TextRenderer.h>

namespace vis
{

  struct VIS_API GraphStyle : public ObjectStyle
  {
    cogs::Color3f text_color = cogs::color::GRAY60;
    cogs::Color3f line_color = cogs::color::GOLD;
    int32_t line_width = 2;
    int32_t point_size = 5;
    std::optional<float> min_x;
    std::optional<float> max_x;
    std::optional<float> min_y;
    std::optional<float> max_y;
    glm::ivec4 target_area = glm::ivec4(50, 50, 250, 250);
    std::string caption = "";
  };

  class VIS_API GraphObject : public Object
  {
  public:
    GraphObject(std::vector<float> values_y);
    GraphObject(std::vector<float> values_x, std::vector<float> values_y);
    ~GraphObject() = default;
    bool IsCompatibileWithStyle(const ObjectStyle *style) override;
  protected:
    void Render2D(const std::string &program) override;
    void FillTextRenderer(vis::TextRenderer *t_renderer) override;
    virtual void LoadRequiredShaderPrograms(glw::ProgramList *programs) override;
  private:
    std::shared_ptr<glw::ArrayObject> function_buffer_;
    std::shared_ptr<glw::ArrayObject> grid_buffer_vertical_;
    std::shared_ptr<glw::ArrayObject> grid_buffer_horizontal_;
    std::vector<glm::vec2> vertical_line_pos_;
    std::vector<glm::vec2> horizontal_line_pos_;
    std::vector<float> vertical_steps_;
    std::vector<float> horizontal_steps_;
    glm::ivec4 graph_area_;
    glm::vec4 edges_;
    uint8_t x_num_ = 1;
    uint8_t y_num_ = 1;

    void SetCustomEdges(const vis::GraphStyle *style);
    void FillGridBuffer();
    void CalculateGridPoints();
  };

  using PGraphObject = std::shared_ptr<GraphObject>;
  using PGraphStyle = std::shared_ptr<GraphStyle>;
}

