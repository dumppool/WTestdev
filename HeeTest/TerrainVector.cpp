#include "TerrainVector.h"

void TerrainVector::Set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}
TerrainVector* TerrainVector::cross(TerrainVector *v){
	float i=y*v->z-z*v->y;
	float j=z*v->x-x*v->z;
	float k=x*v->y-y*v->x;
	return new TerrainVector(i,j,k);
}
TerrainVector* TerrainVector::getNormalize(){
	float lengze=sqrtf(x*x+y*y+z*z);
	TerrainVector* normalized=new TerrainVector(x/lengze,y/lengze,z/lengze);
	return normalized;
}
void TerrainVector::normalize(){
	float lengze=sqrtf(x*x+y*y+z*z);
	x=x/lengze;
	y=y/lengze;
	z=z/lengze;
}
TerrainVector * TerrainVector::getRotatedAxis(float angle,TerrainVector *axis){
	if(angle==0.0)
		return new TerrainVector(x,y,z);

	TerrainVector* u=axis->getNormalize();

	TerrainVector* result=new TerrainVector();

	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	result->x  = (float)(cosTheta + (1 - cosTheta) * u->x * u->x) * x;
	result->x += (float)((1 - cosTheta) * u->x * u->y - u->z * sinTheta) * y;
	result->x += (float)((1 - cosTheta) * u->x * u->z + u->y * sinTheta) * z;

	result->y  = (float)((1 - cosTheta) * u->x * u->y + u->z * sinTheta) * x;
	result->y += (float)(cosTheta + (1 - cosTheta) * u->y * u->y) * y;
	result->y += (float)((1 - cosTheta) * u->y * u->z - u->x * sinTheta) * z;

	result->z  = (float)((1 - cosTheta) * u->x * u->z - u->y * sinTheta) * x;
	result->z += (float)((1 - cosTheta) * u->y * u->z + u->x * sinTheta) * y;
	result->z += (float)(cosTheta + (1 - cosTheta) * u->z * u->z) * z;

	return result;
}