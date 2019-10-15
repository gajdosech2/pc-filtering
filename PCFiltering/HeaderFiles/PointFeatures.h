#pragma once

const float EMPTY_POINT_INTENSITY = -1;

struct PointFeatures
{
public:
  PointFeatures()
  {
    index = normal_x = normal_y = normal_z = intensity = depth = EMPTY_POINT_INTENSITY;
  }

  PointFeatures(int index, float normal_x, float normal_y, float normal_z, float intensity, float depth)
  {
	this->index = index;
	this->normal_x = normal_x;
	this->normal_y = normal_y;
	this->normal_z = normal_z;
    this->intensity = intensity;
    this->depth = depth;
  }

  float NormalDistanceTo(PointFeatures point) {
	  return (point.normal_x - normal_x) * (point.normal_x - normal_x) + (point.normal_y - normal_y) * (point.normal_y - normal_y) + (point.normal_z - normal_z) * (point.normal_z - normal_z);
  }

  float DepthDifference(PointFeatures point) {
	  float diff = point.depth - depth;
	  return diff;
  }

  float IntensityDifference(PointFeatures point) {
	  float diff = point.intensity - intensity;
	  if (diff < 0) {
		  return -diff;
	  }
	  return diff;
  }

  int index;
  float normal_x;
  float normal_y;
  float normal_z;
  float intensity;
  float depth;
};