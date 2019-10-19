#pragma once
#include <math.h> 

const float EMPTY_POINT_INTENSITY = -1;

struct PointFeatures
{
public:
  PointFeatures()
  {
    index = pos_x = pos_y = pos_z = normal_x = normal_y = normal_z = intensity = EMPTY_POINT_INTENSITY;
  }

  PointFeatures(int index, float pos_x, float pos_y, float pos_z, float normal_x, float normal_y, float normal_z, float intensity)
  {
	this->index = index;
	this->pos_x = pos_x;
	this->pos_y = pos_y;
	this->pos_z = pos_z;
	this->normal_x = normal_x;
	this->normal_y = normal_y;
	this->normal_z = normal_z;
    this->intensity = intensity;
  }

  float NormalDistanceTo(PointFeatures point) {
	  return (point.normal_x - normal_x) * (point.normal_x - normal_x) + (point.normal_y - normal_y) * (point.normal_y - normal_y) + (point.normal_z - normal_z) * (point.normal_z - normal_z);
  }

  float PositionDistanceTo(PointFeatures point) {
	  return sqrt((point.pos_x - pos_x) * (point.pos_x - pos_x) + (point.pos_y - pos_y) * (point.pos_y - pos_y) + (point.pos_z - pos_z) * (point.pos_z - pos_z));
  }

  float DepthDifference(PointFeatures point) {
	  float diff = point.pos_z - pos_z;
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
  float pos_x;
  float pos_y;
  float pos_z;
  float normal_x;
  float normal_y;
  float normal_z;
  float intensity;
};