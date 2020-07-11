#pragma once
#include <math.h> 

const float EMPTY_POINT = -1;

struct PointFeatures
{
public:
  PointFeatures()
  {
    index = pos_x = pos_y = pos_z = normal_x = normal_y = normal_z = intensity = EMPTY_POINT;
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

  int index;
  float pos_x;
  float pos_y;
  float pos_z;
  float normal_x;
  float normal_y;
  float normal_z;
  float intensity;
};