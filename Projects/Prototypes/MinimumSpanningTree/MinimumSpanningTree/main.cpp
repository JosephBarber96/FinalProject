#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>

#include <glut.h>

#include "Edge.h"
#include "MinimumSpanningTree.h"
#include "Node.h"
#include "V2.h"

#define WIN_SIZE 500

MinimumSpanningTree* mst;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw all edges
	glColor3f(0.0, 1.0, 0.0);
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
	glLineWidth(2.0);
	for (Edge* edge : mst->GetTreeEdges())
	{
		glBegin(GL_LINE_STRIP);
		glVertex2f(edge->start->position->x, edge->start->position->y);
		glVertex2f(edge->end->position->x, edge->end->position->y);
		glEnd();
	}

	// Draw nodes
	glPointSize(5.0f);
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
	// Minimum spanning tree
	mst = new MinimumSpanningTree;

	// Seed random
	srand(time(0));

	// Plot points
	int numOfPoints = 10;
	for (int i = 0; i < numOfPoints; i++)
	{
		int nodeX = rand() % WIN_SIZE;
		int nodeY = rand() % WIN_SIZE;

		mst->AddNode(new Node(nodeX, nodeY));
	}

	for (Node* node : mst->GetNodes())
	{
		std::cout << "Node pos: " << node->position->x << ", " << node->position->y << std::endl;
	}

	// Assign neighbours
	int loopCounter = 0;
	for (Node* node : mst->GetNodes())
	{
		loopCounter++;
		bool neighbourFoundForNode = false;
		Node* closestNeighbour = nullptr;

		for (Node* possibleNeighbour : mst->GetNodes())
		{
			// Skip itself
			if (node->position == possibleNeighbour->position)
			{
				continue;
			}

			// Assign a closest neighbour if the closest neighbour is full
			if (closestNeighbour == nullptr)
			{
				closestNeighbour = possibleNeighbour;
			}

			// If the possibleNeighbour is closer than closest, replace it
			if (V2::DistanceBetween(*node->position, *possibleNeighbour->position) < V2::DistanceBetween(*node->position, *closestNeighbour->position))
			{
				closestNeighbour = possibleNeighbour;
			}

			if (V2::DistanceBetween(*node->position, *possibleNeighbour->position) < WIN_SIZE / 2)
			{
				node->AddPossibleNeighbour(possibleNeighbour);
			}
		}

		// If after cycling through all neighbours a close enough neighbour has not been found, add the closest neighbour
		if (node->getPossibleNeighbours().empty())
		{
			node->AddPossibleNeighbour(closestNeighbour);
		}
	}


	// Create edges
	for (Node* node : mst->GetNodes())
	{
		for (Node* neighbour : node->getPossibleNeighbours())
		{
			// Create an edge between the node and a neighbour
			Edge* edge = new Edge(node, neighbour);

			// If allEdges does not contain this edge, add it
			if (mst->GetAllEdges().empty())
			{
				mst->AddEdge(edge);
				continue;
			} 
			else
			{
				bool edgeExistsInList = false;
				for (Edge* edgeInList : mst->GetAllEdges())
				{
					if (*edgeInList == *edge)
					{
						edgeExistsInList = true;
					}
				}

				if (!edgeExistsInList)
				{
					mst->AddEdge(edge);
				}
			}
		}
	}

	// Output all edges
	int edgeCounter = 0;
	for (Edge* edge : mst->GetAllEdges())
	{
		std::cout << "Edge " << ++edgeCounter << " - Start: (" << edge->start->position->x << ", " << edge->start->position->y << "), End(" << edge->end->position->x << ", " << edge->end->position->y << ")" << std::endl;
	}

	mst->Attempt();



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