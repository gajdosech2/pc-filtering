#pragma once
#include <string>
#include <COGS/Armature.h>
#include <COGS/SkeletonAction.h>
#include <HIRO/modules/SkeletonUnit.h>



namespace hiro
{

  namespace modules
  {

    class HIRO_API SkinnedMeshUnit : public SkeletonUnit
    {
    public:
      uint64_t kPointCount;
      uint64_t kFaceCount;
      SkinnedMeshUnit(const std::string &name, const cogs::Armature &arm);
      PDataInstance Instantiate() override;
      const cogs::Armature *GetArmature() const;
    private:
      cogs::Armature armature_;
    };

    HIRO_API std::shared_ptr<SkinnedMeshUnit> AddSkinnedMesh(const std::string &name, const cogs::Armature &arm);

  }

}
