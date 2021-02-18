#version 430
#include "api_vert.glsl"

/// INPUT
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_coord;
layout (location = 3) in vec4 in_tangent;

/// OUTPUT
layout (location = 0) out vec3 out_position;
layout (location = 1) out vec2 out_coord;
layout (location = 2) out vec3 out_normal;
layout (location = 3) out vec4 out_tangent;
layout (location = 4) out mat4 out_transform;


void main () 
{
	out_transform = VIEW_MAT * MODEL_MAT;
	// if (use_instancing)
	// {
	// 	out_transform = out_transform * instance_matrices[gl_InstanceID] * preinstance_mat;
	// }
	out_position = in_position;
	out_coord = in_coord;
	out_normal = normalize(in_normal);
	out_tangent = normalize(in_tangent);
}