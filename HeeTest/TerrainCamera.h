#pragma once
#include "TerrainPoint.h"
#include "TerrainVector.h"

class TerrainCamera
{
public:
	TerrainPoint *point;
	TerrainPoint* view;
	TerrainVector* perpendicular;
	TerrainVector* strafe;
	TerrainCamera(void);
	TerrainCamera(float x,float y,float z);
	void reset();
	void moveForward(float distance);
	void moveBack(float distance);
	void leftMove(float distance);
	void rightMove(float distance);
	void update();
	void setViewByMouse();
	void rotateView(float angle, TerrainVector* axis);
	~TerrainCamera(void);
};
