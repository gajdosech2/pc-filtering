#version 430
#include "visualizer/def_geom.glsl"

layout(points) in;
layout(line_strip, max_vertices = 2) out;

/// INPUT
layout (location = 0) in vec3 in_position[1];
layout (location = 1) in vec3 in_normal[1];

/// OUTPUT
layout (location = 0) out vec3 out_color;

/// UNIFORMS
layout (location = LOC_CUSTOM_0) uniform float normal_length;
layout (location = LOC_CUSTOM_1) uniform vec3 normal_color;

/// SOURCE
void main()
{
    if (style.cull_backfaces && dot(in_normal[0], in_position[0]) > 0.0) return;

	out_color = vec3(0,0,0);
    gl_Position = PROJ_MAT * vec4(in_position[0], 1.0); 
    EmitVertex();

	out_color = normal_color;
	gl_Position = PROJ_MAT * vec4(in_position[0] + normal_length * in_normal[0], 1.0); 
    EmitVertex();

    EndPrimitive();
}

