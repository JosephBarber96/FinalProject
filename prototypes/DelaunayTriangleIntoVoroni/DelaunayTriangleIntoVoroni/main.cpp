#include "DelaunayTriangle.h"
#include "Point.h"
#include "V2.h"
#include "Box.h"
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
	for each (Box box in tri->screenSegments)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2f(box.minX, box.minY);
		glVertex2f(box.minX, box.maxY);
		glVertex2f(box.maxX, box.maxY);
		glVertex2f(box.maxX, box.minY);
		glEnd();

		//glRectf(box.minX, box.maxX, box.minY, box.maxY);
	}

	// Lines
	glColor3f(1.0, .0f, .0f);
	for each (Point* point in tri->points)
	{
		for each (Point* neightbour in point->neighbours)
		{

			glBegin(GL_LINES);
			glVertex2f(point->position->x, point->position->y);
			glVertex2f(neightbour->position->x, neightbour->position->y);
			glEnd();
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
	tri->Generate(WIN_SIZE);

	for each (Point* point in tri->points)
	{
		std::cout << "Point position: " << point->position->x << ", " << point->position->y << std::endl;

		std::cout << "Point number of neighbours: " << point->neighbours.size() << std::endl;
	}

	// Init OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIN_SIZE, WIN_SIZE);
	glutCreateWindow("Delunary triangle.");
	InitGL();

	// Display
	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}