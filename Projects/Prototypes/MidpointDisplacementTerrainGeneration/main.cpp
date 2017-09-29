#include "Camera.h"
#include "Vector3.h"
#include <glut.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>

#define WIN_SIZE 500
Camera* camera;

void DrawGrid()
{
	for (int y = -100; y < 100; y += 5)
	{
		for (int x = -100; x < 100; x += 5)
		{
			glBegin(GL_LINE_STRIP);
			glVertex3f(x, y, 0);
			glVertex3f(x, y + 2.5f, 0);
			glVertex3f(x + 2.5f, y + 2.5f, 0);
			glVertex3f(x + 2.5f, y, 0);
			glVertex3f(x, y, 0);
			glEnd();
		}
	}
}

void DrawFloorGrid()
{
	for (float i = -500; i <= 500; i += 5)
	{
		glBegin(GL_LINES);
		glColor3ub(150, 190, 150);
		glVertex3f(-500, 0, i);
		glVertex3f(500, 0, i);
		glVertex3f(i, 0, -500);
		glVertex3f(i, 0, 500);
		glEnd();
	}

	glColor3f(1.0, 0.0, 0.0);
}

void DrawCube()
{

	glColor3f(0.0, 1.0, 0.0);
	
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 50, 0);
	glVertex3f(50, 50, 0);
	glVertex3f(50, 0, 0);
	glEnd();

}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(
		camera->camPos->x, camera->camPos->y, camera->camPos->z,
		camera->camView->x, camera->camView->y, camera->camView->z,
		camera->camUp->x, camera->camUp->y, camera->camUp->z);

	DrawFloorGrid();
	//DrawGrid();
	//DrawCube();

	glutSwapBuffers();
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{

}

void InitGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Matrix mode
	//glMatrixMode(GL_PROJECTION);

	// Setup coordinate system
	glOrtho(-100, 100, -100, 100, -100, 100);

	camera->PositionCamera(
		0, 2.5f, 5, 
		0, 2.5f, 0, 
		0, 1, 0);
}

int main(int argc, char* argv[])
{
	// Give a seed to rand()
	srand(time(NULL));

	camera = new Camera();

	// Init OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIN_SIZE, WIN_SIZE);
	glutCreateWindow("Camera test");
	InitGL();

	// Display
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();

	return 0;
}

//#include <glut.h>
//
//#define WIN_SIZE 500
//
//void DrawCube()
//{
//	glBegin(GL_QUADS);
//
//	// Define vertices in counter-clockwise (CCW) order with normal pointing out
//	glColor3f(0.0f, 1.0f, 0.0f);		// Green
//	glVertex3f(1.0f, 1.0f, -1.0f);
//	glVertex3f(-1.0f, 1.0f, -1.0f);
//	glVertex3f(-1.0f, 1.0f, 1.0f);
//	glVertex3f(1.0f, 1.0f, 1.0f);
//
//	// Bottom face (y = -1.0f)
//	glColor3f(1.0f, 0.5f, 0.0f);		// Orange
//	glVertex3f(1.0f, -1.0f, 1.0f);
//	glVertex3f(-1.0f, -1.0f, 1.0f);
//	glVertex3f(-1.0f, -1.0f, -1.0f);
//	glVertex3f(1.0f, -1.0f, -1.0f);
//
//	// Front face  (z = 1.0f)
//	glColor3f(1.0f, 0.0f, 0.0f);		// Red
//	glVertex3f(1.0f, 1.0f, 1.0f);
//	glVertex3f(-1.0f, 1.0f, 1.0f);
//	glVertex3f(-1.0f, -1.0f, 1.0f);
//	glVertex3f(1.0f, -1.0f, 1.0f);
//
//	// Back face (z = -1.0f)
//	glColor3f(1.0f, 1.0f, 0.0f);		// Yellow
//	glVertex3f(1.0f, -1.0f, -1.0f);
//	glVertex3f(-1.0f, -1.0f, -1.0f);
//	glVertex3f(-1.0f, 1.0f, -1.0f);
//	glVertex3f(1.0f, 1.0f, -1.0f);
//
//	// Left face (x = -1.0f)
//	glColor3f(0.0f, 0.0f, 1.0f);		// Blue
//	glVertex3f(-1.0f, 1.0f, 1.0f);
//	glVertex3f(-1.0f, 1.0f, -1.0f);
//	glVertex3f(-1.0f, -1.0f, -1.0f);
//	glVertex3f(-1.0f, -1.0f, 1.0f);
//
//	// Right face (x = 1.0f)
//	glColor3f(1.0f, 0.0f, 1.0f);		// Magenta
//	glVertex3f(1.0f, 1.0f, -1.0f);
//	glVertex3f(1.0f, 1.0f, 1.0f);
//	glVertex3f(1.0f, -1.0f, 1.0f);
//	glVertex3f(1.0f, -1.0f, -1.0f);
//
//	glEnd();
//}
//
//void Display()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// Add perspective view
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(45.0, 1.0, 1.0, 9.0);
//
//	// moving the camera and target the same relative distance, creates a pan
//	gluLookAt(0.0, 6.0, 0.0, 
//		0.0, 0.0, -7.0, 
//		0, 1, 0);
//
//	// Switch back to Model view
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	//glClearColor(1.0, 1.0, 1.0, 0.0);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	DrawCube();
//
//	glutSwapBuffers();
//	glutPostRedisplay();
//}
//
//void InitGL()
//{
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glClearDepth(1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
//	glShadeModel(GL_SMOOTH);
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//	glMatrixMode(GL_PROJECTION);
//	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
//}
//
//void Keyboard(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 119: //w
//		break;
//	case 97: //a
//		break;
//	case 100: //d
//		break;
//	case 115: //s
//		break;
//	}
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE);
//	glutInitWindowSize(WIN_SIZE, WIN_SIZE);
//	glutCreateWindow("Camera test");
//
//	// Setup funcs
//	glutDisplayFunc(Display);
//	glutKeyboardFunc(Keyboard);
//
//	// Main loop
//	glutMainLoop();
//
//	return 0;
//}