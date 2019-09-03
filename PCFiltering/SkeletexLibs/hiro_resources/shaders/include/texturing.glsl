//! Returns texture coordinate aligned to the center of texel.
vec2 AlignToTexel(in vec2 coord, in float texel_size)
{
	return floor(coord / texel_size) * texel_size + 0.5 * texel_size;
}

//! Returns procedural checker texture parameter (0,1) for a given texture coordinate.
float GetTexelCheckerboardValue(in vec2 coord, in float resolution)
{
	int cx = int(coord.x * resolution);
  int cy = int(coord.y * resolution);
  return float((cx + cy) % 2);
}
