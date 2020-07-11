/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <COGS/Interpolator.h>
#include <COGS/Skeleton.h>
#include <COGS/API.h>

namespace cogs
{

  struct COGS_API SkeletonAction
  {
    SkeletonAction() = default;

    // returns time of last existing key frame
    float GetDuration() const;

    // checks whether all channels are empty
    bool IsEmpty() const;

    // adjusts pose using data from non empty channels
    void AdjustPoseToTime(float time, const Skeleton &target_skeleton, Pose *out_pose) const;

    // builds all frames of pose animation for specified skeleton
    std::vector<Pose> BakePoseAnimation(const Skeleton &target_skeleton) const;

    std::string name;
    std::string target;
    float fps = 1.0f;

    struct RootChannels
    {
      Interpolator<glm::quat> rotation;
      Interpolator<glm::vec3> position;
      // checks whether all channels are empty
      bool IsEmpty() const;
    };
    RootChannels root_channels;

    struct BoneChannels
    {
      Interpolator<glm::quat> rotation;
      Interpolator<glm::vec3> offset;
      Interpolator<glm::vec3> scaling;
      Interpolator<float> length;
      // checks whether all channels are empty
      bool IsEmpty() const;
      // adjusts bone transform using data from non empty channels
      void AdjustTransformToTime(float time, BoneTransform *out_trans) const;
      // returns time of last existing key frame
      float GetDuration() const;
    };
    std::map<std::string, BoneChannels> bone_channels;

  };

}