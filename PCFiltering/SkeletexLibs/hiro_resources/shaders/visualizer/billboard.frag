#version 430
#include "visualizer/def_deferred.glsl"

/// INPUT
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_coords;

/// UNIFORM
layout (binding = 0) uniform sampler2D albedo_map;

void main()
{
	Material material = GetDefaultMaterial();
	material.color = texture(albedo_map, in_coords).rgb;
	material.use_lighting = false;


	WriteFragment(in_position, vec3(0.0), material);
}
