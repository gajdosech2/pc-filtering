#version 430
#include "visualizer/definitions.glsl"

/// INPUT
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

/// OUTPUT
layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;

void main () 
{
	mat4 transform = MODELVIEW_MAT;
	if (use_instancing)
	{
		transform = transform * instance_matrices[gl_InstanceID] * preinstance_mat;
	}
	out_position = (transform * vec4(in_position, 1.0)).xyz;
	out_normal = (transform * vec4(in_normal, 0.0)).xyz;
}