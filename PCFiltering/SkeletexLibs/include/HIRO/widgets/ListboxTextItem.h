#pragma once
#include <CEGUI/widgets/ListboxTextItem.h>
#include "HIRO/API.h"

namespace hiro_widgets
{
  //!
  class HIRO_API ListboxTextItem : public CEGUI::ListboxTextItem
  {
  public:
    static CEGUI::String WidgetTypeName;
    ListboxTextItem(const std::string &text, uint32_t item_id = 0);
    //! Sets padding on top and bottom of the item.
    void SetVertPadding(float padding);
    //! Returns padding on top and bottom of the item.
    float GetVertPadding();

    CEGUI::Sizef getPixelSize(void) const override;
    void draw(CEGUI::GeometryBuffer &buffer, const CEGUI::Rectf &targetRect,
      float alpha, const CEGUI::Rectf *clipper) const override;
  protected:
    float padding_ = 0.0f;
  };

}