/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <COGS/Armature.h>

#include <Visualizer/MeshObject.h>



namespace cogs
{
  struct Pose;
}

namespace vis
{

  struct VIS_API SkinnedMeshStyle : public MeshStyle
  {
  };


  class VIS_API SkinnedMeshObj : public MeshObject
  {
  public:
    SkinnedMeshObj(const cogs::Armature &armature);
    SkinnedMeshObj(const SkinnedMeshObj &object_ptr) = delete;
    virtual ~SkinnedMeshObj() = default;

    void SetPose(const cogs::Pose &pose);

  protected:
    void LoadRequiredShaderPrograms(glw::ProgramList *programs) override;
    bool IsCompatibileWithStyle(const ObjectStyle *style) override;
    void Render(const std::string &program) override;

  private:
    const uint32_t kBoneCount;
    const std::string kMyProgramPoints, kMyProgram;
    cogs::Armature skeleton_;
    glw::UniformBuffer<glm::mat4> bindpose_transforms_;
    glw::UniformBuffer<glm::mat4> pose_transforms_;
  };


  using PSkinnedMeshStyle = std::shared_ptr<SkinnedMeshStyle>;
  using PSkinnedMeshObj = std::shared_ptr<SkinnedMeshObj>;

}


