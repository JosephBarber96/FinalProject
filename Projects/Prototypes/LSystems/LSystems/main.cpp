#define _USE_MATH_DEFINES
#include <string>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>

#include <glut.h>

#include "LSystem.h"
#include "Rule.h"
#include "Vec2.h"
#include "Turtle.h"
#include "Line.h"
#include "Road.h"
#include "RoadTurtle.h"
#include "Utility.h"

LSystem lsys;

// Plants
Turtle turtle;
RoadTurtle roadTurtle;
float lineLengthForPlants;
float angleForPlants;
const int orthoY = 50;
const int orthoX = 50;
const int generationCountForPlants = 5;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(1.0f);

	// Turtle lines
	for (auto line : turtle.getLines())
	{
		glBegin(GL_LINES);
		glVertex2f(line->getStart()->getX(), line->getStart()->getY());
		glVertex2f(line->getEnd()->getX(), line->getEnd()->getY());
		glEnd();
	}

	// Roads
	for (auto road : roadTurtle.getRoads())
	{
		glBegin(GL_LINE_STRIP);
		for (auto vec : road->points)
		{
			glVertex2f(vec->getX(), vec->getY());
		}
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
	gluOrtho2D(-orthoX, orthoX, -orthoY, orthoY);
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

void TurtleDrawLSystem(LSystem &lsys, Turtle& turtle, int lineLength, float angle)
{
	float rotAngle;

	for (auto c : lsys.getSentence())
	{
		if (c == 'F' || c == 'G')
		{
			turtle.DrawLine(lineLength);
		}
		if (c == 'E')
		{
			turtle.DrawLine(lineLength * 10);
		}
		else if (c == '+')
		{
			turtle.Rotate(angle);
		}
		else if (c == '-')
		{
			turtle.Rotate(-angle);
		}
		else if (c == '~')
		{
			int rng = rand() % 100;

			if (rng < 50)
			{
				turtle.Rotate(angle);
			}
			else
			{
				turtle.Rotate(-angle);
			}
		}
		else if (c == '[')
		{
			turtle.Push();
		}
		else if (c == ']')
		{
			turtle.Pop();
		}
		else if (c == 'A')
		{
			int rng = rand() % 100;

			if (rng < 50)
			{
				rotAngle = 2;
			}
			else
			{
				rotAngle = -2;
			}

			turtle.Rotate(rotAngle);
		}
		else if (c == 'R')
		{
			// Negate the rotate angle (restore rotation)
			//rotAngle *= -1;
			//turtle.Rotate(rotAngle);
		}
	}
}

void RoadDrawLSystem(LSystem& lsys, RoadTurtle& turtle, int lineLength, float angle)
{
	float rotAngle = 0;

	for (auto c : lsys.getSentence())
	{
		if (c == 'F' || c == 'G')
		{
			turtle.ExtendRoad(lineLength);
		}
		else if (c == 'E')
		{
			turtle.ExtendRoad(lineLength * 10);
		}
		else if (c == 'W')
		{
			int rng = rand() % 10 + 5;

			turtle.ExtendRoad(lineLength * rng);
		}
		else if (c == '+')
		{
			turtle.Rotate(angle);
		}
		else if (c == '-')
		{
			turtle.Rotate(-angle);
		}
		else if (c == '~')
		{
			int rng = rand() % 100;

			if (rng < 50)
			{
				turtle.Rotate(angle);
			}
			else
			{
				turtle.Rotate(-angle);
			}
		}
		else if (c == '[')
		{
			turtle.Push();
		}
		else if (c == ']')
		{
			turtle.Pop();
		}
		else if (c == 'A')
		{
			int rng = rand() % 100;

			if (rng < 33)
			{
				rotAngle = -2;
			}
			else if (rng < 66)
			{
				rotAngle = 0;
			}
			else
			{
				rotAngle = 2;
			}

			//if (rng < 50)
			//{
			//	rotAngle = 2;
			//}
			//else
			//{
			//	rotAngle = -2;
			//}

			turtle.Rotate(rotAngle);
		}
		//else if (c == 'B')
		//{
		//	turtle.Branch();
		//}

	}
}

void PruneRoads(RoadTurtle& turtle, float minLength)
{
	// Loop through all roads and mark any appropriate roads for deletion
	for (int i = 0; i < turtle.getRoads().size()-1; i++)
	{
		if (turtle.getRoads()[i]->TotalLength() < minLength)
		{
			turtle.getRoads()[i]->markedForDeletion = true;
		}
	}

	std::vector<Road*> list = turtle.getRoads();
	std::vector<Road*>::iterator it = list.begin();

	for (it; it != list.end();)
	{
		if ((*it)->markedForDeletion)
		{
			it = list.erase(it);
		}
		else
			++it;
	}

	turtle.SetNewList(list);
}

void ForPlants()
{

	turtle = Turtle();
	turtle.Reposition(0, -orthoY);
	// turtle.Reposition(-orthoSize / 2, -orthoSize / 2);
	turtle.FaceAngle(90);
	lineLengthForPlants = 1.0f;

	// KockCurve(lsys, angleForPlants);
	// SierpinskiTriangle(lsys, angleForPlants);
	// OriginalTree(lsys, angleForPlants);
	FractalPlant(lsys, angleForPlants);

	for (int i = 0; i < generationCountForPlants; i++)
	{
		lsys.Generate();
	}

	TurtleDrawLSystem(lsys, turtle, lineLengthForPlants, angleForPlants);
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	lsys = LSystem();

	// SimpleLSystemTest();
	ForPlants();

	//roadTurtle = RoadTurtle();
	//roadTurtle.SetStartingTransform(new Vec2(0, -orthoY + orthoY/10 ), 90);

	//int roadLength = 1.f;
	//float angle = 90.f;

	//// L system for roads

	//// original
	////F[+F]F[-F]F

	//// with branching
	////F[+FX]XF[-FX]F

	//// rotation
	////AFX[+FX]AFX[-FX]AFX

	//// random branching
	////AFX[~FX]AFX[~FX]AFX

	//int genCount = 6;

	//lsys.SetAxiom("X");
	//
	//// Minor roads attempt 1
	//lsys.AddRule('X', "AWEX[~FQ]AWEX[~FQ]AWEX");
	//lsys.AddRule('Q', "EW[~EEL]QEEQ[~EEL]Q");
	//lsys.AddRule('L', "ELELEL");


	// Minor roads attempt 2
	//lsys.AddRule('X', "X AW X [~Q] X");
	//lsys.AddRule('Q', "AFQ [~FQ] AWQ [~FQ]AFQ");


	//for (int i = 0; i < genCount; i++)
	//{
	//	lsys.Generate();
	//}

	//RoadDrawLSystem(lsys, roadTurtle, roadLength, angle);
	//PruneRoads(roadTurtle, 15.5f);


	
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