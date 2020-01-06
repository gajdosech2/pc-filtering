#pragma once
#include <CEGUI/widgets/Editbox.h>
#include <GUIP/API.h>

namespace guip
{

  //! Icon representing waiting for a process to finish.
  class WaitCircle : public CEGUI::Window
  {
  public:
    WaitCircle(const CEGUI::String &type, const CEGUI::String &name);

    //! Unique gui system identifier.
    static CEGUI::String WidgetTypeName;

    //! Updates widget. This function is called automatically by a gui system.
    virtual void update(float elapsed) override;
  private:
    float time_since_last_render_ = 0.0f;
  };

}