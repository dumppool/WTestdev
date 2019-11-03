#pragma once
#include "TerrainPoint.h"
#include "TerrainLattice.h"

#define QT_LR_NODE	0
#define QT_LL_NODE	1
#define QT_UL_NODE	2
#define QT_UR_NODE	3

#define QT_COMPLETE_FAN	0
#define QT_LL_UR	5
#define QT_LR_UL	10
#define QT_NO_FAN	15

class TerrainSquare
{
public:
	bool matrix[257][257];
	float Rmatrix[257][257];
	float Hmatrix[257][257];
	TerrainLattice* lattice;
	TerrainCamera* camera;
	float MinResolution;
	float DesiredResolution;
	float m_iTrisPerFrame;
public:
	TerrainSquare(TerrainLattice* lattice,TerrainCamera* camera);
	~TerrainSquare(void);
	void init();
	void shutdown();
	void render();
	void renderNode(int xOff,int zOff,int EdgeLength);
	void update();
	void refineNode(int xOff,int zOff,int EdgeLength);
	void roughness();
	float GetHeight(int x,int y);
	bool GetMatrixData(int x,int y);
	void RenderVertex(int x,int z);
};
