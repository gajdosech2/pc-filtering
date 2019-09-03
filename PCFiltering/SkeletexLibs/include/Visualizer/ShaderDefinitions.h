/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <string>

namespace vis
{

  namespace shader
  {
    const std::string SHADER_NAME_SIMPLE_COLOR = "VIS_SIMPLE_COLOR";
    const std::string SHADER_NAME_IFACE = "VIS_IFACE";
    const std::string TEXT_SHADER = "VIS_TEXT_RENDER";

    // CAUTION: anything changed in following section needs to be updated accordingly in shader file constants.glsl

    //! Maximal number of scenes that can be used at the same time.
    const int MAX_SCENE_COUNT = 16;
    //! Maximal number of individual cameras that can be used at the same time.
    const int MAX_CAMERA_COUNT = 16;
    //! Maximal number of lights that can be available in the renderer.
    const int MAX_LIGHT_COUNT = 16;
    //! Maximal number of lights that can be set for a single scene.
    const int MAX_LIGHT_COUNT_PER_SCENE = 4;
    //! Maximal number of OpenGL instances available when using instanced rendering.
    const int MAX_INSTANCE_COUNT = 32;

    const int ULOC_MODEL_MATRIX = 0;
    const int ULOC_SCENE_ID = 4;
    const int ULOC_USE_INSTANCING = 5;
    const int ULOC_PRE_INSTANCING_MATRIX = 6;
    const int ULOC_CUSTOM_0 = 10;
    const int ULOC_CUSTOM_1 = 11;
    const int ULOC_CUSTOM_2 = 12;
    const int ULOC_CUSTOM_3 = 13;
    const int ULOC_CUSTOM_4 = 14;
    const int ULOC_CUSTOM_5 = 15;
    const int ULOC_CUSTOM_6 = 16;
    const int ULOC_CUSTOM_7 = 17;
    const int ULOC_CUSTOM_8 = 18;
    const int ULOC_CUSTOM_9 = 19;
    const int ULOC_CUSTOM_10 = 20;
    const int ULOC_CUSTOM_11 = 21;

    const int UBIND_INSTANCING_MATRICES = 0;
    const int UBIND_VIEW_MATRICES = 1;
    const int UBIND_SCENE_DATA = 2;
    const int UBIND_LIGHTS = 3;
    const int UBIND_STYLE = 4;
    const int UBIND_CUSTOM_0 = 5;
    const int UBIND_CUSTOM_1 = 6;
  }

}
