#include "TerrainPoint.h"

TerrainPoint::TerrainPoint(){
	x=0;
	y=0;
	z=0;
	enable=true;
	vector=new TerrainVector();
}

TerrainPoint::TerrainPoint(float xx,float yy,float zz){
	x=xx;
	y=yy;
	z=zz;
	enable=true;
	vector=new TerrainVector();
}

TerrainPoint::TerrainPoint(float xx, float zz){
	x=xx;
	y=0;
	z=zz;
	enable=true;
	vector=new TerrainVector();
}

TerrainPoint::~TerrainPoint(void)
{
}

void TerrainPoint::setVector(TerrainVector *Tvector){
	vector->Set(Tvector->x,Tvector->y,Tvector->z);
}

TerrainVector* TerrainPoint::getNormal(TerrainPoint point2,TerrainPoint point3){
	float i=(y-point2.y)*(z-point3.z)-(y-point3.y)*(z-point2.z);
	float j=(x-point3.x)*(z-point2.z)-(x-point2.x)*(z-point3.z);
	float k=(x-point2.x)*(y-point3.y)-(x-point3.x)*(z-point2.z);

	return new TerrainVector(i,j,k);
}