// CubeDX.c
// OpenGL SuperBible, Chapter 9
// Demonstrates rendering with Indexed Vertex Arrays
// Program by Richard S. Wright Jr.

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MaxNumPts 64
float PointArray[MaxNumPts][2];
int NumPts = 0;
// Window size in pixels
int WindowHeight;
int WindowWidth;


// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void addNewPoint(float x, float y) {
	printf("%.2f,%.2f, %i,%i\n", x, y, WindowWidth, WindowHeight);
	if (NumPts >= MaxNumPts) {
		//removeFirstPoint();
		NumPts = 0;
	}
	PointArray[NumPts][0] = x * 40;
	PointArray[NumPts][1] = y * 40;
	NumPts++;
}

// Array containing the six vertices of the cube
static GLfloat corners[] = { -25.0f, 25.0f, 25.0f, // 0 // Front of cube
							  25.0f, 25.0f, 25.0f, // 1
							  25.0f, -25.0f, 25.0f,// 2
							 -25.0f, -25.0f, 25.0f,// 3
							 -25.0f, 25.0f, -25.0f,// 4  // Back of cube
							  25.0f, 25.0f, -25.0f,// 5
							  25.0f, -25.0f, -25.0f,// 6
							 -25.0f, -25.0f, -25.0f };// 7

// Array of indexes to create the cube
static GLubyte indexes[] = { 0, 1, 2, 3,	// Front Face
							 4, 5, 1, 0,	// Top Face
							 3, 2, 6, 7,	// Bottom Face
							 5, 4, 7, 6,	// Back Face
							 1, 5, 6, 2,	// Right Face
							 4, 0, 3, 7 };	// Left Face

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void initRendering() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Make big points and wide lines.  (This may be commented out if desired.)
	glPointSize(8);
	glLineWidth(5);

	// The following commands should induce OpenGL to create round points and 
	//	antialias points and lines.  (This is implementation dependent unfortunately, and
	//  may slow down rendering considerably.)
	//  You may comment these out if you wish.
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
{
	float length;

	// Calculate the length of the vector       
	length = (float)sqrt((vector[0] * vector[0]) +
		(vector[1] * vector[1]) +
		(vector[2] * vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if (length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
{
	float v1[3], v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y] * v2[z] - v1[z] * v2[y];
	out[y] = v1[z] * v2[x] - v1[x] * v2[z];
	out[z] = v1[x] * v2[y] - v1[y] * v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}

void displayLines(void)
{
	int i;

	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the line segments
	glColor3f(1.0f, 0.0f, 0.8f);			// Reddish/purple lines
	if (NumPts > 1) {
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < NumPts; i++) {
			glVertex2f(PointArray[i][0], PointArray[i][1]);
		}
		glEnd();
	}

	// Draw the interpolated points second.
	glColor3f(0.0f, 0.0f, 0.0f);			// Draw points in black
	glBegin(GL_POINTS);
	for (i = 0; i < NumPts; i++) {
		glVertex2f(PointArray[i][0], PointArray[i][1]);
	}
	glEnd();

	//################################################################
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Save the matrix state
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Rotate about x and y axes
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 0.0f, 1.0f);

	// Enable and specify the vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, corners);

	glColor3f(0.0f, 0.3f, 0.1f);
	// Using Drawarrays
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indexes);


	glPopMatrix();
	//####################################################################

	glFlush();
}

/*

*/

// Called to draw scene
void RenderScene(void)
{
	displayLines( );
	return;
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Make the cube a wire frame
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// Save the matrix state
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Rotate about x and y axes
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 0.0f, 1.0f);

	// Enable and specify the vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, corners);

	// Using Drawarrays
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indexes);


	glPopMatrix();

	// Swap buffers
	glutSwapBuffers();
}


// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Black background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glColor3ub(0, 0, 255);
}
////---------------------------------------------------------------------------
// Left button presses place a control point.
void myMouseFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float xPos = ((float)x) / ((float)(WindowWidth - 1));
		float yPos = ((float)y) / ((float)(WindowHeight - 1));

		yPos = 1.0f - yPos;			// Flip value since y position is from top row.

		addNewPoint(xPos, yPos);
		glutPostRedisplay();
	}
}
///////////////////////////////////////////////////////////////////////////////
// Process arrow keys
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;

	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if (key > 356.0f)
		xRot = 0.0f;

	if (key < -1.0f)
		xRot = 355.0f;

	if (key > 356.0f)
		yRot = 0.0f;

	if (key < -1.0f)
		yRot = 355.0f;

	// Refresh the Window
	glutPostRedisplay();
}

void resizeWindow(int w, int h)
{
	WindowHeight = (h > 1) ? h : 2;
	WindowWidth = (w > 1) ? w : 2;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);  // Always view [0,1]x[0,1].
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void ChangeSize(int w, int h)
{
	WindowHeight = (h > 1) ? h : 2;
	WindowWidth = (w > 1) ? w : 2;
	GLfloat nRange = 100.0f;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange*h / w, -nRange * 2.0f, nRange*2.0f);
	else
		glOrtho(-nRange * w / h, nRange*w / h, -nRange, nRange, -nRange * 2.0f, nRange*2.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE | GLUT_RGB );
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	initRendering();

	glutDisplayFunc(displayLines);
	glutReshapeFunc(ChangeSize);// resizeWindow);
	//glutKeyboardFunc(myKeyboardFunc);
	glutSpecialFunc(SpecialKeys);
	glutMouseFunc(myMouseFunc);
	glutMainLoop();
	/*glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	initRendering();
	glutCreateWindow("Cube DX");
	glutReshapeFunc(resizeWindow);// ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	glutMouseFunc(myMouseFunc);

	SetupRC();

	glutMainLoop();*/
	return 0;
}
