#include <glut.h>
#include "Node.h"
#include "Vec2.h"
#include "UtilRandom.h"
#include "Road.h"
#include "RoadNetwork.h"

const int orthoSize = 100;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 1.0f, 0.0f);
	glPointSize(1.0f);

	// Draw the nodes
	for (auto node : Node::getNodes())
	{
		glBegin(GL_POINTS);
		glVertex2f(node->getPos()->getX(), node->getPos()->getY());
		glEnd();
	}

	glColor3f(0.0f, 0.0f, 1.0f);

	// Draw the roads
	for (auto road : Road::getRoads())
	{
		glBegin(GL_LINES);
		glVertex2f(road->parent->getPos()->getX(), road->parent->getPos()->getY());
		glVertex2f(road->child->getPos()->getX(), road->child->getPos()->getY());
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
		int x = UtilRandom::Instance()->Random(-orthoSize + 20, orthoSize - 20);
		int y = UtilRandom::Instance()->Random(-orthoSize + 20, orthoSize - 20);

		Node* node = new Node(x, y);
	}

	RoadNetwork network = RoadNetwork();
	network.Generate(new Node(0, -orthoSize + 5));


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