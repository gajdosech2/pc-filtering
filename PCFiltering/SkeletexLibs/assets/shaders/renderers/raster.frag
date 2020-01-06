#version 430
#include "api_frag.glsl"

//TexCoord
layout (location = 0) in vec2 in_coord;

// Texture
layout (binding = 0) uniform sampler2D ras_tex;

/// SOURCE
void main()
{
	WriteFragment(texture(ras_tex, in_coord).rgb);
}
