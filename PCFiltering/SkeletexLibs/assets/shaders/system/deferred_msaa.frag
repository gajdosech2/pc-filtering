#version 430
#include "api_definitions.glsl"

layout (location = 0) in vec2 in_coord;

layout (location = ULOC_CUSTOM_0) uniform uvec2 size;

layout (binding = 0) uniform sampler2DMS position_sampler;
layout (binding = 1) uniform sampler2DMS normal_sampler;
layout (binding = 2) uniform usampler2DMS material_sampler;

out vec3 out_frag_color;

vec3 EvaluateLighting(in vec3 position_eye, in vec3 normal_eye, in Material material)
{
	if (material.use_lighting == false)
	{
		return material.color;
	}

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	vec3 V = normalize(-position_eye);
	vec3 N = normalize(normal_eye);

	uint used_light_count = 0;

	for (int i = 0; i < 4; i++)
	{
		int lid = SCENES[SCENE_ID].light_ids[i];

		if (lid < 0)
		{
			break;
		}

		vec3 L;
		vec3 light_eye = LIGHTS[lid].position.xyz;
		if (LIGHTS[lid].in_camera_space < 0.5)
		{
			light_eye = (VIEW_MAT * LIGHTS[lid].position).xyz;
		}

		if (LIGHTS[lid].position.w > 0.5)
		{
			L = normalize(light_eye - position_eye);
		}
		else
		{
			L = normalize(-light_eye);
		}

		vec3 H = normalize(L + V);

		ambient += LIGHTS[lid].ambient_intensity * LIGHTS[lid].color;
		diffuse += max(dot(N, L), 0.0) * LIGHTS[lid].color;
		specular += max(pow(dot(N, H), material.shininess), 0.0) * LIGHTS[lid].color * material.specular;

		used_light_count++;
	}

	vec3 ambient_color = material.color;
	vec3 diffuse_color = material.color;
	vec3 specular_color = vec3(1.0);

	if (material.is_metalic)
	{
		specular_color = material.color;
		diffuse = vec3(0.0);
	}

	vec3 lightning = ambient * ambient_color
		+ diffuse * diffuse_color
		+ specular * specular_color;

	return lightning;
}

bool IsBitSet(uint value, uint bit)
{
	return ((value >> bit) & 1) != 0;
}

void main()
{
	ivec2 coord = ivec2(in_coord * vec2(size));
	out_frag_color = vec3(0.0);

	vec4 map0 = texelFetch(position_sampler, coord, gl_SampleID);
    vec4 map1 = texelFetch(normal_sampler, coord, gl_SampleID);
    uvec4 map2 = texelFetch(material_sampler, coord, gl_SampleID);

    Material material;
    material.color = vec3(map2.rgb) / 255.0;
	material.shininess = map0.a;
	material.specular = map1.a;
	material.use_lighting = IsBitSet(map2.a, 0);
	material.is_metalic = IsBitSet(map2.a, 1);
	
    out_frag_color = EvaluateLighting(map0.xyz, map1.xyz, material);
}
