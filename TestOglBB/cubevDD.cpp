/* cubev.c           */

/* E. Angel, Interactive Computer Graphics */
/* A Top-Down Approach with OpenGL, Third Edition */
/* Addison-Wesley Longman, 2003 */

/* rotating cube using vertex arrays */

#include <stdlib.h>
#include <GL/freeglut.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "HUBBLE.H"
#include "DC10.H"

GLfloat vertices[] = {-1.0,-1.0,-1.0,1.0,-1.0,-1.0,
	1.0,1.0,-1.0, -1.0,1.0,-1.0, -1.0,-1.0,1.0, 
	1.0,-1.0,1.0, 1.0,1.0,1.0, -1.0,1.0,1.0};

GLfloat colors[] = {0.0,0.0,0.0,1.0,0.0,0.0,
	1.0,1.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0, 
	1.0,0.0,1.0, 1.0,1.0,1.0, 0.0,1.0,1.0};

GLubyte cubeIndices[]={0,3,2,1,2,3,7,6,0,4,7,3,1,2,6,5,4,5,6,7,0,1,5,4};



static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 2;

void display(void)
{


/* display callback, clear frame buffer and z buffer,
   rotate cube and draw, swap buffers */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);
    glColorPointer(3,GL_FLOAT, 0, colors); 
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);



	glutSwapBuffers(); 
}

void spinCube()
{

/* Idle callback, spin cube 2 degrees about selected axis */

	theta[axis] += 0.05;
	if( theta[axis] > 360.0 ) theta[axis] -= 360.0;
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{

/* mouse callback, selects an axis about which to rotate */

	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-4.0, 4.0, -3.0 * (GLfloat) h / (GLfloat) w,
            5.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-4.0 * (GLfloat) w / (GLfloat) h,
            4.0 * (GLfloat) w / (GLfloat) h, -3.0, 5.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void CMy3DSglLoaderOnSize(  int cx, int cy)
{
	//CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
/////////////////////////////////////////////////////////////////
//添加窗口缩放时的图形变换函数
	glViewport(0, 0, cx, cy);
	/////////////////////////////////////////////////////////////////
	GLdouble aspect_ratio;
	aspect_ratio = (GLdouble)cx / (GLdouble)cy;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0F, aspect_ratio, 1.0F, 10000.0F);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
int Model(0), rotation(0);

void CMy3DSglLoaderViewInit(GLvoid)
{

	::glShadeModel(GL_SMOOTH);
	::glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
	::glClearDepth(1.0F);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDisable(GL_DITHER);

	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat lightPos[] = { 6000.0f,6000.0f,6000.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glMateriali(GL_FRONT, GL_SHININESS, 128);

	//Model = GL3DS_initialize_hubble();
	Model = GL3DS_initialize_DC10();
	if (Model == -1)
	{
		printf("Error initializing display list\n");
		exit(-1);
	}

	rotation = 0;
}


void CMy3DSglLoaderViewRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, -280.0);
	glRotatef(rotation, 0.0, 1.0, 0.0);
	glPushMatrix();
	glCallList(Model);
	rotation = (rotation + 1) % 360;
	glPopMatrix();
	glutSwapBuffers();		//交互缓冲区
	//return 1;
}

void
main(int argc, char **argv)
{


/* need both double buffering and z buffer */

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200,800);



    glutCreateWindow("colorcube");

	CMy3DSglLoaderViewInit();
    //glutReshapeFunc(myReshape);
    //glutDisplayFunc(display);

    glutReshapeFunc(CMy3DSglLoaderOnSize);
	glutDisplayFunc(CMy3DSglLoaderViewRenderScene);

    //glutIdleFunc(spinCube);
    glutMouseFunc(mouse);
    glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */
 	//glEnableClientState(GL_COLOR_ARRAY); 
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, vertices);
    //glColorPointer(3,GL_FLOAT, 0, colors); 
	//glClearColor(1.0,1.0,1.0,1.0);
	//glColor3f(1.0,1.0,1.0);
    glutMainLoop();
}
