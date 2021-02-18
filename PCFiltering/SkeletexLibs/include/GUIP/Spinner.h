#pragma once
#include <CEGUI/widgets/Spinner.h>
#include <GUIP/API.h>

namespace guip
{
  //! Widget that allows user to edit a single line text value.
  class GUIP_API Spinner : public CEGUI::Spinner
  {
  public:
    //! Unique gui system identifier.
    static CEGUI::String WidgetTypeName;

    Spinner(const CEGUI::String &type, const CEGUI::String &name);

    void initialiseComponents(void) override;

  private:
    bool handleEditTextAccept(const CEGUI::EventArgs &e);
  };
}