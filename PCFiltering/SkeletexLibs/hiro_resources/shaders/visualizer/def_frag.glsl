#include "visualizer/definitions.glsl"

struct Material
{
	float shininess;
	float specular;
	vec3 color;
  bool is_metalic;
  bool use_lighting;
};

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
		int lid = scenes[scene_id].light_ids[i];

		if (lid < 0)
		{
			break;
		}

		vec3 L;
		vec3 light_eye = lights[lid].position.xyz;
		if (lights[lid].in_camera_space < 0.5)
		{
			light_eye = (VIEW_MAT * lights[lid].position).xyz;
		}

		if (lights[lid].position.w > 0.5)
		{
			L = normalize(light_eye - position_eye);
		}
		else
		{
			L = normalize(-light_eye);
		}

		vec3 H = normalize(L + V);

		ambient += lights[lid].ambient_intensity * lights[lid].color;
		diffuse += max(dot(N, L), 0.0) * lights[lid].color;
		specular += max(pow(dot(N, H), material.shininess), 0.0) * lights[lid].color * material.specular;

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


bool ShouldHaveEdgeColor(in DefVertex vert, in DefFace face)
{
	if (!style.render_edges)
	{
		return false;
	}

	for (uint i=0; i<3; i++)
	{
		const vec4 edgep = face.edge_volume[i];
		if (edgep.w - dot(vert.position_eye, edgep.xyz) < style.edge_thickness)
		{
			return true;
		}
	}
	return false;
}
