#define MAX_SCENE_COUNT 16
#define MAX_CAMERA_COUNT 16
#define MAX_LIGHT_COUNT 16
#define MAX_LIGHT_COUNT_PER_SCENE 4
#define MAX_INSTANCE_COUNT 32



struct Vertex
{
    vec3 position_eye;
    vec2 coord;
    vec3 normal_eye;
    vec3 color;
};

struct SceneData
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

struct Material
{
	vec3 color;
	float specular;
	float shininess;
	bool is_metalic;
	bool use_lighting;
};



#define UBIND_INSTANCING_MATRICES 0
#define UBIND_VIEW_MATRICES 1
#define UBIND_SCENE_DATA 2
#define UBIND_LIGHTS 3
#define UBIND_CUSTOM_0 4
#define UBIND_CUSTOM_1 5
#define UBIND_CUSTOM_2 6
#define UBIND_CUSTOM_3 7
#define UBIND_CUSTOM_4 8
#define UBIND_CUSTOM_5 9
#define UBIND_CUSTOM_6 10
#define UBIND_CUSTOM_7 11
#define UBIND_CUSTOM_8 12
#define UBIND_CUSTOM_9 13

layout (std140, binding = UBIND_INSTANCING_MATRICES) uniform InstanceMatBlock { mat4 INSTANCES[MAX_INSTANCE_COUNT]; };
layout (std140, binding = UBIND_VIEW_MATRICES) uniform CameraViewBlock { MatAndInv CAMERAS[MAX_CAMERA_COUNT]; };
layout (std140, binding = UBIND_SCENE_DATA) uniform SceneDataBlock { SceneData SCENES[MAX_SCENE_COUNT]; };
layout (std140, binding = UBIND_LIGHTS) uniform LightsBlock { Light LIGHTS[MAX_LIGHT_COUNT]; };



#define ULOC_MODEL_MATRIX 0
#define ULOC_SCENE_ID 4
#define ULOC_USE_INSTANCING 5
#define ULOC_PRE_INSTANCING_MATRIX 6
#define ULOC_CUSTOM_0 10
#define ULOC_CUSTOM_1 11
#define ULOC_CUSTOM_2 12
#define ULOC_CUSTOM_3 13
#define ULOC_CUSTOM_4 14
#define ULOC_CUSTOM_5 15
#define ULOC_CUSTOM_6 16
#define ULOC_CUSTOM_7 17
#define ULOC_CUSTOM_8 18
#define ULOC_CUSTOM_9 19
#define ULOC_CUSTOM_10 20
#define ULOC_CUSTOM_11 21
#define ULOC_CUSTOM_12 22
#define ULOC_CUSTOM_13 23
#define ULOC_CUSTOM_14 24
#define ULOC_CUSTOM_15 25
#define ULOC_CUSTOM_16 26
#define ULOC_CUSTOM_17 27
#define ULOC_CUSTOM_18 28
#define ULOC_CUSTOM_19 29



layout(location = ULOC_MODEL_MATRIX) uniform mat4 MODEL_MAT;
layout(location = ULOC_SCENE_ID) uniform uint SCENE_ID;
layout(location = ULOC_USE_INSTANCING) uniform bool USING_INSTANCING;
layout(location = ULOC_PRE_INSTANCING_MATRIX) uniform mat4 PRE_INSTANCE_MAT;



#define PRE_INSTANCE_INV_MAT inverse(PRE_INSTANCE_MAT)
#define MODEL_INV_MAT inverse(MODEL_MAT)
#define VIEW_MAT CAMERAS[SCENES[SCENE_ID].camera_id].matrix
#define VIEW_INV_MAT CAMERAS[SCENES[SCENE_ID].camera_id].inverse
#define PROJ_MAT SCENES[SCENE_ID].proj
#define PROJ_INV_MAT SCENES[SCENE_ID].proj_inverse
