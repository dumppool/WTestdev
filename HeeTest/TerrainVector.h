#pragma once
#include "std.h"

class TerrainVector
{
public:
	float	x, y, z;
public:
	TerrainVector(){ Set(0, 0, 0); }
	TerrainVector(float _x, float _y, float _z)	{ Set(_x, _y, _z); }
	TerrainVector(TerrainVector &v){ Set(v.x, v.y, v.z); }

	void	Set(float _x, float _y, float _z);
	TerrainVector* getNormalize();
	void normalize();
	TerrainVector *cross(TerrainVector *v);
	TerrainVector *getRotatedAxis(float angle,TerrainVector *v);
};
