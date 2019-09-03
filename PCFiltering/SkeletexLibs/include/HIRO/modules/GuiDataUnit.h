#pragma once
#include <CEGUI/CEGUI.h>
#include <utils/ExtCEGUI.h>
#include <HIRO/DataUnit.h>



namespace hiro
{

  namespace modules
  {

    class HIRO_API GuiDataUnit : public DataUnit
    {
    public:
      GuiDataUnit(const std::string &name);
      PDataInstance Instantiate() override;
      void Setup(const std::string &layout_xml);
      CEGUI::Window *GetWindow(const std::string &name);
      CEGUI::Window *GetRootWindow();
      CEGUI::Window *GetMainRootWindow();
      template <typename T>
      T *GetWindow(const std::string &name)
      {
        return static_cast<T *>(GetWindow(name));
      }
    protected:
      CEGUI::Window *window_root_ = nullptr;
      GuiDataUnit(const hiro::DataId &id);
    };

  }

}