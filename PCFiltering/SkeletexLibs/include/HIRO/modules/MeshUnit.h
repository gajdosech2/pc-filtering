#pragma once
#include <string>

#include <COGS/Mesh.h>
#include <Visualizer/MeshObject.h>

#include <HIRO/modules/PointCloudUnit.h>



namespace hiro
{

  namespace modules
  {

    HIRO_API void AddMesh(const std::string &name, const cogs::Mesh &mesh);

    /*!
      \brief Unit which visualizes cogs::Mesh structures.
    */
    class HIRO_API MeshUnit : public PointCloudUnit
    {
    public:
      /*!
        \brief
          Constructor of TriPointcloudUnit
        \param name
          The name of the unit.
        \param pc
          Input data as T.
      */
      MeshUnit(const std::string &name, const cogs::Mesh &mesh);

      //! Returns number of faces.
      uint32_t GetFaceCount() const;

      //! Returns material assigned to mesh.
      const cogs::MaterialModel *GetMaterial() const;

      //! Creates TriPointCloudInstance. Function is handled by HIRO, non accessible by user.
      PDataInstance Instantiate() override;

      //!Gets the TriPointCloudObj pointer.
      vis::PMeshObject GetTriPointCloudObj() const;

    protected:

      uint32_t face_count_{ 0u };
      std::unique_ptr<cogs::MaterialModel> material_;
      MeshUnit(const hiro::DataId &id);
    };

    /*!
      \brief Instantiated class from MeshUnit.
    */
    class HIRO_API MeshInstance : public PointCloudInstance
    {
    public:
      MeshInstance(const MeshUnit *unit);

      void GenerateGui(hiro::GuiGenerator &gui) override;

      bool FocusCamera(glm::vec3 *focus_point, glm::vec3 *cam_position) const override;

      virtual void Initialize() override;

    protected:
      const MeshUnit *GetTpcUnit() const;
      vis::PMeshStyle GetTpcStyle() const;
      virtual void OnColorSourceChange() override;

    };

  }

}
