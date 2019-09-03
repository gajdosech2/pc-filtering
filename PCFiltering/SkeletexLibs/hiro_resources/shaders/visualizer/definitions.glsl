#define MAX_SCENE_COUNT 16
#define MAX_CAMERA_COUNT 16
#define MAX_LIGHT_COUNT 16
#define MAX_LIGHT_COUNT_PER_SCENE 16
#define MAX_INSTANCES_COUNT 32

#define MVP_MAT scenes[scene_id].proj * views[scenes[scene_id].camera_id].matrix * modelmat
#define MODELVIEW_MAT views[scenes[scene_id].camera_id].matrix * modelmat
#define VIEW_MAT views[scenes[scene_id].camera_id].matrix
#define VIEW_INV_MAT views[scenes[scene_id].camera_id].inverse
#define PROJ_MAT scenes[scene_id].proj
#define PROJ_INV_MAT scenes[scene_id].proj_inverse

struct DefStyleData
{
	vec3 color;
	bool is_metalic;
	float specular_intensity;
	float shininess;
	bool use_lightning;
	bool force_flatshading;
	vec3 edge_color;
	bool render_edges;
	float edge_thickness;
	int fragcolor;
	bool cull_backfaces;
};

struct DefVertex
{
    vec3 position_eye;
    vec2 coord;
    vec3 normal_eye;
    vec3 color;
};

struct DefFace
{
    vec3 normal;
    vec3 center;
    vec3 color;
    mat3x4 edge_volume;
};

struct DefSceneData
{
	mat4 proj;
	mat4 proj_inverse;
	int camera_id;
	ivec4 light_ids;
	ivec2 resolution;
};

struct Light
{
	vec4 position;
	vec3 color;
	float ambient_intensity;
	float in_camera_space;
};

struct MatAndInv
{
	mat4 matrix;
	mat4 inverse;
};

layout (std140, binding = 0) uniform InstanceMatBlock { mat4 instance_matrices[MAX_INSTANCES_COUNT]; };
layout (std140, binding = 1) uniform CameraViewBlock { MatAndInv views[MAX_CAMERA_COUNT]; };
layout (std140, binding = 2) uniform SceneDataBlock { DefSceneData scenes[MAX_SCENE_COUNT]; };
layout (std140, binding = 3) uniform LightsBlock { Light lights[MAX_LIGHT_COUNT]; };
layout (std140, binding = 4) uniform StyleBlock { DefStyleData style; };

#define BIND_CUSTOM_0 5
#define BIND_CUSTOM_1 6

layout(location = 0) uniform mat4 modelmat;
layout(location = 4) uniform uint scene_id;
layout(location = 5) uniform bool use_instancing;
layout(location = 6) uniform mat4 preinstance_mat;

#define LOC_CUSTOM_0 10
#define LOC_CUSTOM_1 11
#define LOC_CUSTOM_2 12
#define LOC_CUSTOM_3 13
#define LOC_CUSTOM_4 14
#define LOC_CUSTOM_5 15
#define LOC_CUSTOM_6 16
#define LOC_CUSTOM_7 17
#define LOC_CUSTOM_8 18
#define LOC_CUSTOM_9 19
#define LOC_CUSTOM_10 20
#define LOC_CUSTOM_11 21
