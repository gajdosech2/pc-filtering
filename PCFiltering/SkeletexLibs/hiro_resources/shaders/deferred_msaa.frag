#version 430
#include "visualizer/def_frag.glsl"

layout (location = 0) in vec2 in_coord;

layout (location = LOC_CUSTOM_0) uniform uvec2 size;

layout (binding = 0) uniform sampler2DMS position;
layout (binding = 1) uniform sampler2DMS normal;
layout (binding = 2) uniform sampler2DMS material;

out vec3 out_frag_color;

void main()
{
	ivec2 coord = ivec2(in_coord * vec2(size));
	out_frag_color = vec3(0.0);

	vec4 map0 = texelFetch(position, coord, gl_SampleID);
    vec4 map1 = texelFetch(normal, coord, gl_SampleID);
    vec4 map2 = texelFetch(material, coord, gl_SampleID);

    Material material;
    material.color = map2.rgb;
	material.shininess = map0.a;
	material.specular = map1.a;
	material.is_metalic = false;
	material.use_lighting = map2.a > 0.01;

    out_frag_color = EvaluateLighting(map0.xyz, map1.xyz, material);
}
