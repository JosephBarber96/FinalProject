#include <glut.h>

#define WIN_SIZE 500

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(5.0f);
	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_POINTS);
	glVertex2f(50.0f, 50.0f);
	glEnd();

	glutSwapBuffers();
	glutPostRedisplay();
}

void InitGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Matrix mode
	glMatrixMode(GL_PROJECTION);

	// Setup coordinate system
	gluOrtho2D(0, WIN_SIZE, 0, WIN_SIZE);
}

int main(int argc, char* argv[])
{
	// Init OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIN_SIZE, WIN_SIZE);
	glutCreateWindow("Minimum spanning tree.");
	InitGL();

	// Display
	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}