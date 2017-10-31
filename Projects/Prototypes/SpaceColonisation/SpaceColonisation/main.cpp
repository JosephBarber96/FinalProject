#include <glut.h>
#include "Node.h"
#include "UtilRandom.h"

const int orthoSize = 100;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 1.0f, 0.0f);
	glPointSize(1.0f);

	for (auto node : Node::nodes)
	{
		glBegin(GL_POINTS);
		glVertex2f(node->x, node->y);
		glEnd();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void InitGl()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// matrix mode
	glMatrixMode(GL_PROJECTION);

	gluOrtho2D(-orthoSize, orthoSize, -orthoSize, orthoSize);
}

int main(int argc, char* argv[])
{
	// Create nodes
	for (int i = 0; i < 100; i++)
	{
		int x = UtilRandom::Instance()->Random(-orthoSize, orthoSize);
		int y = UtilRandom::Instance()->Random(-orthoSize, orthoSize);

		Node* node = new Node(x, y);
	}


	// OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Space colonisation algorithm");
	InitGl();

	glutDisplayFunc(Display);
	glutMainLoop();

	system("PAUSE");
	return 0;
}