#include <GL/glew.h> // Include the GLEW header file
#include <GL/glut.h> // Include the GLUT header file
#include <math.h>
#include <stdio.h>

void setCamera(void);
void display(void);
void reshape(int win_width, int win_height);
void processRegularKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

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
	glutCreateWindow ("My First OpenGL Window");

	// Tell GLUT to use the self-defined call-back function "display()" for rendering
	glutDisplayFunc(display);

	// Tell GLUT to use the self-defined call-back function "reshape()" for reshaping/redrawing
	glutReshapeFunc(reshape);

	// Tell GLUT to respond to key operations
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processRegularKeys);

	// Enter GLUT's main loop
	glutMainLoop();

	return 0;
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
	glColor3f(1.0f, 0.0f, 1.0f);
	//set three vertices
	glVertex3f(0.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, 0.8f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.8f);
	glEnd();

	glColor3f(1.0f, 1.0f, 0.0f);
	//glutSolidSphere() function draws a shaded sphere centered at the origin
	//glutSolidSphere(GLdouble radius, GLint slices, GLint stacks)
	//radius: Sphere radius. 
	//slices: The number of divisions around the z axis. (latitudal) 
	//stacks: The number of divisions along the z axis. (longitudal) 
	glutSolidSphere(.3, 20, 16);

	//Draw x, y, z axes
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.5f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.5f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.5f);
	glEnd();

	//force execution of GL commands in finite time
	glFlush();
}

void processRegularKeys(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 't':
			translateX += 0.05;
			translateY += 0.05;
			display();
			break;
		case 's':
			scaleX -= 0.1;
			scaleY -= 0.1;
			scaleZ -= 0.1;
			display();
			break;
		case 'r':
			rotationAngle += 10.0;
			display();
			break;
		case 'm':
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
			refMoveX = sin(angle);
			refMoveZ = cos(angle);
			centerX += refMoveX, centerY = 0.0f, centerZ += refMoveZ;
			display();
			break;
		case GLUT_KEY_RIGHT:
			angle += 0.01f;
			refMoveX = sin(angle);
			refMoveZ = cos(angle);
			centerX -= refMoveX, centerY = 0.0f, centerZ -= refMoveZ;
			display();
			break;
		//Move eye position
		case GLUT_KEY_UP:
			eyeX += eyeMoveX;
			eyeZ += eyeMoveZ;
			display();
			break;
		case GLUT_KEY_DOWN:
			eyeX -= eyeMoveX;
			eyeZ -= eyeMoveZ;
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
