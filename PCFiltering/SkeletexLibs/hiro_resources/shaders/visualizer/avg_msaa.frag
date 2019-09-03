#version 430
#include "visualizer/def_frag.glsl"

layout (location = 0) in vec2 in_coord;

layout (location = LOC_CUSTOM_0) uniform uvec2 size;

layout (binding = 0) uniform sampler2DMS msaa_texture;

out vec3 out_frag_color;

void main()
{
	ivec2 coord = ivec2(in_coord * vec2(size));
	out_frag_color = texelFetch(msaa_texture, coord, 0).rgb;
	
  	for (int i = 1; i < _SHADER_SAMPLES_; i++)
	{
		vec4 map0 = texelFetch(msaa_texture, coord, i);
		out_frag_color += map0.xyz;
	}

	if (_SHADER_SAMPLES_ == 0){
		out_frag_color = out_frag_color;
	}

	out_frag_color /= float(_SHADER_SAMPLES_);
}
