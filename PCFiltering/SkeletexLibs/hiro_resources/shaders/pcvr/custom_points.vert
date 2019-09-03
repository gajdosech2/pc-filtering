#version 430
#include "visualizer/definitions.glsl"
#include "include/math.glsl"
#include "include/color_mapping.glsl"

/// INPUT
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_coords;
layout (location = 2) in vec3 in_displ_vector;

/// OUTPUT
layout (location = 0) out DefVertex out_vert;
layout (location = 10) out float out_vert_size;
layout (location = 11) out float out_depth_mm;

layout (location = LOC_CUSTOM_2) uniform float effect_time;
layout (location = LOC_CUSTOM_3) uniform uint effect_id;
layout (location = LOC_CUSTOM_4) uniform vec3 intensity_params;
layout (location = LOC_CUSTOM_5) uniform vec2 resolution;
layout (location = LOC_CUSTOM_6) uniform vec3 pc_origin_scansp;
layout (location = LOC_CUSTOM_7) uniform vec3 aabb_min_scansp;
layout (location = LOC_CUSTOM_8) uniform vec3 aabb_max_scansp;
layout (location = LOC_CUSTOM_9) uniform float flip_cloud;
layout (location = LOC_CUSTOM_10) uniform float use_depth_map;
layout (location = LOC_CUSTOM_11) uniform mat4 custom_transformation;

layout (binding = 1) uniform sampler2D depth_map;
layout (binding = 2) uniform sampler2D intensity_map;

#define DISCARD_POINT out_vert_size = -1.0; return;

bool IsNoise(float point_depth)
{
	int radius = 1;
	int friend_count = 0;
	for (float x = -radius; x <= radius; ++x)
	{
		for (float y = -radius; y <= radius; ++y)
		{
			vec2 coord = vec2(x,y) / resolution + in_coords;
			float friend_depth;
			if (use_depth_map > 0.5){
				friend_depth = texture(depth_map, coord).x;
			}else{
				friend_depth = length(texture(depth_map, coord).xyz);
			}
			if (abs(point_depth - friend_depth) < 5.0)
			{
				friend_count++;
			}
		}
	}
	return friend_count < 4;
}

bool IsEdgy()
{
	const vec2 border = vec2(100.0, 10.0);
	vec2 px_coord = in_coords * resolution;
	return px_coord.x < border.x || px_coord.y < border.y 
	|| px_coord.x > resolution.x - border.x || px_coord.y > resolution.y - border.y;
}

void main () 
{
	//out_vert.position_eye = (VIEW_MAT * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	//out_vert.normal_eye = (transform * vec4(in_normal, 0.0)).xyz;
	//out_vert.color = vec3(1.0);
	//out_vert.coord = in_coords;
	//return;
	
	mat4 transform = modelmat;
	if (use_instancing) {
		transform = modelmat * instance_matrices[gl_InstanceID] * preinstance_mat;
	}
	
	if (use_depth_map > 0.5){
		out_depth_mm = texture(depth_map, in_coords).x;
	}else{
		out_depth_mm = length(texture(depth_map, in_coords).xyz);
	}
	
	if (out_depth_mm == 0.0){
		DISCARD_POINT;
	}

	if (_ENABLED_NOISE_REDUCTION_ && IsNoise(out_depth_mm)) {
		DISCARD_POINT;
	}

	if (_ENABLED_BORDER_DISCARD_ && IsEdgy()) {
		DISCARD_POINT;
	}
	
	vec3 position_scan_sp;
	if (use_depth_map > 0.5){
		position_scan_sp = (custom_transformation * vec4(in_displ_vector * out_depth_mm, 1.0)).xyz * 0.001;
	}else{
		position_scan_sp = (custom_transformation * vec4(texture(depth_map, in_coords).xyz, 1.0)).xyz * 0.001;
	}

	//Photoneo to Skeletex coord system transform
	position_scan_sp.y = -position_scan_sp.y;
	position_scan_sp.z = -position_scan_sp.z;
	
	if (flip_cloud > 0.5){
		position_scan_sp.x = position_scan_sp.x - pc_origin_scansp.x;
		position_scan_sp.x = -position_scan_sp.x;
		position_scan_sp.x = position_scan_sp.x + pc_origin_scansp.x;
	}
	
	const float intensity_min = intensity_params.x;
	const float intensity_max = intensity_params.y;
	const float intensity_exp = intensity_params.z;
	
	float intensity = (texture(intensity_map, in_coords).x - intensity_min) / (intensity_max - intensity_min);
	
	//intensity correction
	//float vec_lenght = out_depth_mm * 0.001;
	//intensity *= pow(vec_lenght, 2.0);
	
	
	if (intensity_exp != 1.0){
		intensity = pow(intensity, intensity_exp);
	}
	
	vec3 position_cloud_sp = position_scan_sp - pc_origin_scansp;

	vec3 out_position_world_sp = (transform * vec4(position_cloud_sp, 1.0)).xyz;
	vec3 out_color = vec3(intensity);
	out_vert_size = 0.0;

	// SAND
	if (effect_id == 1)
	{
		float mod_time = effect_time - position_scan_sp.y * 0.5 - position_scan_sp.x * 0.4 - (position_scan_sp.z - aabb_min_scansp.z) * 0.5 ;
		mod_time -= 0.2 * Random(position_scan_sp.x * position_scan_sp.y - position_scan_sp.z);
		float step = pow(clamp(mod_time, 0.0, mod_time), 4.0);
		out_position_world_sp.y -= step;
		out_color = mix(out_color, vec3(0.9, 0.8, 0.6), clamp(mod_time, 0.0, 1.0));
	}
	// LIGHT SPEED
	else if (effect_id == 2)
	{
		float speed = Random(position_scan_sp.x * position_scan_sp.y - position_scan_sp.z) * 1.0 + 0.05;
		float mod_time = effect_time - position_scan_sp.z * 1.0 - length(position_scan_sp) * 2.0 + 2.0;
		mod_time *= 0.02 + 0.6 * speed;
		float step = pow(clamp(mod_time, 0.0, mod_time), 4.0);
		const vec3 center = (transform * vec4(0.0, 0.0, 0.0, 1.0)).xyz + vec3(0.0, 0.0, -1.0);
		//vec3 displace = -vec3(0.0, 0.0, 0.05 + 0.1 * Random(position_scan_sp.x * position_scan_sp.y + position_scan_sp.z));

		
		vec3 displace = in_displ_vector;
		//displace.z *= -1.0;
		displace.z -= 0.5 * Random(position_scan_sp.x * position_scan_sp.y + position_scan_sp.z);

		vec3 new_pos = position_cloud_sp + displace * step * 2.0;
		out_position_world_sp = (transform * vec4(new_pos, 1.0)).xyz;
	
		vec3 ef_color = vec3(0.4 + speed, 0.0, 0.8 - speed);
		out_color = mix(out_color, clamp(ef_color, 0.0, 1.0), clamp(mod_time, 0.0, 1.0));

		out_color *= clamp(distance(vec3(0.0),out_position_world_sp)*0.1 + 1.0, 0.0, 1.0);
	}
	// HOLOGRAM
	else if (effect_id == 3)
	{
		// invisible line
		if (fract(2.0 * position_scan_sp.y - effect_time * 0.2) < 0.01) {
			DISCARD_POINT;
		}
		// sinus pos distortion
		out_position_world_sp.x -= 0.002 * sin(11.0 * effect_time + position_cloud_sp.y * 40.0);
		// color hor sin paterns
		float sin_time = sin(3.0 * effect_time - position_scan_sp.x * 50.0) * 1.0 + 4.0;
		out_color *= sin_time;
		// color noise
		out_color *= 0.5 + 0.5 * Random(position_scan_sp.x * position_scan_sp.y * effect_time);
		// color blue tint
		out_color.r *= 0.4;
		out_color.g *= 0.7;
	}
	// ATOMIC PULSE
	else if (effect_id == 4)
	{
		const float pulse_speed = 1.1;
		const float pulse_hold = 8.0;
		const vec3 pulse_spread = vec3(10.5, 10.5, 5.0);
		const float noise = 0.95;

		float stage = 0.5 * cos(clamp(pulse_speed*effect_time, 0.0, TWO_PI)) + 0.5;
		stage = 1.0 - pow(stage, pulse_hold);

		const vec3 pc_center_world = (transform * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
		float dist = clamp(2.0 - distance(pc_center_world, out_position_world_sp), 0.0, 10.0);

		float rand = (1.0 - noise) + noise *Random(position_scan_sp.x * position_scan_sp.y - position_scan_sp.z);
		float rand0 = Random(position_scan_sp.z);
		float rand1 = Random(position_scan_sp.x);
		float rand2 = Random(position_scan_sp.y);

		// displacement
		vec3 direction = pulse_spread * normalize(position_scan_sp);
		direction.z *= -1.0;
		vec3 new_pos = position_cloud_sp + direction * stage * dist * rand;

		out_position_world_sp = (transform * vec4(new_pos, 1.0)).xyz;

		// jitter
		vec3 jitter;
		jitter.x = sin(effect_time * 28.5 * rand0 * (rand + 1.0));
		jitter.y = cos(effect_time * 25.0 * rand1 * (rand + 1.0));
		jitter.z = cos(effect_time * 22.8 * rand2 * (rand + 1.0));
		out_position_world_sp += jitter * 0.0015 * stage;

		out_vert_size = stage * 0.003;

		vec3 rand_cl = ValToColorRandom(rand * 0.5 + rand2 * 0.5);
		vec3 fancy_cl = mix(ValToColorRainbow(intensity * 0.7), rand_cl, 0.5) * 0.9 * (intensity + 0.3);
		fancy_cl = clamp(fancy_cl + 0.1, 0, 1);
		out_color = mix(vec3(intensity), fancy_cl, stage);
	}

	// DEBUG
	//if (IsNoise(out_depth_mm)) out_color = vec3(1.0, 0.0, 00);
	//if (IsEdgy()) out_color = vec3(1.0, 0.0, 0.0);

	out_vert.position_eye = (VIEW_MAT * vec4(out_position_world_sp, 1.0)).xyz;
	//out_vert.normal_eye = (transform * vec4(in_normal, 0.0)).xyz;
	out_vert.coord = in_coords;
	out_vert.color = out_color;
}