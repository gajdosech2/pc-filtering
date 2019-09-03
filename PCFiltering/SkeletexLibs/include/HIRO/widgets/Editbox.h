#pragma once
#include <CEGUI/widgets/Editbox.h>
#include "HIRO/API.h"

namespace hiro_widgets
{
  //! Interface extension class for CEGUI::Combobox. Any constructed CEGUI::Combobox is of this type.
  class HIRO_API Editbox : public CEGUI::Editbox
  {
  public:
    static CEGUI::String WidgetTypeName;

    Editbox(const CEGUI::String &type, const CEGUI::String &name);

  private:
    void onKeyDown(CEGUI::KeyEventArgs &) override;
  };

}