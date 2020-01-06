#include "api_definitions.glsl"

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out uvec4 out_material;

void WriteFragment(in vec3 position_eye, in vec3 normal_eye, in Material material)
{
	// Store the fragment position vector in the first gbuffer texture.
	out_position.rgb = position_eye;
	// As the alpha value we store shininess.
	out_position.a = material.shininess;

	// Store the per-fragment normals.
	out_normal.rgb = normal_eye;
	// As the alpha value we store specular intensity.
	out_normal.a = material.specular;

	// Material color
	out_material.rgb = uvec3(material.color * 255.0);
	// boolean parameter bits
	out_material.a = 0;
	if (material.use_lighting)
		out_material.a |= 1 << 0;
	if (material.is_metalic)
		out_material.a |= 1 << 1;
}

void WriteFragment(in vec3 color)
{
	out_position = vec4(0.0);
	out_normal = vec4(0.0);
	out_material = uvec4(uvec3(color * 255.0), 0);
}
