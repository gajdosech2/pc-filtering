#version 430
#include "api_frag.glsl"
#include "include/texturing.glsl"

#define ALPHA_ZERO_SQUARLE_SIZE 20.0

layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;

//TexCoord
layout (location = 0) in vec2 in_coord;

// Texture
layout (binding = 0) uniform sampler2D ras_tex;

/// SOURCE
void main()
{
  const vec4 image_color = texture(ras_tex, in_coord);
  const vec3 alpha_zero = mix(vec3(0.4),vec3(0.6),GetCheckerGridValue(gl_FragCoord.xy / ALPHA_ZERO_SQUARLE_SIZE));
  const vec3 final_color = mix(alpha_zero, image_color.rgb, image_color.a);
  WriteFragment(final_color);
}
