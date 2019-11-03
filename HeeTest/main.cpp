#include "std.h"
#include "TerrainLattice.h"
#include "COGLTexture.h"
#include "TerrainCamera.h"
#include "TerrainSquare.h"

TerrainLattice* lattice;
TerrainCamera* camera;
TerrainSquare* square;

Image* heightMap;
float lastx=0.0f,lasty=0.0f;
float keyMoveDistence=2.0f;
float keyTurnAngle=2.0f;

bool isLattice;
int optimize;
float seamove;

GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 10.0, 10.0, 10.0, 1.0 };
GLfloat lm_ambient[] = { 0.3, 0.3, 0.3, 1.0 };

COGLTexture terrain,detail,skybox0,skybox1,skybox2,skybox3,skybox4,skybox5;

void renderSquares(){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	terrain.SetActive();
	glColor3f(1.0f,1.0f,1.0f);
	square->render();
}
void createSkyBox(){
	glEnable(GL_TEXTURE_2D);
	float cx=0.0f,cy=158.0f,cz=0.0f;
	float width=300.0f,height=300.0f,length=300.0f;
	float x = cx - width  / 2.0f;
	float y = cy - height / 2.0f;
	float z = cz - length / 2.0f;

	skybox0.SetActive();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,	y + height, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,	y,z);
	glEnd();

	skybox1.SetActive();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,	y,	z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,	y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y,	z + length);
	glEnd();

	skybox2.SetActive();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,	y,	z+length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,	y+height,	z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y+height,z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,z+length);
	glEnd();

	skybox3.SetActive();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z+length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y+height, z+length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width,	y + height,	z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width,	y,	z);
	glEnd();

	skybox4.SetActive();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width,y+height,z+length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y+height,z+length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y+height,z); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width,y+height,z);
	glEnd();

	skybox5.SetActive();
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f+seamove, 0.0f); glVertex3f(x,y,z+length);
	glTexCoord2f(3.0f+seamove, 0.0f); glVertex3f(x,y,z);
	glTexCoord2f(3.0f+seamove, 3.0f); glVertex3f(x+width,y,z); 
	glTexCoord2f(0.0f+seamove, 3.0f); glVertex3f(x+width,y,z+length);
	seamove+=0.001f;
	glEnd();
}

void renderTerrain(){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	if(isLattice==false){
		terrain.SetActive();
		for(int i=0;i<lattice->height-1;i++){
			glBegin(GL_TRIANGLE_STRIP);
			for(int j=0;j<lattice->width-1;j++){
				glTexCoord2f((float)j/(float)(lattice->width-1),(float)i/(float)(lattice->height-1));
				glNormal3f(lattice->lattice[i][j].vector->x,lattice->lattice[i][j].vector->y,lattice->lattice[i][j].vector->z);
				glVertex3f(lattice->lattice[i][j].x,lattice->lattice[i][j].y,lattice->lattice[i][j].z);
				glTexCoord2f((float)j/(float)(lattice->width-1),(float)(i+1)/(float)(lattice->height-1));
				glNormal3f(lattice->lattice[i+1][j].vector->x,lattice->lattice[i+1][j].vector->y,lattice->lattice[i+1][j].vector->z);
				glVertex3f(lattice->lattice[i+1][j].x,lattice->lattice[i+1][j].y,lattice->lattice[i+1][j].z);
			}
			glEnd();
		}
	}else{
		for(int i=0;i<lattice->height;i++){
			glBegin(GL_LINES);
			for(int j=0;j<lattice->width;j++){
				if(j+1<=lattice->width-1){
					glVertex3f(lattice->lattice[i][j].x,lattice->lattice[i][j].y,lattice->lattice[i][j].z);
					glVertex3f(lattice->lattice[i][j+1].x,lattice->lattice[i][j+1].y,lattice->lattice[i][j+1].z);
				}
				if(j-1>=0&&i+1<=lattice->height-1){
					glVertex3f(lattice->lattice[i][j].x,lattice->lattice[i][j].y,lattice->lattice[i][j].z);
					glVertex3f(lattice->lattice[i+1][j-1].x,lattice->lattice[i+1][j-1].y,lattice->lattice[i+1][j-1].z);
				}
				if(i+1<=lattice->height-1){
					glVertex3f(lattice->lattice[i][j].x,lattice->lattice[i][j].y,lattice->lattice[i][j].z);
					glVertex3f(lattice->lattice[i+1][j].x,lattice->lattice[i+1][j].y,lattice->lattice[i+1][j].z);
				}
			}
			glEnd();
		}
	}
}
void debugFunction(){
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(50.0f,0.0f,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,50.0f,0.0f);

	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,0.0f,50.0f);


	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(camera->point->x,camera->point->y,camera->point->z);
	glVertex3f(0.0f,0.0f,0.0f);
	glEnd();
}
void initTextures(void)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	terrain.LoadFromFile("terrain.bmp");
	detail.LoadFromFile("detail.bmp");
	skybox0.LoadFromFile("SkyBox/SkyBox0.bmp");
	skybox1.LoadFromFile("SkyBox/SkyBox1.bmp");
	skybox2.LoadFromFile("SkyBox/SkyBox2.bmp");
	skybox3.LoadFromFile("SkyBox/SkyBox3.bmp");
	skybox4.LoadFromFile("SkyBox/SkyBox4.bmp");
	skybox5.LoadFromFile("SkyBox/SkyBox5.bmp");
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
void resetPosition(){
	camera->reset();
}
void init(void)
{    
	heightMap=loadBMP("heightMap.bmp");
	lattice=new TerrainLattice(heightMap);
	camera=new TerrainCamera();
	square=new TerrainSquare(lattice,camera);
	isLattice=false;
	optimize=1;
	seamove=0.0f;
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glEnable(GL_CULL_FACE);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera->point->x,camera->point->y,camera->point->z,camera->view->x,camera->view->y,camera->view->z,camera->perpendicular->x,camera->perpendicular->y,camera->perpendicular->z);
	camera->update();
	glScalef(1.0f,1.0f,1.0f);

	//debugFunction();
	//<== createSkyBox();
	if(optimize>0){
		renderSquares();
	}else{
		renderTerrain();
	}
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 420.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	  case 'w':
		  camera->moveForward(keyMoveDistence);
		  break;
	  case 'W':
		  camera->moveForward(keyMoveDistence);
		  break;
	  case 'a':
		  camera->leftMove(keyMoveDistence);
		  break;
	  case 'A':
		  camera->rightMove(keyMoveDistence);
		  break;
	  case 's':
		  camera->moveBack(keyMoveDistence);
		  break;
	  case 'S':
		  camera->moveBack(keyMoveDistence);
		  break;
	  case 'd':
		  camera->rightMove(keyMoveDistence);
		  break;
	  case 'D':
		  camera->rightMove(keyMoveDistence);
		  break;
	  case 'g':
		  camera->reset();
		  break;
	  case 'G':
		  camera->reset();
		  break;
	  case 'z':
		  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		  break;
	  case 'Z':
		  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		  break;
	  case 27:
		  exit(0);
		  break;
	  case 'x':
		  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		  break;
	  case 'X':
		  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		  break;
	  case 32:
		  camera->point->y+=1.0f;
		  camera->view->y+=1.0f;
		  break;
	  case 'o':
		  optimize=-optimize;
		  break;
	  case 'O':
		  optimize=-optimize;
		  break;
	  default:
		  break;
	}
}
void mouseMovement(int x, int y) {
	int diffx=x-lastx; 
	int diffy=y-lasty; 
	lastx=x; 
	lasty=y;
	camera->setViewByMouse();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth,WindowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	initTextures();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouseMovement);
	glutMainLoop();
	return 0; 
}
