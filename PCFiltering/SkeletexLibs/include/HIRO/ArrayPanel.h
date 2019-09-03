#pragma once
#include <CEGUI/Window.h>
#include "API.h"

namespace hiro_widgets
{

  class HIRO_API ArrayPanel : public CEGUI::Window
  {
  public:
    static CEGUI::String WidgetTypeName;

    ArrayPanel(const CEGUI::String &type, const CEGUI::String &name);

    void SetElementSpacing(double spacing);
    double GetElementSpacing() const;

    void SetPadding(double padding);
    double GetPadding() const;

  protected:
    virtual void addChild_impl(CEGUI::Element *element) override;

  private:
    float element_spacing_ = 0.0f;
    float padding_ = 0.0f;
    void ReLayout();
    bool ReLayoutCallback(const CEGUI::EventArgs &e);
  };

}