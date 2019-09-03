#pragma once
#include <string>
#include <COGS/Skeleton.h>
#include <COGS/SkeletonAction.h>
#include <HIRO/DataUnit.h>



namespace hiro
{

  namespace modules
  {

    class HIRO_API SkeletonUnit : public DataUnit
    {
    public:
      const geom::AABB3 kAABB;
      SkeletonUnit(const std::string &name, const cogs::Skeleton &skeleton);
      SkeletonUnit(const std::string &name, const cogs::Skeleton &skeleton, const cogs::SkeletonAction &action);
      virtual PDataInstance Instantiate() override;
      void SetPose(const cogs::Pose &pose);
      const cogs::Skeleton &GetSkeleton() const;
      const cogs::SkeletonAction *GetAction() const;
      const cogs::Pose &GetPose() const;
      const cogs::Pose &GetRestPose() const;
    protected:
      SkeletonUnit(const DataId &dataid, const cogs::Skeleton &skeleton);
      cogs::Pose restpose_, pose_;
      cogs::Skeleton skeleton_;
      std::unique_ptr<cogs::SkeletonAction> skeleton_action_;
    };

    using PSkeletonUnit = std::shared_ptr<SkeletonUnit>;

    HIRO_API PSkeletonUnit AddSkeleton(const std::string &name, const cogs::Skeleton &skeleton);

    HIRO_API PSkeletonUnit AddSkeleton(const std::string &name, const cogs::Skeleton &skeleton, const cogs::SkeletonAction &action);

  }

}