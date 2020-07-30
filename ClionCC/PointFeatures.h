#pragma once
#include <math.h> 

const float EMPTY_POINT = -1;

struct PointFeatures
{
public:

    PointFeatures()
    {
        index = pos_x = pos_y = pos_z = normal_x = normal_y = normal_z = intensity = coord_x = coord_y = EMPTY_POINT;
    };

  PointFeatures(int coord_x, int coord_y)
  {
    index = pos_x = pos_y = pos_z = normal_x = normal_y = normal_z = intensity = EMPTY_POINT;
    this->coord_x = coord_x;
    this->coord_y = coord_y;
  }

  PointFeatures(int index, float pos_x, float pos_y, float pos_z,
          float normal_x, float normal_y, float normal_z, float intensity,
          int coord_x, int coord_y)
  {
	this->index = index;
	this->pos_x = pos_x;
	this->pos_y = pos_y;
	this->pos_z = pos_z;
	this->normal_x = normal_x;
	this->normal_y = normal_y;
	this->normal_z = normal_z;
    this->intensity = intensity;
    this->coord_x = coord_x;
    this->coord_y = coord_y;
  }

  int index;
  float pos_x;
  float pos_y;
  float pos_z;
  float normal_x;
  float normal_y;
  float normal_z;
  float intensity;
  int coord_x;
  int coord_y;
};