#version 430
#include "visualizer/def_deferred.glsl"

/// INPUT
layout (location = 0) in vec3 in_color;

/// SOURCE
void main()
{
	Material material = GetDefaultMaterial();
	material.color = in_color;
	material.use_lighting = false;

	WriteFragment(vec3(0.0), vec3(0.0), material);
}
