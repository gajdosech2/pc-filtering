/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <functional>
#ifdef HIRO_ENABLE_CEGUI
#   include <CEGUI/CEGUI.h>
#endif
#include <HIRO/API.h>



namespace hiro
{
  /*
    \brief Base class for panels in gui system.
  */
  class HIRO_API GuiPanel
  {
  public:

    GuiPanel();

    virtual ~GuiPanel();

    //! Sets the height of panel to the new value in pixels.
    virtual void SetHeight(uint32_t pixel_height);

    //! Sets callback which is evoked when user requests panel to close.
    void SetCallbackClose(const std::function<void(void)> &callback);

#ifdef HIRO_ENABLE_CEGUI
    //! Places this to the children list of specified container.
    void Embed(CEGUI::VerticalLayoutContainer *parent_containter);
#endif
  protected:

    //! Creates custom client window element in the panel body.
    void CreateClientWindow(const std::string &layout_xml = "");

    //! Sets the title name in the header.
    void SetTitle(const std::string &title);

    //! Enables/disables focus button in the header. Initially disabled.
    void SetFocusButtonEnabled(bool enabled);

    //! Enables/disables close button in the header. Initially enabled.
    void SetCloseButtonEnabled(bool enabled);

#ifdef HIRO_ENABLE_CEGUI
    //! Returns pointer to the client window in the panel body.
    CEGUI::Window *GetClientWindow();

    //! Returns pointer to the frame window.
    CEGUI::Window *GetFrameWindow();

    //! Sets callback which is evoked when user requests panel to focus.
    void SetCallbackFocus(const CEGUI::Event::Subscriber &subscriber);
#endif

  private:
#ifdef HIRO_ENABLE_CEGUI
    CEGUI::Window *header_ = nullptr;
    CEGUI::Window *client_win_ = nullptr;
    CEGUI::Window *panel_ = nullptr;

    bool is_rolledup_ = false;
    float true_height_;

    CEGUI::VerticalLayoutContainer *container_ = nullptr;

    std::function<void(void)> remove_request_callback_;

    bool OnRollupClick(const CEGUI::EventArgs &e);
    bool OnRemoveClick(const CEGUI::EventArgs &e);
    bool OnClientSizeChange(const CEGUI::EventArgs &e);
#endif
  };

}