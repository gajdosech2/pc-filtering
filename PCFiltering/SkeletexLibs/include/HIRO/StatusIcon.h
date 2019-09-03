#pragma once
#include <CEGUI/Window.h>
#include "API.h"

namespace hiro_widgets
{

  class HIRO_API StatusIcon : public CEGUI::Window
  {
  public:

    enum class Status
    {
      none,
      waiting,
      success,
      failure
    };

    static CEGUI::String WidgetTypeName;

    StatusIcon(const CEGUI::String &type, const CEGUI::String &name);
    void SetStatus(StatusIcon::Status status);
    StatusIcon::Status GetStatus() const;
    virtual void update(float elapsed) override;
  private:
    Status status_ = Status::none;
    float time_since_last_render_ = 0.0f;
    void RotateSelf(float elapsed);
  };

}