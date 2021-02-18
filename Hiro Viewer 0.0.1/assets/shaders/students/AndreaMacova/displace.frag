#version 430
#include "api_frag.glsl"

layout (location = 0) in vec3 position_eye;
layout (location = 1) in flat vec3 normal_eye;

layout (std140, binding = UBIND_CUSTOM_0) uniform MaterialBlock { Material in_material; };

out vec3 out_frag_color;

void main()
{
  WriteFragment(position_eye, normal_eye, in_material);
}