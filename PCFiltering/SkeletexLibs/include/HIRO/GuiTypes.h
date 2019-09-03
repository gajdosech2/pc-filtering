/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <vector>
#include <memory>
#include <functional>
#ifdef HIRO_ENABLE_CEGUI
#ifdef __GNUC__
#undef True
#undef False
#undef None
#endif
#include <CEGUI/CEGUI.h>
#include "HIRO/widgets/Combobox.h"
#endif

#include "COGS/Color.h"
#include "HIRO/API.h"

namespace hiro_widgets { class ArrayPanel; }

namespace hiro
{

  enum class BoolFunc
  {
    equals_true,
    equals_false
  };

  namespace gui
  {

    //! Interface for default gui element.
    class HIRO_API Element
    {
    public:
      using ConditionFunc = std::function<bool(void)>;
      using CallbackFunc = std::function<void(const hiro::gui::Element *)>;

      //! Returns unique identifier of current element.
      uint32_t GetId() const;
      //! This gui element is available only if defined function returns true.
      void SetConditionFunc(ConditionFunc condition_func);
      //! This gui element is available only if bool value passes specified BoolFunc.
      void SetConditionBool(bool *condition_var, BoolFunc func = BoolFunc::equals_true);
      /*!
        Tests last condition set by any SetCondition function.
        If no conditions provided, returns true.
      */
      bool TestCondition() const;
      //! Subscribes a function, which will be called every time an Element changes state.
      void Subscribe(CallbackFunc callback_func);

#ifdef HIRO_ENABLE_CEGUI
      Element(CEGUI::Window *win);
#endif
    protected:
      virtual void NotifyChange() const;
    private:
      const uint32_t id_ = 0u;
      ConditionFunc condition_ = nullptr;
      std::vector<CallbackFunc> callbacks_;
    };



    //! Gui element which can be interpreted as variable of some type.
    template <typename T>
    class HIRO_API LinkedElement : public Element
    {
    public:
      //! Change current value and notify subscribers if it differs from previous value.
      virtual void Set(const T new_value) = 0;
      //! Get current value.
      virtual T Get() const = 0;
      //! Set pointer to a variable, which will automatically receive current value when updated.
      //! Current value is set to value stored in linked variable.
      void SetLink(T *pointer)
      {
        link_ptr_ = pointer;
        if (link_ptr_ != nullptr)
        {
          Set(*link_ptr_);
        }
      }
#ifdef HIRO_ENABLE_CEGUI
      LinkedElement(CEGUI::Window *win) : Element(win) {};
#endif
    protected:
      void NotifyChange() const override
      {
        if (link_ptr_ != nullptr)
        {
          *link_ptr_ = Get();
        }
        Element::NotifyChange();
      };
    private:
      T *link_ptr_ = nullptr;
    };



    //! Pushable button element which can be subscribed to receive click events.
    class HIRO_API Button : public Element
    {
    public:
      //! Set text which is shown on button.
      void SetCaption(const std::string &caption);
      //! Returns text which is shown on button.
      std::string GetCaption() const;
#ifdef HIRO_ENABLE_CEGUI
      Button(CEGUI::PushButton *cegui_btn);
    private:
      CEGUI::PushButton *cegui_win;
      bool OnPressed(const CEGUI::EventArgs &e);
#endif
    };



    //! Non-editable element with assigned text.
    class HIRO_API Label : public Element
    {
    public:
      void Set(const std::string &text);
      std::string Get() const;
#ifdef HIRO_ENABLE_CEGUI
      Label(CEGUI::Window *cegui_checkbox);
    private:
      CEGUI::Window *cegui_win;
#endif
    };



    //! Selectable check box which can be linked to boolean variable.
    class HIRO_API Checkbox : public LinkedElement<bool>
    {
    public:
      void Set(bool state) override;
      bool Get() const override;
#ifdef HIRO_ENABLE_CEGUI
      Checkbox(CEGUI::ToggleButton *cegui_checkbox);
    private:
      CEGUI::ToggleButton *cegui_win;
      bool OnStateChange(const CEGUI::EventArgs &e);
#endif
    };



    /*!
      \brief Edit box which can contain only numeric integer values.

      Set min max values to define range. Default range is <-32768,32767>.
    */
    class HIRO_API NumericInt : public LinkedElement<int32_t>
    {
    public:
      //! Set value to numeric. Values outside of min max range are clipped.
      void Set(int32_t value) override;
      //! Returns current value of numeric.
      int32_t Get() const override;
      void SetMin(int32_t min_val);
      void SetMax(int32_t max_val);
      void SetMinMax(int32_t min, int32_t max);
      //! Set step by which the numeric changes when clicked on up down buttons.
      void SetStep(int32_t val_step);
      int32_t GetMin() const;
      int32_t GetMax() const;
      //! Returns step by which the numeric changes when clicked on up down buttons.
      int32_t GetStep() const;
#ifdef HIRO_ENABLE_CEGUI
      NumericInt(CEGUI::Spinner *cegui_spinner);
    private:
      CEGUI::Spinner *spinner_win_;
      bool OnValueChange(const CEGUI::EventArgs &e);
#endif
    };



    /*!
      \brief  Edit box which can contain only numeric float values.

      Set min max values to define range. Default range is <-32768,32767>.
    */
    class HIRO_API NumericFloat: public LinkedElement<float>
    {
    public:
      //! Set value to numeric. Values outside of min max range are clipped.
      void Set(float value) override;
      //! Returns current value of numeric.
      float Get() const override;
      void SetMin(float min_val);
      void SetMax(float max_val);
      void SetMinMax(float min, float max);
      //! Set step by which the numeric changes when clicked on up down buttons.
      void SetStep(float val_step);
      float GetMin() const;
      float GetMax() const;
      //! Returns step by which the numeric changes when clicked on up down buttons.
      float GetStep() const;
#ifdef HIRO_ENABLE_CEGUI
      NumericFloat(CEGUI::Spinner *cegui_spinner);
    private:
      CEGUI::Spinner *spinner_win_;
      bool OnValueChange(const CEGUI::EventArgs &e);
#endif
    };



    /*!
      \brief  Slider which can be directly represented by float variable.

      Set min max values to define range. Default range is <0,1>.
    */
    class HIRO_API SlidingFloat : public LinkedElement<float>
    {
    public:
      //! Set value to slider handle. Values outside of min max range are clipped.
      void Set(float value) override;
      //! Returns current value defined by slider handle.
      float Get() const override;
      void SetMin(float min_val);
      void SetMax(float max_val);
      void SetMinMax(float min, float max);
      //! Set step by which the slider moves when clicked out of handle.
      void SetStep(float val_step);
      float GetMin() const;
      float GetMax() const;
      //! Returns step by which the slider moves when clicked out of handle.
      float GetStep() const;
#ifdef HIRO_ENABLE_CEGUI
      SlidingFloat(CEGUI::Slider *cegui_slider);
    private:
      float min_value_ = 0.0f;
      CEGUI::Slider *cegui_win_;
      bool OnValueChange(const CEGUI::EventArgs &e);
#endif
    };



    /*!
      \brief  Slider which can be directly represented by integer variable.

      Set min max values to define range. Default range is <0,10>.
    */
    class HIRO_API SlidingInt : public LinkedElement<int32_t>
    {
    public:
      //! Set value to slider handle. Values outside of min max range are clipped.
      void Set(int32_t value) override;
      //! Returns current value defined by slider handle.
      int32_t Get() const override;
      void SetMin(int32_t min_val);
      void SetMax(int32_t max_val);
      void SetMinMax(int32_t min, int32_t max);
      //! Set step by which the slider moves when clicked out of handle. Default is 1.
      void SetStep(int32_t val_step);
      int32_t GetMin() const;
      int32_t GetMax() const;
      //! Returns step by which the slider moves when clicked out of handle.
      int32_t GetStep() const;
#ifdef HIRO_ENABLE_CEGUI
      SlidingInt(CEGUI::Slider *cegui_slider);
    private:
      int32_t min_value_ = 0;
      int32_t max_value_ = 10;
      CEGUI::Slider *cegui_win_;
      bool OnValueChange(const CEGUI::EventArgs &e);
      bool OnThumbReleased(const CEGUI::EventArgs &e);
      bool OnThumbChanged(const CEGUI::EventArgs &e);
#endif
      float GetAsFloat() const;
    };



    /*!
      \brief List of items where only one item can be selected at a time.

      Each item have defined value which identifies it.
    */
    class HIRO_API Droplist : public LinkedElement<int32_t>
    {
    public:
      //! Selects first item which has defined value.
      void Set(const int32_t value) override;
      //! Returns value of selected item.
      int32_t Get() const override;
      //! Adds a single item to the list.
      void AddItem(const std::string &name, int32_t value);
      //! Adds a single item to the list. Position in the list determines value of item.
      void AddItem(const std::string &name);
      //! Adds multiple items by pair <name,value>.
      void AddItems(const std::vector<std::pair<std::string, int32_t>> &items);
      //! Adds multiple items by name. Position in the list determines value of item.
      void AddItemsIndexed(const std::vector<std::string> &items);
#ifdef HIRO_ENABLE_CEGUI
      Droplist(hiro_widgets::Combobox *cegui_listbox);
    private:
      hiro_widgets::Combobox *cegui_win_;
      bool OnSelectionChange(const CEGUI::EventArgs &e);
      void AddItemWithoutInitialization(const std::string &name, int32_t value);
#endif
    };



    /*!
      \brief List of check box elements which can be managed as a group.
    */
    class HIRO_API CheckboxList : public Element
    {
    public:
      using ItemCallbackFunc = std::function<void(uint32_t, bool)>;

      //! Add check box item to the list, with defined title and initial state value. Optionally set its id, otherwise next available id is computed from the number of elements already in the list.
      size_t AddItem(const std::string &title, bool state = false, const std::optional<size_t> id = std::nullopt);
      //! Assigns custom identification color to item.
      void SetItemColor(size_t item, const cogs::Color3f &color);
      //! Subscribe callback function notified every time an item changes state.
      void SubscribeItems(ItemCallbackFunc func);
#ifdef HIRO_ENABLE_CEGUI
      CheckboxList(hiro_widgets::ArrayPanel *array_panel);
    private:
      struct Item
      {
        CEGUI::Window *wrapper;
        CEGUI::ToggleButton *checkbox;
        CEGUI::Window *colorstripe;
      };
      std::vector<Item> items_;
      std::vector<ItemCallbackFunc> item_callbacks_;
      hiro_widgets::ArrayPanel *array_panel_;
      bool OnItemToggle(const CEGUI::EventArgs &e);
      bool OnSelectAll(const CEGUI::EventArgs &e);
      bool OnUnselectAll(const CEGUI::EventArgs &e);
      void CreateNewItem(Item *item, size_t id);
#endif
    };


  }

}
