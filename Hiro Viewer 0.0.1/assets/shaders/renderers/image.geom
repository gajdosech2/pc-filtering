#version 430
#include "api_definitions.glsl"

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

/// UNIFORMS
layout (location = ULOC_CUSTOM_0) uniform uvec2 raster_size;
layout (location = ULOC_CUSTOM_1) uniform mat4 image_to_pixel;

/// OUTPUT
layout (location = 0) out vec2 out_coord;

const vec2 tex_coords[4] = vec2[4] (
   vec2(0.0, 1.0),
   vec2(1.0, 1.0),
   vec2(0.0, 0.0),
   vec2(1.0, 0.0)
);

void main()
{
    vec2 raster_points[4];
    raster_points[0] = vec2(0.0, 0.0);
    raster_points[1] = vec2(raster_size.x, 0.0);
    raster_points[2] = vec2(0.0, raster_size.y);
    raster_points[3] = vec2(raster_size);

    for (int i=0; i<4; ++i)
    {
        gl_Position = PROJ_MAT * image_to_pixel * vec4(raster_points[i], 0.0, 1.0);
        out_coord = tex_coords[i];
        EmitVertex();
    }

    EndPrimitive();
}

