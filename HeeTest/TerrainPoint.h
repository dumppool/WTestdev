#pragma once
#include "TerrainVector.h"
class TerrainPoint
{
public:
	float x,y,z;
	TerrainVector* vector;
	boolean enable;
public:
	TerrainPoint();
	TerrainPoint(float xx,float yy,float zz);
	TerrainPoint(float xx,float zz);
	~TerrainPoint(void);
	void setVector(TerrainVector* Tvector);
	TerrainVector* getNormal(TerrainPoint point2,TerrainPoint point3);
};
