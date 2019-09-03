#pragma once
#include <string>
#include <COGS/PointCloud.h>
#include <Visualizer/PointCloudObject.h>
#include <HIRO/DataUnit.h>



namespace hiro
{

  namespace modules
  {

    HIRO_API void AddPointCloud(const std::string &name, const cogs::PointCloud &pc);

    /*!
      \brief Data structure for point clouds.
      This class is a subclass of DataUnit, gathers and calculates information from input PointCloud. Creates PointCloudInstance.
    */
    class HIRO_API PointCloudUnit : public DataUnit
    {
    public:
      struct DataRecord
      {
        std::vector<float> data;
        float min, max;
      };

      /*!
        \brief Constructor of PointcloudUnit
        \param name
        The name of the unit.
        \param pc
        Input data as PointCloud.
      */
      PointCloudUnit(const std::string &name, const cogs::PointCloud &pc);

      //! Creates PointCloudInstance. Function is handled by HIRO, non accessible by user.
      PDataInstance Instantiate() override;

      //! Returns number of points.
      uint32_t GetPointCount() const;

      //! Returns a PointCloudObject pointer.
      vis::PPointCloudObject GetRenderer() const;

      //! Returns calculated center of mass.
      glm::vec3 GetCenterOfMass() const;

      //! Returns camera position based on center of mass.
      glm::vec3 GetCameraPos() const;

      //! Returns if coloring options for points are available.
      bool IsColoringAvailable() const;

      /*!
        \brief Returns current coloring option.
        It returns available RGB color values for one point with given id.
        \param id
        id of the given point
      */
      const std::vector<cogs::Color3f> *GetColoringOption(const uint32_t id) const;

      //! Returns names of coloring properties.
      const std::vector<std::string> GetColoringOptionNames() const;

      //! Returns if float data for points is available.
      bool IsFDataAvailable() const;

      /*!
        \brief Returns a DataRecord value for point.
        It returns available DataRecord value for one point with given id.
        \param id
        id of the given point
      */
      const DataRecord *GetFDataOption(const uint32_t id) const;

      //! Returns names of float data properties.
      const std::vector<std::string> GetFDataOptionNames() const;

    protected:
      PointCloudUnit(const DataId &id);
      vis::PPointCloudObject vis_object_{ nullptr };
      uint32_t point_count_{ 0u };
      void StoreNeededData(const cogs::PointCloud &pc);

    private:
      void CalculateCenterOfMass(const cogs::PointCloud &pc);
      glm::vec3 center_of_mass_, camera_pos_;
      std::vector<std::vector<cogs::Color3f>> coloring_options_;
      std::vector<std::string> coloring_option_names_;
      std::vector<DataRecord> fdata_options_;
      std::vector<std::string> fdata_option_names_;
    };



    /*!
      \brief Instantiated class from DataInstance.
      It stores changes from the instantiated PointcloudUnit. Saves and loads settings and generating GUI elements for PointcloudUnit.
    */
    class HIRO_API PointCloudInstance : public DataInstance
    {
    public:
      //! Constructor which takes PoindcloudUnit as input.
      PointCloudInstance(const PointCloudUnit *unit);

      //! Generates the GUI elements.
      void GenerateGui(hiro::GuiGenerator &gui) override;

      //! Changes camera position to focus on the center of mass of point cloud.
      bool FocusCamera(glm::vec3 *focus_point, glm::vec3 *cam_position) const override;

      //! Load previously saved changes .
      virtual bool LoadState(std::istream &str) override;

      //! Save changes of the PointcloudUnit made by user.
      virtual void SaveState(std::ostream &str) override;

      //! Adds PointCloudObject and PointCloudStyle to the ViewArea.
      virtual void Initialize() override;

    protected:

      enum ColorSource
      {
        MATERIAL = 0, // material color
        NORMALS = 1, // normals rendered as rgb
        COLORS = 2, // pure point cloud rgb property
        DATAVIS = 3, // color mapping of float data
        ALBEDOTEX = 4 // albedo texture
      };

      const PointCloudUnit *unit_;
      vis::PPointCloudStyle style_;

      int color_source_ = 0;
      int active_color_option_ = 0;
      int active_data_option_ = 0;
      float data_multiplier_ = 1.0f;
      cogs::ColorMap data_colormap_ = cogs::ColorMap::grayscale;

      PointCloudInstance(const PointCloudUnit *unit, vis::PPointCloudStyle style, uint32_t savefile_version);
      virtual void OnColorSourceChange();
      void UpdateColors() const;
      void UpdateDataMapping() const;
    };

  }

}
