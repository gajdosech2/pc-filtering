/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <HIRO/DataId.h>
#include <HIRO/GuiTypes.h>



namespace hiro
{

  //! Interface for automatic generating gui to side panel, for a single DataInstance.
  class HIRO_API GuiGenerator
  {
  public:
#ifdef HIRO_ENABLE_CEGUI
    GuiGenerator(const DataId &instance, CEGUI::Window *client);
#endif
    virtual ~GuiGenerator();

    //! Set up whether tidy up should be performed automatically, when anything changes in gui.
    void SetAutoTestConditionsEnabled(bool enabled);
    //! Hide the elements whose conditions are not met.
    void TestElementConditions();

    /*!
      Creates new text label at the bottom of panel.
      Properties of this element can be edited using returned object.
    */
    gui::Label *AddLabel(const std::string &text);

    /*!
      Creates new button at the bottom of panel.
      Properties of this element can be edited using returned object.
    */
    gui::Button *AddButton(const std::string &caption);

    /*!
      Creates new check box element at the bottom of panel.
      Properties of this element can be edited using returned object.
    */
    gui::Checkbox *AddCheckbox(const std::string &title);

    /*!
      Creates new drop selection list at the bottom of panel.
      Properties of this element can be edited using returned object.
    */
    gui::Droplist *AddDroplist(const std::string &title);

    /*!
      Creates new numeric edit box for integer values at the bottom of panel.
      Properties of this element can be edited using returned object.
    */
    gui::NumericInt *AddNumericInt(const std::string &title);

    /*!
      Creates new numeric edit box for float values at the bottom of panel.
      Properties of this element can be edited using returned object.
    */
    gui::NumericFloat *AddNumericFloat(const std::string &title);

    /*!
      Creates new slider for float values at the bottom of panel.
      Properties of this element can be edited using returned object.
    */
    gui::SlidingFloat *AddSlidingFloat(const std::string &title);

    /*!
      Creates new slider for integer values at the bottom of panel.
      Properties of this element can be edited using returned object.
    */
    gui::SlidingInt *AddSlidingInt(const std::string &title);

    /*!
      Creates new list of check box elements at the bottom of panel.
      Properties of this element can be edited using returned object.
    */
    gui::CheckboxList *AddCheckboxList();

    //! Adds new vertical separator at the bottom of panel.
    void AddSeparator();

    void PanelBegin(gui::Element::ConditionFunc condition_func = nullptr);
    void PanelEnd();

  private:
    struct Impl;
    std::unique_ptr<Impl> m;
#ifdef HIRO_ENABLE_CEGUI
    void OnValueChange(const hiro::gui::Element *el);
#endif
  };

}