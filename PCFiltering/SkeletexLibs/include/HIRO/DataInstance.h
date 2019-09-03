#pragma once
#include <Visualizer/Scene.h>
#include <Visualizer/TextRenderer.h>
#include <HIRO/Camera.h>
#include <HIRO/GuiGenerator.h>

namespace hiro { class DataUnit; class Viewarea; }



namespace hiro
{

  /*!
      \brief Visual part of module. Provides options for visualization, that do not interfere with data stored in DataUnit.

      To unlock its powers, derive your custom DataInstance and override functionality that you require.
  */
  class HIRO_API DataInstance
  {
  public:
    const DataId kDataId;
    DataInstance(const hiro::DataUnit *unit);
    virtual ~DataInstance();

    //! Function called right after object creation to set up all properties.
    virtual void Initialize();

    //! Function used to specify gui elements. Called whenever gui should be generated.
    virtual void GenerateGui(hiro::GuiGenerator &gui);

    //! Function called each time user requests to focus camera at current instance.
    virtual bool FocusCamera(glm::vec3 *focus_point, glm::vec3 *cam_position) const;

    //! Function called each time HIRO context is updated.
    virtual void Update(const float time_delta);

    //! Function called when user presses specific key for this instance. When function handles key action, it should return true.
    virtual bool KeyPressed(int key_code, int mods);

    //! Function called during rendering, to specify what texts should be rendered.
    virtual void RenderTexts(const glm_ext::TransMat4 &projection, const glm_ext::TransMat4 &view, glm::vec2 viewport_res, vis::TextRenderer *t_renderer);

    //! Returns pointer to DataUnit which was used to instantiate current object.
    template <typename UnitType = DataUnit>
    const UnitType * GetUnit() const { return static_cast<const UnitType *>(dataunit_); }

    //! Return handle to the camera used to render current instance.
    hiro::Camera *GetCamera() const;

    /*!
      \brief Insert specified renderer to the list of currently used renderers.

      \param object Defines a renderer to add.
      \param style Defines a rendering style to be used.
      \param layer Specifies a layer on which the renderer should appear.

      \return Identifier of currently added renderer.
    */
    int32_t AddRenderer(const vis::PObject &object, const vis::PObjectStyle &style, uint8_t layer = 0u) const;

    /*!
      \brief Remove specified renderer from the list of currently used renderers.

      \param renderer_id Identifier of renderer to be removed.
    */
    void RemoveRenderer(int32_t renderer_id) const;

#ifdef HIRO_ENABLE_CEGUI
    //! Adds a custom GUI element to the viewport of viewarea. Not available for educational builds.
    [[maybe_unused]] void AddCeguiElement(CEGUI::Element *element);
#endif

    //! Assign Viewarea to current instance. Not to be called by a user.
    void SetViewarea(Viewarea *viewarea);

  protected:

    //! Enables state saving, instantly loads file existing file if versions are equal.
    void EnableSavefile(const uint32_t current_version);

    /*!
      \brief Override this method to read custom state from auto save stream.

      This method is not designed to be called by user, it is called automatically by system.
      In case that current version specified by function EnableSavefile does not match version in a file, this method is not called.
      Note: Must be consistent with LoadState counterpart method.
    */
    virtual bool LoadState(std::istream &str);

    /*!
      \brief Override this method to write custom state to auto save stream.

      This method is not designed to be called by user, it is called automatically by system.
      Note: Must be consistent with LoadState counterpart method.
    */
    virtual void SaveState(std::ostream &str);

  private:
    const DataUnit *dataunit_;
    Viewarea *viewarea_;
    uint32_t savefile_version_ = 0u;
    bool is_savefile_enabled_ = false;
    bool OnStyleChange(const void *arg);
    void ReadStatefile();
    bool WriteStatefile();
  };

  using PDataInstance = std::shared_ptr<DataInstance>;

}