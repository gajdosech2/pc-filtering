#version 430
#include "include/math.glsl"
#include "api_definitions.glsl"

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

/// INPUT
layout (location = 0) in vec3 in_position_eye[3];

/// OUTPUT
layout (location = 0) out vec3 out_position_eye;
layout (location = 1) out flat vec3 out_normal_eye;

void main()
{
  out_normal_eye = GetNormalCCW(
    in_position_eye[0],
    in_position_eye[1],
    in_position_eye[2]
  );

  vec3 face_center_eye = (in_position_eye[0] + in_position_eye[1] + in_position_eye[2]) / 3.0;
  if (dot(out_normal_eye, face_center_eye) > 0.0)
  {
      return;
  }

  for (int i = 0; i < 3; i++) 
  {
      out_position_eye = in_position_eye[i];
      gl_Position = PROJ_MAT * vec4(out_position_eye, 1.0); 
      EmitVertex();
  }

  EndPrimitive();
}

