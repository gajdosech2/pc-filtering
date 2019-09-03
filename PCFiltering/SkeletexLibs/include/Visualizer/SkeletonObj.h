/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <COGS/Skeleton.h>
#include <COGS/SkeletonAction.h>

#include <Visualizer/Object.h>



namespace vis
{

  struct VIS_API SkeletonStyle : public ObjectStyle
  {
    Material bonejoint_material = Material(cogs::color::GRAY70);
    bool enable_lighting = true;
    bool display_basis = false;
    bool display_joints = true;
    enum Mode { OCTAHEDRAL, OCTAHEDRAL_WIRE, STICKMAN } render_mode = OCTAHEDRAL;
    StyleData GetBoneStyleData() const;
    StyleData GetBasisStyleData(const cogs::Color4b &color) const;
    bool LoadState(std::istream &str) override;
    void SaveState(std::ostream &str) override;
  };


  class VIS_API SkeletonObj : public Object
  {
  public:
    const uint32_t kBoneCount;
    SkeletonObj(const cogs::Skeleton &skeleton);
    SkeletonObj(const SkeletonObj &object) = delete;
    virtual ~SkeletonObj();
    bool IsCompatibileWithStyle(const ObjectStyle *style) override;

    void SetPose(const cogs::Pose &skelpose);

  protected:
    void LoadRequiredShaderPrograms(glw::ProgramList *programs) override;
    void Render(const std::string &program) override;

  private:
    const std::string kMySkeletonProgram;
    cogs::Skeleton skeleton_;
    glw::UniformBuffer<glm::mat4> joint_transform_buffer_;
    glw::UniformBuffer<glm::mat4> bone_transform_buffer_;
    glw::UniformBuffer<glm::mat4> basis_transform_buffer_;
    glw::ArrayObject bone_line_buffer_;
    void ResetTransfromBuffers();
  };


  using PSkeletonStyle = std::shared_ptr<SkeletonStyle>;
  using PSkeletonObj = std::shared_ptr<SkeletonObj>;
}


