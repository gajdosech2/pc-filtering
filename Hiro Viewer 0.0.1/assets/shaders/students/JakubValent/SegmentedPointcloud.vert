#version 430
#include "api_vert.glsl"

#define MAX_SEGMENTS 50

// IN
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 4) in float in_intensity;
layout (location = 5) in float in_label;

// OUT
layout (location = 0) out vec3 out_position_eye;
layout (location = 1) out vec3 out_normal_eye;
layout (location = 2) out float out_intensity;

layout (location = 3) out vec3 out_source_color;
layout (location = 4) out float out_shininess;
layout (location = 5) out float out_ambient;
layout (location = 6) out float out_diffuse;
layout (location = 7) out float out_specular;
layout (location = 8) flat out uint out_shader;
layout (location = 9) out float out_roughness;
layout (location = 10) out float out_metalness;
layout (location = 11) out float out_shin_t;
layout (location = 12) out float out_shin_b;
layout (location = 13) flat out uint out_is_energy_conserving;

// Uniforms
layout (location = ULOC_CUSTOM_0) uniform int in_num_of_segments;
// layout (location = LOC_CUSTOM_1) uniform int in_add_intensity;
layout (location = ULOC_CUSTOM_2) uniform vec3 in_source_color[MAX_SEGMENTS];
layout (location = 62) uniform float in_shininess[MAX_SEGMENTS];
layout (location = 112) uniform float in_ambient[MAX_SEGMENTS];
layout (location = 162) uniform float in_diffuse[MAX_SEGMENTS];
layout (location = 212) uniform float in_specular[MAX_SEGMENTS];
layout (location = 262) uniform uint in_segment_labels[MAX_SEGMENTS];
layout (location = 312) uniform uint in_segment_shaders[MAX_SEGMENTS];
layout (location = 362) uniform float in_roughness[MAX_SEGMENTS];
layout (location = 412) uniform float in_metalness[MAX_SEGMENTS];
layout (location = 462) uniform float in_shin_t[MAX_SEGMENTS];
layout (location = 512) uniform float in_shin_b[MAX_SEGMENTS];
layout (location = 562) uniform uint in_is_energy_conserving[MAX_SEGMENTS];

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(void)
{
  mat4 modelview = VIEW_MAT * MODEL_MAT;
  gl_Position = modelview * vec4(in_position, 1.0);

  int segment_index = 0;
  for(int i = 0; i < in_num_of_segments; i++)
  {
    if(int(in_label) == in_segment_labels[i])
    {
      segment_index = i;
      break;
    }
  }

  if(in_segment_shaders[segment_index] == 0) // Blinn-Phong
  {
    //gl_Position = modelview * vec4(in_position, 1.0);

    
    out_shininess = in_shininess[segment_index];
    out_ambient =  in_ambient[segment_index];
    out_diffuse =  in_diffuse[segment_index];
    out_specular =  in_specular[segment_index];

    //out_position_eye = gl_Position.xyz;
    //vec4 normal_eye = modelview * vec4(in_normal, 0.0);
    //out_normal_eye = normal_eye.xyz;
    //gl_Position = PROJ_MAT * gl_Position;
  }
  else if(in_segment_shaders[segment_index] == 1) // Cook-Torrance
  {
    out_diffuse =  in_diffuse[segment_index];
    out_specular =  in_specular[segment_index];
    out_roughness =  in_roughness[segment_index];
    out_metalness = in_metalness[segment_index];
  }
  else if(in_segment_shaders[segment_index] == 2) // Oren-Nayar
  {
    out_roughness =  in_roughness[segment_index];
  }
  else if(in_segment_shaders[segment_index] == 3) // Ashikhmin-Shirley
  {
    out_shin_t = in_shin_t[segment_index];
    out_shin_b = in_shin_b[segment_index];
    out_is_energy_conserving = in_is_energy_conserving[segment_index];
  }
  out_source_color = in_source_color[segment_index];
  out_position_eye = gl_Position.xyz;
  out_normal_eye = (modelview * vec4(in_normal, 0.0)).xyz;
  gl_Position = PROJ_MAT * gl_Position;
  out_shader = in_segment_shaders[segment_index];
  out_intensity = in_intensity * 100.0;
}
