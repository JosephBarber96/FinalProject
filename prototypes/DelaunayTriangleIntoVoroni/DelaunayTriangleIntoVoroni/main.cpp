#include "DelaunayTriangle.h"
#include "Point.h"
#include "V2.h"
#include "ScreenSegment.h"
#include <glut.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>

#define WIN_SIZE 500

DelaunayTriangle* tri;

// http://blog.ivank.net/fortunes-algorithm-and-implementation.html

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Segments
	glColor3f(.0f, .0f, 1.f);

	for (std::vector<ScreenSegment*> vec : tri->screenSegments)
	{
		for (ScreenSegment* ss : vec)
		{
			glBegin(GL_LINE_STRIP);
			glVertex2f(ss->minX, ss->minY);
			glVertex2f(ss->minX, ss->maxY);
			glVertex2f(ss->maxX, ss->maxY);
			glVertex2f(ss->maxX, ss->minY);
			glVertex2f(ss->minX, ss->minY);
			glEnd();
		}
	}

	// Lines
	glColor3f(1.f, .0f, .0f);

	for (std::vector<ScreenSegment*> vec : tri->screenSegments)
	{
		for (ScreenSegment* ss : vec)
		{
			for (Point* neighbour : ss->point->neighbours)
			{
				glBegin(GL_LINES);
				glVertex2f(ss->point->position->x, ss->point->position->y);
				glVertex2f(neighbour->position->x, neighbour->position->y);
				glEnd();
			}


		}
	}

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
	// Give a seed to rand()
	srand(time(NULL));

	int size = 4;

	// Create and generate a delaunay triangle
	tri = new DelaunayTriangle();
	tri->GenerateDelaunay(25, WIN_SIZE - 50, size, size, false);

	// Init OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIN_SIZE, WIN_SIZE);
	glutCreateWindow("Delaunay triangle.");
	InitGL();

	// Display
	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}