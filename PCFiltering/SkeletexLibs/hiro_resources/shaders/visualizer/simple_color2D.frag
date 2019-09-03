#version 430
#include "visualizer/def_deferred.glsl"

/// INPUT
layout (location = 0) in vec3 in_color;

/// OUTPUT
out vec3 out_color;

/// SOURCE
void main()
{
	out_color = in_color;
}
