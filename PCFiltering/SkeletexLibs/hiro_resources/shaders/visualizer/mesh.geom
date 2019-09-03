#version 430
#include "visualizer/def_geom.glsl"

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

/// INPUT
layout (location = 0) in DefVertex in_vert[3];
layout (location = 4) in vec3 in_face_color[3];
layout (location = 5) in vec4 in_tangents[3];

/// OUTPUT
layout (location = 0) out flat uint out_is_back_facing;
layout (location = 1) out DefVertex out_vert;
layout (location = 5) out vec4 out_tangents;
layout (location = 6) out flat DefFace out_triang;

layout (location = LOC_CUSTOM_0) uniform float z_bias = 0.0;

void main()
{
    out_triang = GetTrianglePrimitive(in_vert, in_face_color[0]);

    bool is_back_facing = dot(out_triang.normal, out_triang.center) > 0.0;
    if (style.cull_backfaces && is_back_facing)
    {
        return;
    }

    out_is_back_facing = is_back_facing ? 1 : 0;
    for (int i = 0; i < 3; i++)
    {
        out_vert = in_vert[i];
        out_tangents = in_tangents[i];
        gl_Position = PROJ_MAT * vec4(in_vert[i].position_eye, 1.0);
        gl_Position.z -= z_bias;
        EmitVertex();
    }

    EndPrimitive();
}
