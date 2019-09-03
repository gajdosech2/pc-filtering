#version 430
#include "visualizer/def_geom.glsl"

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

/// INPUT
layout (location = 0) in DefVertex in_vert[1];
layout (location = 10) in float in_vert_size[1];
layout (location = 11) in float in_depth_mm[1];

/// OUTPUT
layout (location = 0) out DefVertex out_vert;

/// UNIFORM
layout (location = LOC_CUSTOM_0) uniform vec4 psizing_mm;

const vec2 positions[4] = vec2[4] (
   vec2(-0.5, -0.5),
   vec2(+0.5, -0.5),
   vec2(-0.5, +0.5),
   vec2(+0.5, +0.5)
);
const vec2 coords[4] = vec2[4] (
   vec2(0.0, 0.0),
   vec2(1.0, 0.0),
   vec2(0.0, 1.0),
   vec2(1.0, 1.0)
);

void main()
{
	if (in_vert_size[0] < 0.0 ||
		(style.cull_backfaces && dot(in_vert[0].normal_eye, in_vert[0].position_eye) > 0.0))
	{
		return;
	}
	out_vert = in_vert[0];

	const vec2 depth_minmax = psizing_mm.xy;
	const vec2 scale_minmax = 0.001 * psizing_mm.zw;
	float depth_ratio = clamp((in_depth_mm[0] - depth_minmax.x) / (depth_minmax.y - depth_minmax.x), 0.0, 1.0);
	float basic_size = scale_minmax.x + depth_ratio * (scale_minmax.y - scale_minmax.x);
	
	for (int i=0; i<4; i++)
	{
		out_vert.coord = coords[i];

		float size = basic_size;
		if (in_vert_size[0] > 0.0)
		{
			size += in_vert_size[0];
		}

		vec3 displace = vec3(size * positions[i], 0.0);
		gl_Position = PROJ_MAT * vec4(in_vert[0].position_eye + displace, 1.0); 
		EmitVertex();
	}
	EndPrimitive();
}

