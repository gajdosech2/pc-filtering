#version 430
#include "visualizer/definitions.glsl"

/// INPUT
layout (location = 0) in vec3 in_position;
layout (location = 2) in vec2 in_coords;

/// OUTPUT
layout (location = 0) out vec3 out_position;
layout (location = 1) out vec2 out_coords;

void main ()
{
	mat4 transform = PROJ_MAT * MODELVIEW_MAT;
	if (use_instancing)
	{
		transform = transform * instance_matrices[gl_InstanceID] * preinstance_mat;
	}

	gl_Position = transform * vec4(in_position, 1.0);

	out_coords = in_coords;
	out_position = gl_Position.xyz;
}
