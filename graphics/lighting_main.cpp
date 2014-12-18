#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void init(void);
void display(void);
void reshape (int w, int h);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

void init(void) 
{
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	GLfloat light_position[] = {.8, .8, 1.0, 0.0};	//Last element: 0.0, no attenuation; otherwise, attenuation
	GLfloat diffuse_light_RGBA[] = {1.0, 0.0, 0.0, 1.0};
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

void display(void)
{
	float eyeX=1.0f, eyeY = 1.0f, eyeZ = 1.0f;
	float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
	float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Draw x, y, z axes
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();

	//void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks);
	//Renders a sphere centered at the modeling coordinates origin of the specified radius.
	//The sphere is subdivided around the Z axis into slices and along the Z axis into stacks. 
	//	radius: The radius of the sphere.
	//	slices: The number of subdivisions around the Z axis (similar to lines of longitude).
	//	stacks: The number of subdivisions along the Z axis (similar to lines of latitude). 
	glutSolidSphere(.5, 100, 100);

	glFlush ();
}

void reshape (int w, int h)
{
	//void glViewport(GLint x, GLint y, GLsizei width, GLsizei height); 
	//x, y: Specify the lower left corner of the viewport rectangle, in pixels. The initial value is (0,0). 
	//width, height: Specify the width and height of the viewport.
	//When a GL context is first attached to a window, width and height are set to the dimensions of that window. 
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


