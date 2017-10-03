#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>

#include <glut.h>

#include "Defines.h"
#include "Edge.h"
#include "MinimumSpanningTree.h"
#include "Node.h"
#include "V2.h"

MinimumSpanningTree* mst;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw all edges
	glColor4f(0.8, 1.0, 0.8, 0.1);
	glLineWidth(1.0);
	for (Edge* edge : mst->GetAllEdges())
	{
		glBegin(GL_LINE_STRIP);
		glVertex2f(edge->start->position->x, edge->start->position->y);
		glVertex2f(edge->end->position->x, edge->end->position->y);
		glEnd();
	}

	// Draw only minimum spanning graph
	glColor3f(0.0, 0.0, 1.0);
	glLineWidth(5.0);
	for (Edge edge : mst->GetTreeEdges())
	{
		glBegin(GL_LINE_STRIP);
		glVertex2f(edge.start->position->x, edge.start->position->y);
		glVertex2f(edge.end->position->x, edge.end->position->y);
		glEnd();
	}

	// Draw nodes
	glPointSize(6.0f);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (Node* node : mst->GetNodes())
	{
		glVertex2f(node->position->x, node->position->y);
	}
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
	// Seed random
	srand(time(0));

	// Minimum spanning tree
	mst = new MinimumSpanningTree;

	// Plot points
	int numOfPoints = 29;
	mst->SpawnPoints(numOfPoints, 0, 0, WIN_SIZE, WIN_SIZE);

	// Assign neighbours
	float maxDist = WIN_SIZE / 2;
	mst->AssignNighbours(maxDist);

	// Edges
	mst->CreateAllEdges();

	// Sort the MST
	mst->Sort();

	std::cout << "The complete MST has " << mst->GetTreeEdges().size() << " edges.";

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