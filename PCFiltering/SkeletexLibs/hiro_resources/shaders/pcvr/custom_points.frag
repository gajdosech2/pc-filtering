#version 430
#include "visualizer/def_deferred.glsl"

layout (location = 0) in DefVertex in_vert;
layout (binding = 0) uniform sampler2D sprite;

void main()
{
	// discard out of circle
	vec2 normc = in_vert.coord * 2.0 - 1.0;
	if ((normc.x * normc.x + normc.y * normc.y - 1.0) > 0)
	{
		discard;
	}

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
		material.color = n * 0.5 + 0.5;
	}
	//POINT_COLORS
	else if (style.fragcolor == 3){
		material.color = in_vert.color;
	}
	
	WriteFragment(in_vert.position_eye, in_vert.normal_eye, material);  
}