#version 430
#include "visualizer/definitions.glsl"

/// INPUT
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

/// OUTPUT
layout (location = 0) out vec3 out_color;

/// UNIFORMS
layout (location = LOC_CUSTOM_0) uniform bool use_uniform_color = false;
layout (location = LOC_CUSTOM_1) uniform vec3 uniform_color;

/// SOURCE
void main () 
{
	mat4 transform = MVP_MAT;
	if (use_instancing)
	{
		transform = transform * instance_matrices[gl_InstanceID] * preinstance_mat;
	}
	
	if (use_uniform_color)
	{
		out_color = uniform_color;
	}
	else
	{
		out_color = in_color;
	}
	
	gl_Position = transform * vec4(in_position, 1.0);
}