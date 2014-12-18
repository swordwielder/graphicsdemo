#include <GL/glew.h> // Include the GLEW header file
#include <GL/glut.h> // Include the GLUT header file
#include <math.h>
#include <stdio.h>
#include <iostream>


using namespace std;
void setCamera(void);
void display(void);
void reshape(int win_width, int win_height);
void init(void);
void processRegularKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void bounceBall();

static int win_width = 800, win_height  = 800; //in pixels
typedef int BOOL;
#define TRUE 1
#define FALSE 0

//#define VIEWING_DISTANCE_MIN  3.0
int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);

	// Set up a basic (single) display buffer
	glutInitDisplayMode(GLUT_SINGLE);

	// Set the position of the drawing window on the display screen with the origin (0, 0) at the top-left corner
	glutInitWindowPosition(100, 100);
	// Set the width and height of the 2D drawing window on the display screen
	glutInitWindowSize(win_width, win_height);

	// Set the title for the drawing window
	glutCreateWindow ("Bouncing ball demo"); // Qi Chen & Andrew DiMotta

	init();
	// Tell GLUT to use the self-defined call-back function "display()" for rendering
	glutDisplayFunc(display);

	// Tell GLUT to use the self-defined call-back function "reshape()" for reshaping/redrawing
	glutReshapeFunc(reshape);

	// Tell GLUT to respond to key operations
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processRegularKeys);
	
	glutMouseFunc (MouseButton);
  	glutMotionFunc (MouseMotion);
	//glutWarpPointer(MouseButton);
		
	
	glutIdleFunc(display);
	// Enter GLUT's main loop
	glutMainLoop();

	return 0;
}

void init(void) 
{
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {90.0};
	GLfloat light_position[] = {70.8, 50.8, 7.0, 0.0};	//Last element: 0.0, no attenuation; otherwise, attenuation
	GLfloat diffuse_light_RGBA[] = {124.0, 252.0, 0.0, 1.0};
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);


	//An object's material properties determine how it reflects light and what material it is made of.
	//OpenGL binds the Phong lighting coefficients into materials:
	//       k_a (ambient), k_d (diffuse), k_s (specular), n_shiny (Phong exponent controlling the shininess)
	//glMaterialfv() sets the current material's ambient, diffuse, and specular colors and how shiny it is
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//void glLightfv(GLenum light, GLenum pname, const GLfloat *params)
	//light: Specifies a light. The number of lights depends on the implementation, but at least eight lights are supported.
	//       They are identified by symbolic names of the form GL_LIGHT$i$ where 0 = $ i $ < GL_MAX_LIGHTS.
	//pname: specifies a light source parameter for light.
	//       Options: GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR, GL_POSITION, GL_SPOT_CUTOFF, GL_SPOT_DIRECTION, GL_SPOT_EXPONENT,
	//       GL_CONSTANT_ATTENUATION, GL_LINEAR_ATTENUATION, and GL_QUADRATIC_ATTENUATION
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light_RGBA);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

// the position of the camera/eye
//float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 1.0f;
static float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 0.0f;
//gluLookAt(0,0,0,0,2,5,0,1,0);
// the position of the reference point (0, 1, -1)
//float centerX = eyeX + 0.0f, centerY = 1.0f, centerZ = eyeZ - 1.0f;
static float centerX = 0.0f, centerY = 2.0f, centerZ = 5.0f;

// the direction of the up vector (0, 1, 0)
static float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

static float rotationAngle = 0.0f;
static float translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;
static float scaleX = 0.5f, scaleY = 0.5f, scaleZ = 0.5f;

static GLdouble fov = 80.0;
static GLdouble aspect = win_width / win_height;
static GLdouble nearPlane = 1;
static GLdouble farPlane = 1000;
static BOOL g_bButton1Down = FALSE;

static float rotatepot = 0.0f;
static float ballY = 2.0;
static float ballX = 0.0;
static float potX = -1.0;
static float potY = -0.25;

static float xpos=0.0;
static float ypos=0.0;

GLuint _textureId;
//static int g_yClick = 0;

void setCamera()
{
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//void glViewport(GLint x, GLint y, GLsizei width, GLsizei height); 
	//x, y: Specify the lower left corner of the viewport rectangle, in pixels. The initial value is (0,0). 
	//width, height: Specify the width and height of the viewport.
	//When a GL context is first attached to a window, width and height are set to the dimensions of that window. 
	glViewport(0, 0, win_width, win_height);

	// Set camera position
	// To operate on Model-View matrix
	// Model Matrix: contains translations, rotations or scaling, applied to an object.
	// View Matrix: controls the way we look at a scene, simulating a moving camera (lookAt).
	//              It multiplies the model matrix and, basically aligns the world (the objects from a scene) to the camera.
	glMatrixMode(GL_MODELVIEW);
	// Reset the model-view matrix
	glLoadIdentity();

	//gluLookAt() creates a viewing matrix derived from an eye point, a reference point indicating the center of the scene, and an UP vector
	//eyeX, eyeY, eyeZ:				the position of the eye point. 
	//centerX, centerY, centerZ:	the position of the reference point. 
	//upX, upY, upZ:				the direction of the camera's up vector (decide the orientation of the camera).
	//By default, the eye is set on the Z axis
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	//gluLookAt(0,0,0,0,2,5,0,1,0);
	//Projection Matrix (orthographic or perspective): used to enlarge or shrink the clipping volume
	//		It multiplies the product of view matrix and model matrix and, basically projects the world coordinates to the unit cube.
	//		By default, it uses orthographic projection.
	// Set projection frustum
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	//fovy:		the field of view angle, in degrees, in the y direction. 
	//aspect:	the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height). 
	//zNear:	the distance from the viewer to the near clipping plane (always positive). 
	//zFar:		the distance from the viewer to the far clipping plane (always positive). 
	//gluPerspective(fov, aspect, nearPlane, farPlane);
}

void display(void)
{
	float colorBronzeDiff[4] = { 0.8, 0.6, 0.0, 1.0 };
  	float colorBronzeSpec[4] = { 1.0, 1.0, 0.4, 1.0 };
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	glClearColor(0.0,191.0, 255.0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	setCamera();

	//rotate rotationAngle degrees w.r.t. axis Z
	//multiply the current matrix by a rotation matrix
	glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);

	//translate by translateX, translateY, translateZ
	//multiply the current matrix by a translation matrix
	glTranslatef(translateX, translateY, translateZ);

	//scale by scaleX, scaleY, scaleZ
	//multiply the current matrix by a scale matrix
	glScalef(scaleX, scaleY, scaleZ);

	//Draw a triangle
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 0.0f);
	//set three vertices
	glVertex3f(0.0f, 0.5f, 0.0f);
	glEnd();
	
	//glColor3f(1.0f, 1.0f, 0.0f);
	//glutSolidSphere() function draws a shaded sphere centered at the origin
	//glutSolidSphere(GLdouble radius, GLint slices, GLint stacks)
	//radius: Sphere radius. 
	//slices: The number of divisions around the z axis. (latitudal) 
	//stacks: The number of divisions along the z axis. (longitudal) 
	//glutSolidSphere(.3, 20, 16);

	glBegin(GL_QUADS);
	//glColor3f(0.0f, 250.0f, 154.0f);
	glVertex3f( 2.0f,  1.0f, 2.0f); // Upper Left

	glVertex3f( -2.0f,  1.0f, 2.0f); // Upper Right

	glVertex3f( -3.0f,   0.1f, 3.0f); // Lower Right

	glVertex3f( 3.0f,   0.1f, 3.0f); // Lower Left
	glEnd();

	
	glPushMatrix();
	glTranslatef(potX,potY,0.0);
	glRotatef(rotatepot, 0.0, rotatepot, 1.0f);	
	glColor3f(0.0f, 0.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBronzeDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorBronzeSpec);
  	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
  	glColor4fv(colorBronzeDiff);
  	glBindTexture(GL_TEXTURE_2D, 0);
  	glColor4fv(colorBronzeDiff);
	glutSolidTeapot(0.3);
	//glColor3f(0.0f, 1.0f, 0.0f);
	glPopMatrix();


	//Image* img = loadBMP("sky.bmp");
	//_textureId = loadTextureFromImage(img);

	glBindTexture(GL_TEXTURE_2D, _textureId);
	
	glPushMatrix();
	//glMaterialfv(GL_FRONT, GL_SPECULAR, {80});
	glColor3f(1.0f, 10.0f, 0.0f);
	glTranslatef(ballX , ballY , 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBronzeDiff);
  	glMaterialfv(GL_FRONT, GL_SPECULAR, colorBronzeSpec);
  	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
  	glColor4fv(colorBronzeDiff);
	glutSolidSphere(.5, 20, 30);
	bounceBall();
	glColor3f(1.0f, 10.0f, 0.0f);
	
	glPopMatrix();
	
		

	
	//force execution of GL commands in finite time
	glFlush();
}

BOOL bounceup = FALSE;
BOOL limit = FALSE;
float he = ballY;
void bounceBall()
{
	if (limit == TRUE){
	if (bounceup)
	{
		ballY += .009;
		//he -= 0.1;
		if (ballY >= he){
			he = he/1.2;
			bounceup = FALSE;
			
		}
		
	}
	if (!bounceup)
	{
		ballY -= .009;
		if (ballY <= 0)
			//he = he/2;
			bounceup = TRUE;
	}
	}
	
}



void MouseButton(int button, int state, int x, int y)
{
	xpos = (float)x; ///(float)(win_height);
	ypos = (float)y; ///(float)(win_width);
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.
  if (button == GLUT_LEFT_BUTTON)
    {
	
	
	if ( xpos >= 400 && ypos >= 400){
		potX -= 0.02;
		potY -= 0.02;
		ballX -= 0.02;
		ballY -= 0.02;	
	}else if (xpos >= 400 && ypos <= 400){
		potX -= 0.02;
		potY += 0.02;
		ballX -= 0.02;
		ballY += 0.02;
	}else if (xpos <= 400 && ypos <= 400){
		potX += 0.02;
		potY += 0.02;
		ballX += 0.02;
		ballY += 0.02;
		//if (xpos == potX || ypos == potY){
		//}
		
	}else{
		potX += 0.02;
		potY -= 0.02;
		ballX += 0.02;
		ballY -= 0.02;	
		
	}

	
      g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
      cout << "x is :  " + x;

	
    }
}


void MouseMotion(int x, int y)
{

	
  // If button1 pressed, zoom in/out if mouse is moved up/down.
  if (g_bButton1Down)
    {
	
      //g_fViewDistance = (y - g_yClick) / 3.0;
      //if (g_fViewDistance < VIEWING_DISTANCE_MIN)
         //g_fViewDistance = VIEWING_DISTANCE_MIN;
		
      //glutSolidTeapot(0.3);
      glutPostRedisplay();
    }
}

void processRegularKeys(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:             // ESCAPE key
    			exit (0);
    			break;
		case 'd':
			limit = TRUE;
			//rotationAngle += 10.0;
			display();
			break;
		case 'r':
			
			eyeX = 0.0f; eyeY = 0.0f; eyeZ = 0.0f;
			centerX = 0.0f; centerY = 2.0f; centerZ = 5.0f;
			upX = 0.0f; upY = 1.0f; upZ = 0.0f;
			//gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
			translateX = translateY = translateZ = 0.0;
			scaleX = scaleY = scaleZ = 0.5;
			ballY = 2.0;
			ballX = 0.0;
			limit = FALSE;
			rotationAngle = 0.0;
			rotatepot = 0.0;
			potX = -1.0;
			potY = -0.25;
			he = 2.0;
			display();
			break;
		case 'a':
			eyeX -= 0.1f; eyeZ -= 0.1f;
			break;
		case 's':
			eyeX += 0.1f; eyeZ += 0.1f;
			break;
		case 'x':
			scaleX += 0.03; scaleY+=0.03; scaleZ+=0.03;
			break;
		case 'w':
			scaleX -= 0.03; scaleY-=0.03; scaleZ-=0.03;
			break;
	}	
}

//the angle of rotation in degrees for the camera's direction
static float angle = 0.0;
static float refMoveX = 0.0, refMoveZ = 0.0;
static float eyeMoveX = 0.1, eyeMoveZ = 0.1;

void processSpecialKeys(int key, int x, int y)
{
	switch(key)
	{
		//Move reference point
		//Move eye position
		case GLUT_KEY_LEFT:
			rotatepot -= 3.0;
			display();
			break;
		case GLUT_KEY_RIGHT:
			rotatepot += 3.0;
			display();
			break;
		
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w, 1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5*(GLfloat)w/(GLfloat)h, 1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

