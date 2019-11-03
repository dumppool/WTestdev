#pragma once
#include "TerrainPoint.h"
#include "imageloader.h"
#include "TerrainCamera.h"

class TerrainLattice
{
public:
	int height,width;
	int bytesperrow;
	TerrainPoint** lattice;
	TerrainVector** vector;
public:
	TerrainLattice(int Lheight,int Lwidth);
	TerrainLattice(Image *image);
	~TerrainLattice(void);
	void setDepth(Image *image);
	void initVector();
	void initSmooth();
	float getDistanceFromCamera(TerrainCamera* camera,int row,int column);
	void reset();
};
