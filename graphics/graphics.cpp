#include <GL/glew.h> // Include the GLEW header file
#include <GL/glut.h> // Include the GLUT header file
#include <math.h>
#include <stdio.h>

void setCamera(void);
void display(void);
void reshape(int win_width, int win_height);
void processRegularKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void init(void);
void DrawCubeWithTextureCoords (float fSize);
void DrawCubeFace(float fSize);
static int win_width = 500, win_height  = 500; //in pixels

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
	glutCreateWindow ("Graphics demo");
	
	
	// Tell GLUT to use the self-defined call-back function "display()" for rendering
	glutDisplayFunc(display);
	init();
	// Tell GLUT to use the self-defined call-back function "reshape()" for reshaping/redrawing
	glutReshapeFunc(reshape);

	// Tell GLUT to respond to key operations
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processRegularKeys);

	// Enter GLUT's main loop
	glutMainLoop();

	return 0;
}

void init(void) 
{
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	GLfloat light_position[] = {.8, .8, 1.0, 0.0};	//Last element: 0.0, no attenuation; otherwise, attenuation
	GLfloat diffuse_light_RGBA[] = {0.0, 9.0, 0.0, 1.0};
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);


	//An object's material properties determine how it reflects light and what material it is made of.
	//OpenGL binds the Phong lighting coefficients into materials:
	//       k_a (ambient), k_d (diffuse), k_s (specular), n_shiny (Phong exponent controlling the shininess)
	//glMaterialfv() sets the current material's ambient, diffuse, and specular colors and how shiny it is
	glMaterialfv(GL_FRONT, GL_COLOR_INDEXES, mat_specular);
	glMaterialfv(GL_FRONT, GL_COLOR_INDEXES, mat_shininess);

	//void glLightfv(GLenum light, GLenum pname, const GLfloat *params)
	//light: Specifies a light. The number of lights depends on the implementation, but at least eight lights are supported.
	//       They are identified by symbolic names of the form GL_LIGHT$i$ where 0 = $ i $ < GL_MAX_LIGHTS.
	//pname: specifies a light source parameter for light.
	//       Options: GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR, GL_POSITION, GL_SPOT_CUTOFF, GL_SPOT_DIRECTION, GL_SPOT_EXPONENT,
	//       GL_CONSTANT_ATTENUATION, GL_LINEAR_ATTENUATION, and GL_QUADRATIC_ATTENUATION
	
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light_RGBA);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void DrawCubeWithTextureCoords (float fSize)
{
  glPushMatrix();
  DrawCubeFace (fSize);
  glRotatef (90, 1, 0, 0);
  DrawCubeFace (fSize);
  glRotatef (90, 1, 0, 0);
  DrawCubeFace (fSize);
  glRotatef (90, 1, 0, 0);
  DrawCubeFace (fSize);
  glRotatef (90, 0, 1, 0);
  DrawCubeFace (fSize);
  glRotatef (180, 0, 1, 0);
  DrawCubeFace (fSize);
  glPopMatrix();
}

void DrawCubeFace(float fSize)
{
  fSize /= 2.0;
  glBegin(GL_QUADS);
  glVertex3f(-fSize, -fSize, fSize);    glTexCoord2f (0, 0);
  glVertex3f(fSize, -fSize, fSize);     glTexCoord2f (1, 0);
  glVertex3f(fSize, fSize, fSize);      glTexCoord2f (1, 1);
  glVertex3f(-fSize, fSize, fSize);     glTexCoord2f (0, 1);
  glEnd();
}

// the position of the camera/eye
//float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 1.0f;
static float eyeX = 1.0f, eyeY = 1.0f, eyeZ = 1.0f;

// the position of the reference point (0, 1, -1)
//float centerX = eyeX + 0.0f, centerY = 1.0f, centerZ = eyeZ - 1.0f;
static float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;

// the direction of the up vector (0, 1, 0)
static float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

static float rotationAngle = 0.0f;
static float translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;
static float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;

static GLdouble fov = 80.0;
static GLdouble aspect = win_width / win_height;
static GLdouble nearPlane = 1;
static GLdouble farPlane = 1000;

void setCamera()
{
	
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
	
}

void display(void)
{
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	glClearColor(255.0,255.0,255.0,0);
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
	//glBegin(GL_TRIANGLES);
	//glColor3f(1.0f, 0.0f, 1.0f);
	//set three vertices
	//glVertex3f(0.0f, 0.5f, 0.0f);
	//glVertex3f(0.0f, 0.8f, 0.0f);
	//glVertex3f(0.0f, 0.5f, 0.8f);
	//glEnd();

	//glColor3f(1.0f, 1.0f, 0.0f);
	//glutSolidSphere() function draws a shaded sphere centered at the origin
	//glutSolidSphere(GLdouble radius, GLint slices, GLint stacks)
	//radius: Sphere radius. 
	//slices: The number of divisions around the z axis. (latitudal) 
	//stacks: The number of divisions along the z axis. (longitudal) 
	//glutSolidSphere(.5, 20, 30);
	
	glutSolidTeapot(0.3);
	
	glColor3f(0.0, 8.0, 0.0);
  	glBegin(GL_LINES);
  	for (GLfloat i = -10.5; i <= 2.5; i += 0.25) {
    		glVertex3f(i, 10.5, 2.5); glVertex3f(i, 10.5, -2.5);
    		glVertex3f(2.5, 10.5, i); glVertex3f(-2.5, 10.5, i);
  	}
  	glEnd();

	glutSolidSphere(.5, 20, 30);
	
	
	//force execution of GL commands in finite time
	glFlush();
}

void processRegularKeys(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:             // ESCAPE key
    			exit (0);
    			break;
		case 'd':
			scaleX -= 0.1;
			scaleY -= 0.1;
			scaleZ -= 0.1;
			display();
			break;
		case 'r':
			eyeX = 1.0f; eyeY = 1.0f; eyeZ = 1.0f;
			centerX = 0.0f; centerY = 0.0f; centerZ = 0.0f;
			upX = 0.0f; upY = 1.0f; upZ = 0.0f;
			gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
			translateX = translateY = translateZ = 0.0;
			scaleX = scaleY = scaleZ = 1.0;
			rotationAngle = 0.0;
			display();
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
		case GLUT_KEY_LEFT:
			angle -= 0.01f;
			refMoveX -=10;			
			//refMoveX = sin(angle);
			//refMoveZ = cos(angle);
			//centerX += refMoveX, centerY = 0.0f, centerZ += refMoveZ;
			display();
			break;
		case GLUT_KEY_RIGHT:
			angle += 0.01f;
			refMoveX +=10;		
			//refMoveX = sin(angle);
			//refMoveZ = cos(angle);
			//centerX -= refMoveX, centerY = 0.0f, centerZ -= refMoveZ;
			display();
			break;
		//Move eye position
		//case GLUT_KEY_UP:
		//	eyeX += eyeMoveX;
		//	eyeZ += eyeMoveZ;
		//	display();
		//	break;
		//case GLUT_KEY_DOWN:
		//	eyeX -= eyeMoveX;
		//	eyeZ -= eyeMoveZ;
		//	display();
		//	break;
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
