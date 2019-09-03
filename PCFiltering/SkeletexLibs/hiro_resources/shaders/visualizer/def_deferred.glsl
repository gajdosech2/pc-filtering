#include "visualizer/def_frag.glsl"

Material GetDefaultMaterial()
{
	Material result;
	result.color = style.color;
	result.shininess = style.shininess;
	result.specular = style.specular_intensity;
	result.use_lighting = style.use_lightning;
	result.is_metalic = style.is_metalic;
	return result;
}
	layout (location = 0) out vec4 out_position;
	layout (location = 1) out vec4 out_normal;
	layout (location = 2) out vec4 out_material;

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

	//Material color
	out_material.rgb = material.color;
	//Boolean to decide use_lightning.
	out_material.a = material.use_lighting ? 1.0 : 0.0;
	// todo: material.a = add here material.is_metalic
}
