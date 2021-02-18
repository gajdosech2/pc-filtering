#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <Utils/GeometryStructures.h>

namespace utils
{

  //! Distance measurement units.
  enum class Units : uint8_t
  {
    m = 0,  //!< meter
    mm = 1  //!< millimeter
  };

  //! Gets ratio that describes relation between two provided units. target_value = source_value * ratio.
  static inline float GetUnitConversionRatio(Units source_units, Units target_units)
  {
    const std::map<Units, float> unit_scale
    {
      { Units::m, 1.0f },
      { Units::mm, 1000.0f }
    };
    auto src_unit_scale = unit_scale.at(source_units);
    auto tar_unit_scale = unit_scale.at(target_units);
    return tar_unit_scale / src_unit_scale;
  }

  //! Base axis.
  enum class BaseAxis : uint8_t
  {
    positive_x,
    positive_y,
    positive_z,
    negative_x,
    negative_y,
    negative_z
  };

  //! Converts base axis to vec3.
  static inline glm::vec3 BaseAxisToVec3(BaseAxis axis)
  {
    switch (axis)
    {
    case utils::BaseAxis::positive_x:
      return geom::AxisToVec3(geom::Axis::x);
    case utils::BaseAxis::positive_y:
      return geom::AxisToVec3(geom::Axis::y);
    case utils::BaseAxis::positive_z:
      return geom::AxisToVec3(geom::Axis::z);
    case utils::BaseAxis::negative_x:
      return -geom::AxisToVec3(geom::Axis::x);
    case utils::BaseAxis::negative_y:
      return -geom::AxisToVec3(geom::Axis::y);
    case utils::BaseAxis::negative_z:
      return -geom::AxisToVec3(geom::Axis::z);
    }
    return {};
  }

  //! Converts base axis to vec4, with possible custom w element value.
  static inline glm::vec4 BaseAxisToVec4(BaseAxis axis, float w = 0.0f)
  {
    return {BaseAxisToVec3(axis), w};
  }

  static inline bool AreCollinear(BaseAxis axis_a, BaseAxis axis_b)
  {
    const auto vec_a = BaseAxisToVec3(axis_a);
    const auto vec_b = BaseAxisToVec3(axis_b);

    std::optional<float> div = std::nullopt;
    for (glm::length_t i = 0; i < 3; ++i)
    {
      if (glm_ext::AreEqual(vec_b[i], 0.0f))
      {
        if (glm_ext::AreEqual(vec_a[i], 0.0f))
        {
          continue;
        }
        else
        {
          return false;
        }
      }
      else
      {
        float this_div = vec_a[i] / vec_b[i];
        if (div.has_value() && !glm_ext::AreEqual(*div, this_div))
        {
          return false;
        }
        div = this_div;
      }
    }
    return true;
  }

  //! Which hand to use.
  enum class Handedness : uint8_t
  {
    left,
    right
  };

  //! Definition of an arbitrary space.
  struct SpaceDefinition
  {
    Units units{ Units::mm };
    BaseAxis up{ BaseAxis::positive_y };
    BaseAxis forward{ BaseAxis::positive_z };
    Handedness handedness{ Handedness::right };

    SpaceDefinition() = default;

    SpaceDefinition(Units t_units, BaseAxis t_up,  BaseAxis t_forward, Handedness t_handedness)
      : units(t_units), up(t_up),  forward(t_forward), handedness(t_handedness)
    {
    };

    //! Creates basis matrix that defines this space.
    glm::mat4 CreateBasisMat() const
    {
      const auto src_up = BaseAxisToVec3(up);
      const auto src_fwd = BaseAxisToVec3(forward);
      const auto src_cr = handedness == Handedness::right ? glm::cross(src_up, src_fwd) : glm::cross(src_fwd, src_up);
      return { glm::vec4(src_cr, 0.0f), glm::vec4(src_up, 0.0f), glm::vec4(src_fwd, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
    }

    //! Creates transformation matrix that defines transition from this space to the target_space.
    glm::mat4 CreateTransformTo(SpaceDefinition target_space) const
    {
      const auto src_basis = CreateBasisMat();
      const auto tar_basis = target_space.CreateBasisMat();
      const auto unit_conversion_ratio = GetUnitConversionRatio(units, target_space.units);
      const glm::mat4 space_scaling { glm::scale({}, glm::vec3(unit_conversion_ratio)) };
      return space_scaling * tar_basis * glm::inverse(src_basis);
    }

    bool operator ==(SpaceDefinition const &b) const
    {
      return units == b.units
        && up == b.up
        && forward == b.forward
        && handedness == b.handedness;
    }

    bool operator !=(SpaceDefinition const &b) const
    {
      return !operator==(b);
    }
  };



  const SpaceDefinition PHOXI_MARKER_SPACE = []()
  {
    SpaceDefinition space;
    space.up = BaseAxis::positive_z;
    space.forward = BaseAxis::negative_x;
    space.units = Units::mm;
    space.handedness = Handedness::right;
    return space;
  }
  ();

  const SpaceDefinition PHOXIDEVICE_MARKER_SPACE = []()
  {
    SpaceDefinition space;
    space.up = BaseAxis::negative_z;
    space.forward = BaseAxis::negative_x;
    space.units = Units::m;
    space.handedness = Handedness::right;
    return space;
  }
  ();

  const SpaceDefinition COGS_MARKER_SPACE = []()
  {
    SpaceDefinition space;
    space.up = BaseAxis::positive_y;
    space.forward = BaseAxis::positive_z;
    space.units = Units::m;
    space.handedness = Handedness::right;
    return space;
  }
  ();

  const SpaceDefinition PHOXI_CAMERA_SPACE = []()
  {
    SpaceDefinition space;
    space.up = BaseAxis::negative_y;
    space.forward = BaseAxis::positive_z;
    space.units = Units::mm;
    space.handedness = Handedness::right;
    return space;
  }
  ();

  const SpaceDefinition COGS_CAMERA_SPACE = []()
  {
    SpaceDefinition space;
    space.up = BaseAxis::positive_y;
    space.forward = BaseAxis::negative_z;
    space.units = Units::m;
    space.handedness = Handedness::right;
    return space;
  }
  ();

}