#define _USE_MATH_DEFINES
#include <string>
#include <iostream>
#include <math.h>
#include <stdio.h>

#include <glut.h>

#include "LSystem.h"
#include "Rule.h"
#include "Vec2.h"
#include "Turtle.h"
#include "Line.h"

LSystem lsys;

// Plants
Turtle turtle;
float lineLengthForPlants;
float angleForPlants;
const int orthoSize = 22;
const int generationCountForPlants = 5;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(1.0f);
	for (auto line : turtle.getLines())
	{
		glBegin(GL_LINES);
		glVertex2f(line->getStart()->getX(), line->getStart()->getY());
		glVertex2f(line->getEnd()->getX(), line->getEnd()->getY());
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

	// Coordinate system
	// minx, maxx, miny, maxy
	gluOrtho2D(-orthoSize, orthoSize, -orthoSize, orthoSize);
}

void SimpleLSystemTest()
{
	LSystem lsys = LSystem("A");
	lsys.AddRule(new Rule('A', "AB"));
	lsys.AddRule(new Rule('B', "A"));

	std::cout << "Axiom (Gen 0): " << lsys.getAxiom() << std::endl;
	for (int i = 1; i <= 5; i++)
	{
		lsys.Generate();
		std::cout << "Generation " << i << ": " << lsys.getSentence() << std::endl;

	}
}

void KockCurve(LSystem &tree, float &angle)
{
	/*
	Variables:
		F = Draw Forward
		- = Rotate left
		+ = Rotate right
	*/

	angle = 90;

	tree.SetAxiom("F");
	tree.AddRule(new Rule('F', "F+F-F-F+F"));
}

void SierpinskiTriangle(LSystem &tree, float &angle)
{
	/*
	Variables:
		F = Draw Forward
		G = Draw Forward
		- = Rotate left
		+ = Rotate right
	*/

	angle = 120;

	tree.SetAxiom("F-G-G");
	tree.AddRule(new Rule('F', "F-G+F+G-F"));
	tree.AddRule(new Rule('G', "GG"));
}

void OriginalTree(LSystem &tree, float &angle)
{
	/*
	Variables:
		F = Draw Forward
		- = Rotate left
		+ = Rotate right
		[ = Push Position AND Angle
		] = Push Position AND Angle
	*/

	angle = 30;
	
	tree.SetAxiom("F");
	tree.AddRule(new Rule('F', "FF+[+F-F-F]-[-F+F+F]"));
}

void FractalPlant(LSystem &tree, float &angle)
{
	/*
	Variables:
		F = Draw Forward
		- = Rotate left
		+ = Rotate right
		[ = Push Position AND Angle
		] = Push Position AND Angle
		X - Simply controls the flow of the L ssystem.
	*/

	angle = 25;

	tree.SetAxiom("X");
	tree.AddRule(new Rule('X', "F[-X][X]F[-X]+FX"));
	tree.AddRule(new Rule('F', "FF"));
}

void Roads(LSystem &tree, float &angle)
{
	/*
	Variables:
		V = block_vertical
		U = block_horizontal
		X = road_vertical
		Y = road_horizontal
	*/

	tree.SetAxiom("V");
	tree.AddRule(new Rule('V', "UXU"));
	tree.AddRule(new Rule('U', "VYV"));
}

void TurtleTree(LSystem &lsys, Turtle& turtle, int lineLength, float angle)
{
	for (auto c : lsys.getSentence())
	{
		if (c == 'F' || c == 'G')
		{
			turtle.DrawLine(lineLength);
		}
		else if (c == '+')
		{
			turtle.Rotate(angle);
		}
		else if (c == '-')
		{
			turtle.Rotate(-angle);
		}
		else if (c == '[')
		{
			turtle.Push();
		}
		else if (c == ']')
		{
			turtle.Pop();
		}
	}
}

void ForPlants()
{
	turtle = Turtle();
	turtle.Reposition(0, -orthoSize);
	// turtle.Reposition(-orthoSize / 2, -orthoSize / 2);
	turtle.FaceAngle(90);
	lineLengthForPlants = 1.0f;

	// KockCurve(lsys, angleForPlants);
	// SierpinskiTriangle(lsys, angleForPlants);
	OriginalTree(lsys, angleForPlants);
	// FractalPlant(lsys, angleForPlants);

	for (int i = 0; i < generationCountForPlants; i++)
	{
		lsys.Generate();
	}

	TurtleTree(lsys, turtle, lineLengthForPlants, angleForPlants);
}

int main(int argc, char* argv[])
{
	lsys = LSystem();

	// SimpleLSystemTest();
	// ForPlants();



	
	// OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("L-systems");
	InitGl();

	glutDisplayFunc(Display);
	glutMainLoop();

	system("PAUSE");
	return 0;
}