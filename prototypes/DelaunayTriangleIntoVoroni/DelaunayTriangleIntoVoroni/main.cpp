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
	glColor3f(.0f, .0f, 1.0f);

	for (int y = 0; y < tri->columns; y++)
	{
		for (int x = 0; x < tri->rows; x++)
		{
			glBegin(GL_LINE_STRIP);
			glVertex2f(tri->screenSegments[y][x]->minX, tri->screenSegments[y][x]->minY);
			glVertex2f(tri->screenSegments[y][x]->minX, tri->screenSegments[y][x]->maxY);
			glVertex2f(tri->screenSegments[y][x]->maxX, tri->screenSegments[y][x]->maxY);
			glVertex2f(tri->screenSegments[y][x]->maxX, tri->screenSegments[y][x]->minY);
			glEnd();
		}
	}

	// Lines
	glColor3f(1.0, .0f, .0f);
	for (int y = 0; y < tri->columns; y++)
	{
		for (int x = 0; x < tri->rows; x++)
		{
			for each (Point* neighbour in tri->screenSegments[y][x]->point->neighbours)
			{
				glBegin(GL_LINES);
				glVertex2f(tri->screenSegments[y][x]->point->position->x, tri->screenSegments[y][x]->point->position->y);
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

	// Create and generate a delaunay triangle
	tri = new DelaunayTriangle();
	tri->GenerateDelaunay(WIN_SIZE, 9, 9, false);

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