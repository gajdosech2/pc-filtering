#version 430
#include "include/texturing.glsl"
#include "visualizer/def_deferred.glsl"

layout (location = 0) in flat uint in_is_back_facing;
layout (location = 1) in DefVertex in_vert;
layout (location = 5) in vec4 in_tangents;
layout (location = 6) in flat DefFace in_triang;

layout (binding = 0) uniform sampler2D albedo_map;
layout (binding = 1) uniform sampler2D normal_map;

void main()
{
	vec3 out_normal_eye = (style.force_flatshading) ? in_triang.normal : in_vert.normal_eye;
	Material material = GetDefaultMaterial();

	//MATERIAL
	if (style.fragcolor == 0){
		material.color = style.color;
	}
	//POINT_POSITIONS
	else if (style.fragcolor == 1){
		material.color = (VIEW_INV_MAT * vec4(in_vert.position_eye, 1.0)).xyz;
	}
	//POINT_NORMALS
	else if (style.fragcolor == 2){
		vec3 n = normalize((VIEW_INV_MAT * vec4(out_normal_eye, 0.0)).xyz);
		material.color = n;
	}
	//POINT_COLORS
	else if (style.fragcolor == 3){
		material.color = in_vert.color;
	}
	//POINT_NORMALS NORMALIZED
	else if (style.fragcolor == 4){
		vec3 n = normalize((VIEW_INV_MAT * vec4(out_normal_eye, 0.0)).xyz);
		material.color = n * 0.5 + 0.5;
	}
	//CHECKERBOARD
	else if (style.fragcolor == 5) {
		if (GetTexelCheckerboardValue(in_vert.coord, 128) > 0.5)
		{
			material.color = vec3(0.9, 1.0, 0.0);
		}
	}
	//ALBEDO_TEXTURE
	else if (style.fragcolor == 6) {
		material.color = texture(albedo_map, in_vert.coord).rgb;

		vec3 T = normalize(in_tangents.xyz);
		vec3 N = normalize(in_vert.normal_eye);

		vec3 B;
		if(in_tangents.w < 0){
			B = cross(T,N);
		}
		else{
			B = cross(N,T);
		}

		mat3 TBN = mat3(T, B, N);

		//vec3 loaded_normal = texture(normal_map, in_vert.coord).rgb;
		//loaded_normal = normalize(loaded_normal * 2.0 - 1.0);
		//out_normal_eye = normalize(TBN * loaded_normal);
	}

	/*if (in_is_back_facing == 1)
	{
		material.color *= 0.3;
	}*/

	WriteFragment(in_vert.position_eye, out_normal_eye, material);
}
