#ifndef STD_HEAD_FILE_
#define STD_HEAD_FILE_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include "c24bitmap.h"
//#include <GL/glaux.h>
//#include <Windows.h>
typedef bool boolean;
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define FALSE false
#define TURE true
#include <vector>
using namespace std;

typedef struct _AUX_RGBImageRec {
    GLint sizeX,sizeY;
    unsigned char *data;
	vector<unsigned char> data_vec;
} AUX_RGBImageRec;

/*typedef struct _POINT
{
	int x,y;
}POINT;

bool inline GetCursorPos(
  POINT* lpPoint
){return 1;};

bool inline SetCursorPos(
  POINT* lpPoint
){return 1;};
 
bool inline SetCursorPos(
  int xx,int yy
){return 1;};

void inline ShowCursor(bool val){};
*/

inline AUX_RGBImageRec * auxDIBImageLoadA(const char* filename)
{ 
	AUX_RGBImageRec *ImgPt = new AUX_RGBImageRec;
	C24BitMap Pic,PicM;
	Pic.Load((char*)filename);
	PicM.Load((char*)filename);
	ImgPt->sizeX = Pic.Width;
	ImgPt->sizeY = Pic.Height;
	ImgPt->data_vec.resize(Pic.LineWidth*Pic.Height);
	ImgPt->data = &ImgPt->data_vec[0];
	//memcpy(ImgPt->data, Pic.Buffer, Pic.LineWidth*Pic.Height);
	int i, j;
	Loopi(Pic.Width)
	  Loopj(Pic.Height)
	 {
		C24PixVal Px1 = get_pix_color(Pic , i, j);
		C24PixVal Px2 = get_pix_color(PicM, i, j);
		*Px2.r = *Px1.b;
		*Px2.b = *Px1.r;
	 }
	memcpy(ImgPt->data, PicM.Buffer, Pic.LineWidth*Pic.Height);
	return ImgPt;
};

#define  sqr( X )	( (X) * (X) )
#define  PI		3.14159265358979323846f
#define  WindowWidth 1200
#define  WindowHeight 800
#define  GL_CLAMP_TO_EDGE 0x812F

#endif
