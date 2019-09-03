#include "include/math.glsl";
#include "visualizer/definitions.glsl"

mat3x4 GetTriangleEdgeVolume(in DefVertex verts[3], in vec3 triang_normal)
{
    mat3x4 result;
    for (int i = 0; i < 3; i++)
    {
        int i2 = (i == 2) ? 0 : i+1;
        vec3 edge = verts[i].position_eye - verts[i2].position_eye;
        vec3 edgep_normal = normalize(cross(triang_normal, edge));
        result[i] = vec4(edgep_normal, dot(edgep_normal, verts[i].position_eye));
    }
    return result;
}

DefFace GetTrianglePrimitive(in DefVertex verts[3], in vec3 triangle_color)
{
    DefFace triang;
    triang.normal = GetNormalCCW(verts[0].position_eye, verts[1].position_eye, verts[2].position_eye);
    vec3 pos_sum = verts[0].position_eye + verts[1].position_eye + verts[2].position_eye;
    triang.center = pos_sum / 3.0;
    triang.color = triangle_color;
    if (style.render_edges)
    {
        triang.edge_volume = GetTriangleEdgeVolume(verts, triang.normal);
    }
    return triang;
}
