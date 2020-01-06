#pragma once
#include <CEGUI/widgets/Editbox.h>
#include <GUIP/API.h>

namespace guip
{
  //! Widget that allows user to edit a single line text value.
  class GUIP_API Editbox : public CEGUI::Editbox
  {
  public:
    //! Unique gui system identifier.
    static CEGUI::String WidgetTypeName;

    Editbox(const CEGUI::String &type, const CEGUI::String &name);

  private:
    void onKeyDown(CEGUI::KeyEventArgs &) override;
  };

}