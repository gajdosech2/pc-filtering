/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <map>
#include <utils/GeometryStructures.h>

#include <COGS/Bone.h>
#include <COGS/API.h>

namespace cogs
{
  //! Struct used for storage of specific configuration of bone transformations in a skeleton.
  struct COGS_API Pose
  {
    //! Transformation matrix used for all bones in the skeleton.
    Transform root_transform;
    //! Per-bone specific transformations.
    std::vector<BoneTransform> bone_transforms;

    /*! \brief
      Interpolates between this pose and target pose.

      Target pose must be designed for same skeleton.
      cogs::Transform::InterpolateWith is used for each corresponding pair of transformations.
    */
    void InterpolateWith(const Pose &target_pose, float target_weight, Pose *out_pose);
  };

  //! 3D Graph structure, usually representing topology of a 3D model.
  class COGS_API Skeleton
  {
    friend class Importer;
  public:

    //! Abstract helper class used for storage of skeleton properties.
    struct BaseProperty
    {
      BaseProperty(const uint32_t size)
        : size_(size)
      {};
      virtual ~BaseProperty() {};

      const uint32_t size_;
    };

    /*! \brief
      Template class used for storage of skeleton properties.

      Input class T must not own any objects or primitives referenced by pointers.
    */
    template<class T>
    struct Property : public BaseProperty
    {
      Property(const T &new_data)
        : BaseProperty(sizeof(*this)), data_(new_data)
      {};

      virtual ~Property() override {};

      //! Returns stored data.
      const T GetData() const
      {
        return data_;
      };
    private:
      const T data_;
    };

    //! Creates an empty skeleton.
    Skeleton();

    //! Creates a copy of specified skeleton.
    Skeleton(const Skeleton &skeleton);

    // #TODO remove or replace this constructor due to ambiguous meaning
    //! Creates new skeleton which shares topology, bone captions and radiuses with source skeleton.
    //! But has different bone head/tail positions and basis orientations.
    Skeleton(const cogs::Skeleton &src_skeleton, const glm::vec3 &new_root_position, const std::vector<glm::vec3> &new_tail_positions);

    //! Creates a copy of specified skeleton.
    Skeleton &operator=(const Skeleton &skeleton);

    //! Destroys instance and free all its data.
    ~Skeleton();

    std::string GetName() const;

    void SetName(const std::string &new_name);

    //! Returns all bones of skeleton.
    std::vector<Bone *> GetAllBones() const;

    std::vector<std::string> GetAllBoneCaptions() const;

    //! Returns all bones of skeleton.
    std::vector<Bone *> GetBones(const std::vector<std::string> &bone_captions) const;

    //! Returns bone with specified id.
    Bone *GetBone(const uint32_t id) const;

    //! Returns bone with specified caption.
    Bone *GetBone(const std::string &bone_caption) const;

    //! Returns all bones which are starting the sub hierarchies.
    std::vector<Bone *> GetRootBones() const;

    //! Removes all bones from skeleton and clears hierarchy data.
    void Clear();

    //! Number of bones in skeleton hierarchy.
    unsigned GetBoneCount() const;

    //! Will add bone to skeleton, which starts at the fathers tail position and ends in specified tail_position.
    Bone *AddBone(const std::string &name, const std::string &parent_name, const glm::vec3 &tail_position);

    //! Will add bone to skeleton, which has a defined bone transform.
    Bone *AddBone(const std::string &name, const std::string &parent_name, const BoneTransform &transform);

    //! Adds bone to skeleton, which starts at skeleton root position and ends in defined tail_position.
    Bone *AddBoneToRoot(const std::string &name, const glm::vec3 &tail_position);

    //! Adds bone to skeleton, which has a defined bone transform.
    Bone *AddBoneToRoot(const std::string &name, const BoneTransform &transform);

    //! Rolls basis of a single bone, without manipulating whole child hierarchy.
    void RollBasis(const std::string &bone_name, const float roll_angle);

    //! Resizes individual bones their orientation will not change.
    void ApplyBoneLengths(const std::vector<float> &bone_lengths);

    /*! /brief
      Sorts the vector of bones (returned by GetBones) so it corresponds with input skeleton.

      /param ref_skeleton
      Reference skeleton, must have same number of bones with same names as this skeleton.
    */
    void SortBySkeleton(const cogs::Skeleton &ref_skeleton);

    /*! /brief
      Interpolates skeleton into target skeleton (same principle as Pose::InterpolateWith).

      /param target_skel
      Target skeleton. Must have same number of bones and same bone names as this skeleton.

      /param target_weight
      Number in interval <0, 1>, if equals 0 then output will be copy of this skeleton. If equals 1, output will be copy of target skeleton.

      /param out_skeleton
      Result of the interpolation.
    */
    void InterpolateTo(const cogs::Skeleton &target_skel, float target_weight, cogs::Skeleton *out_skeleton) const;

    Pose GetActualPose() const;
    Pose GetActualPose(const std::vector<std::string> &bone_order) const;
    void ApplyPose(const Pose &pose, const bool ignore_root_transform = false);

    //! Sets default pose (pose with only identity matrices).
    void ApplyBindpose();

    //! Applies current pose transformations on bones and sets pose transformations to identities.
    void ApplyCurrentPoseAsBindpose();


    //! Returns specific skeleton property. If property do not exist, throws error.
    [[deprecated]]
    BaseProperty *GetProperty(const std::string &prop) const;

    //! Returns specific skeleton property. If property do not exist, throws error.
    template<class T>
    [[deprecated]]
    T GetProperty(const std::string &prop) const
    {
      assert(HasProperty(prop));
      return ((Property<T> *)(properties_.at(prop)))->GetData();
    }

    //! Sets specific skeleton property. If property do not exist yet, creates new property record.
    template<class T>
    [[deprecated]]
    void SetProperty(const std::string &prop, const T &value)
    {
      if (HasProperty(prop))
      {
        delete properties_[prop];
      }
      properties_[prop] = new Property<T>(value);
    }

    //! Checks if the property exist.
    [[deprecated]]
    bool HasProperty(const std::string &prop) const;

    cogs::Transform transform;

  protected:

    std::string name_;

    cogs::Transform bind_transform;

    std::unordered_map<std::string, BaseProperty *> properties_;

    std::map <std::string, Bone *> bone_for_name_;
    std::vector<Bone *> bones_;
    std::vector<Bone *> root_bones_;
    unsigned bone_count_ = 0;

    Bone *CreateNewBone(const std::string &caption);
    Bone *CreateBoneClone(const cogs::Bone *bone);
    void MakeCloneOf(const Skeleton &skeleton);

    void OnTransformChange();
  };



  COGS_API std::vector<geom::LineSegment3> ToLineSegments(const cogs::Skeleton &skeleton);

  //! Splits skeleton into one bone skeletons.
  COGS_API std::vector<cogs::Skeleton> SplitIntoSingleBoneSkeletons(const cogs::Skeleton &skeleton);

  COGS_API uint32_t FindClosestBoneToTriangle(const cogs::Skeleton &skeleton, const geom::Triangle3 &triangle);

  //! Subdivides skeleton bones into segments of same lengths according their radii (greater the radius, greater the number of fragments).
  COGS_API cogs::Skeleton TesselateSkeletonByRadiuses(const cogs::Skeleton &skeleton);

  COGS_API geom::Aabb3 GetAabb3(const Skeleton &skeleton);
}
