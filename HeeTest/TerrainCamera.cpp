#include "TerrainCamera.h"

TerrainCamera::TerrainCamera(void)
{
	point=new TerrainPoint(0.0f,60.0f,60.0f);
	view=new TerrainPoint(0.0f,60.0f,0.0);
	perpendicular=new TerrainVector(0.0f,1.0f,0.0f);
	strafe=new TerrainVector(-1.0f,0.0f,0.0f);
	SetCursorPos(WindowWidth >> 1, WindowHeight >> 1);
	ShowCursor(FALSE);
}

TerrainCamera::TerrainCamera(float x,float y,float z)
{
	point=new TerrainPoint(x,y,z);
	view=new TerrainPoint(0.0f,y,0.0f);
	perpendicular=new TerrainVector(0.0f,1.0f,0.0f);
	TerrainVector* forwardDir=new TerrainVector(-x,0.0f,-z);
	strafe=perpendicular->cross(forwardDir);
	SetCursorPos(WindowWidth >> 1, WindowHeight >> 1);
	ShowCursor(FALSE);
}

void TerrainCamera::reset(){
	point=new TerrainPoint(0.0f,60.0f,60.0f);
	view=new TerrainPoint(0.0f,60.0f,0.0);
	perpendicular=new TerrainVector(0.0f,1.0f,0.0f);
	strafe=new TerrainVector(-1.0f,0.0f,0.0f);
	SetCursorPos(WindowWidth >> 1, WindowHeight >> 1);
	ShowCursor(FALSE);
}

TerrainCamera::~TerrainCamera(void)
{
}

void TerrainCamera::moveForward(float distance){
	TerrainVector* forwardDirection=new TerrainVector(view->x-point->x,view->y-point->y,view->z-point->z);
	forwardDirection->normalize();
	float length=sqrtf(distance);
	point->x=point->x+forwardDirection->x*length;
	point->y=point->y+forwardDirection->y*length;
	point->z=point->z+forwardDirection->z*length;

	view->x=view->x+forwardDirection->x*length;
	view->y=view->y+forwardDirection->y*length;
	view->z=view->z+forwardDirection->z*length;
}
void TerrainCamera::moveBack(float distance){
	TerrainVector* forwardDirection=new TerrainVector(view->x-point->x,view->y-point->y,view->z-point->z);
	forwardDirection->normalize();
	float length=sqrtf(distance);
	point->x=point->x-forwardDirection->x*length;
	point->y=point->y-forwardDirection->y*length;
	point->z=point->z-forwardDirection->z*length;

	view->x=view->x-forwardDirection->x*length;
	view->y=view->y-forwardDirection->y*length;
	view->z=view->z-forwardDirection->z*length;
}

void TerrainCamera::leftMove(float distance){
	float length=sqrtf(distance);
	view->x+=strafe->x*length;
	view->y+=strafe->y*length;
	view->z+=strafe->z*length;

	point->x+=strafe->x*length;
	point->y+=strafe->y*length;
	point->z+=strafe->z*length;
}
void TerrainCamera::rightMove(float distance){
	float length=sqrtf(distance);
	view->x-=strafe->x*length;
	view->y-=strafe->y*length;
	view->z-=strafe->z*length;

	point->x-=strafe->x*length;
	point->y-=strafe->y*length;
	point->z-=strafe->z*length;
}
void TerrainCamera::rotateView(float angle, TerrainVector* axis){
	TerrainVector* vNewView;
	TerrainVector* vView =new TerrainVector(view->x-point->x,view->y-point->y,view->z-point->z);	
	vNewView = vView->getRotatedAxis(angle, axis);
	view->x=point->x+vNewView->x;
	view->y=point->y+vNewView->y;
	view->z=point->z+vNewView->z;
}
void TerrainCamera::setViewByMouse(){
	POINT mousePos;
	int middleX = WindowWidth  >> 1;
	int middleY = WindowHeight >> 1;
	float angleY = 0.0f;
	float angleZ = 0.0f;	
	static float currentRotX = 0.0f;

	GetCursorPos(&mousePos);						

	if ((mousePos.x == middleX) && (mousePos.y == middleY))
	{
		return;
	}
	SetCursorPos(middleX, middleY);

	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	currentRotX -= angleZ;  
	if (currentRotX > 1.3f)
	{
		currentRotX = 1.3f;
	}
	else if (currentRotX < -1.3f)
	{
		currentRotX = -1.3f;
	}
	else
	{
		rotateView(-angleZ, strafe);
	}
	rotateView(angleY, new TerrainVector(0.0f,1.0f,0.0f));
	update();
}
void TerrainCamera::update(){
	TerrainVector* forwrdDirection=new TerrainVector(view->x-point->x,view->y-point->y,view->z-point->z);
	strafe=perpendicular->cross(forwrdDirection)->getNormalize();
}