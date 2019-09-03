#version 430
#include "visualizer/def_deferred.glsl"

layout (location = 0) in flat uint in_is_back_facing;
layout (location = 4) in DefVertex in_vert;

void main()
{
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
		vec3 n = normalize((VIEW_INV_MAT * vec4(in_vert.normal_eye, 0.0)).xyz);
		material.color = n;
	}
	//POINT_COLORS
	else if (style.fragcolor == 3){
		material.color = in_vert.color;
	}
	//POINT_NORMALS NORMALIZED
	else if (style.fragcolor == 4){
		vec3 n = normalize((VIEW_INV_MAT * vec4(in_vert.normal_eye, 0.0)).xyz);
		material.color = n * 0.5 + 0.5;
	}

	if (in_is_back_facing == 1)
	{
		material.color *= 0.3;
	}

	WriteFragment(in_vert.position_eye, in_vert.normal_eye, material);
}
