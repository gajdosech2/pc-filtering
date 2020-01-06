/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <string>

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

#include <COGS/Color.h>
#include <HIRO_DRAW/API.h>



namespace hiro::shader
{
  // CAUTION:
  // anything changed in this section needs to be updated accordingly in GLSL

  //! Maximal usable number of scenes.
  const int MAX_SCENE_COUNT{ 16 };
  //! Maximal usable number of cameras.
  const int MAX_CAMERA_COUNT{ 16 };
  //! Maximal usable number of lights.
  const int MAX_LIGHT_COUNT{ 16 };
  //! Maximal usable number of lights per scene
  const int MAX_LIGHT_COUNT_PER_SCENE{ 4 };
  //! Maximal usable number of Renderer-Style pairs per scene.
  const int MAX_INSTANCE_COUNT{ 32 };

  //! Location of model matrix stored in hiro::draw::Style::transform.
  const int ULOC_MODEL_MATRIX{ 0 };
  //! Location of scene index, that currently renders.
  const int ULOC_SCENE_ID{ 4 };
  //! Location of boolean deciding whether instanced rendering is used.
  const int ULOC_USE_INSTANCING{ 5 };
  //! Location of a pre-instance matrix.
  const int ULOC_PRE_INSTANCING_MATRIX{ 6 };

  const int ULOC_CUSTOM_0{ 10 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_1{ 11 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_2{ 12 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_3{ 13 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_4{ 14 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_5{ 15 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_6{ 16 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_7{ 17 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_8{ 18 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_9{ 19 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_10{ 20 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_11{ 21 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_12{ 22 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_13{ 23 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_14{ 24 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_15{ 25 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_16{ 26 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_17{ 27 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_18{ 28 }; //!< Location usable for custom uniform.
  const int ULOC_CUSTOM_19{ 29 }; //!< Location usable for custom uniform.

  //! Uniform binding location for instance matrices.
  const int UBIND_INSTANCING_MATRICES{ 0 };
  //! Uniform binding location for camera view matrices.
  const int UBIND_VIEW_MATRICES{ 1 };
  //! Uniform binding location for scene data structures.
  const int UBIND_SCENE_DATA{ 2 };
  //! Uniform binding location for light structures.
  const int UBIND_LIGHTS{ 3 };

  const int UBIND_CUSTOM_0{ 4 }; //!< Uniform binding location for custom data.
  const int UBIND_CUSTOM_1{ 5 }; //!< Uniform binding location for custom data.
  const int UBIND_CUSTOM_2{ 6 }; //!< Uniform binding location for custom data.
  const int UBIND_CUSTOM_3{ 7 }; //!< Uniform binding location for custom data.
  const int UBIND_CUSTOM_4{ 8 }; //!< Uniform binding location for custom data.
  const int UBIND_CUSTOM_5{ 9 }; //!< Uniform binding location for custom data.
  const int UBIND_CUSTOM_6{ 10 }; //!< Uniform binding location for custom data.
  const int UBIND_CUSTOM_7{ 11 }; //!< Uniform binding location for custom data.
  const int UBIND_CUSTOM_8{ 12 }; //!< Uniform binding location for custom data.
  const int UBIND_CUSTOM_9{ 13 }; //!< Uniform binding location for custom data.



  //! Properties of a simple phong material.
  struct HIRO_DRAW_API Material
  {
    //! Creates a default material.
    Material() = default;
    //! Creates a default material with a specified color.
    Material(const cogs::Color3f &col);
    //! Defines material color.
    cogs::Color3f color{ cogs::color::GRAY70 };
    //! Intensity of specular reflection. Should be in range <0,1>.
    float specular{ 0.5f };
    //! Phong shininess parameter. Bigger values result in smaller specular reflections.
    float shininess{ 60.0f };
    //! When a material is metallic, it uses only colored specular component.
    bool metallic{ false };
    uint8_t __padding0{ 0 };
    uint16_t __padding1{ 0 };
    //! Can be used to disable lighting and colorize the output with unmodified material color.
    bool use_lighting{ true };
    uint8_t __padding2{ 0 };
    uint16_t __padding3{ 0 };
  };

  //! Definition of scene light.
  struct HIRO_DRAW_API Light
  {
    //! Lighting position or direction (defined by homogeneous coordinate).
    glm::vec4 position{ 0.0f };
    //! Generated color influence of light.
    cogs::Color3f color{ cogs::color::WHITE };
    //! Ambient color influence of light.
    float ambient_intensity{ 0.1f };
    //! Whether is light position defined in camera space.
    float in_camera_space{ false };
    glm::vec3 __padding{ 0.0f };

    static Light CreateLight(
      const glm::vec4 &position,
      const cogs::Color3f &color,
      float ambient_intensity,
      bool in_camera_space);

    static Light CreatePointLight(
      const glm::vec3 &position,
      const cogs::Color3f &color,
      float ambient_intensity);

    static Light CreateDirectionalLight(
      const glm::vec3 &direction,
      const cogs::Color3f &color,
      float ambient_intensity);

    //! A point light whose position is at the position of the camera, oriented in direction of view.
    static Light CreateHeadLight(const cogs::Color3f &color);
  };

  //! Matrix and its inverse.
  struct MatAndInv
  {
    //! Initializes the structure by setting a matrix to the specified and calculating inverse.
    MatAndInv(const glm::mat4 &mat);
    glm::mat4 matrix{ 1.0f };
    glm::mat4 inverse{ 1.0f };
  };

  //! Scene information wrapper.
  struct SceneData
  {
    //! Scene projection matrix.
    glm::mat4 projection;
    //! Inverse to scene projection matrix.
    glm::mat4 projection_inverse;
    //! Index of a camera used by this scene.
    int32_t camera_id;
    glm::vec3 __padding0{ 0.0f };
    //! Indices of lights used by this scenes.
    glm::ivec4 light_ids;
    //! Resolution of a scene viewport.
    glm::ivec2 resolution;
    glm::vec2 __padding1{ 0.0f };
  };

}
