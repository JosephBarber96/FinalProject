#define _USE_MATH_DEFINES
#include <string>
#include <iostream>
#include <math.h>
#include <stdio.h>

#include "LSystem.h"
#include "Rule.h"
#include "Vec2.h"
#include "Turtle.h"


float DegreeToRadians(float degree)
{
	return degree * M_PI / 180;
}

Vec2 AngleToVector(float degree)
{
	// https://math.stackexchange.com/questions/180874/convert-angle-radians-to-a-heading-vector

	float rads = DegreeToRadians(degree);

	float x = cos(rads);
	float y = sin(rads);

	float roundedX = roundf(x * 100) / 100;
	float roundedY = roundf(y * 100) / 100;

	
	return Vec2(degree, roundedX, roundedY);
}

void DegreeToVectorTest()
{
	Vec2 degToVec = AngleToVector(180);
	std::cout << degToVec.getAngle() << " to vector: (" << degToVec.getX() << ", " << degToVec.getY() << ")" << std::endl;

	degToVec = AngleToVector(45);
	std::cout << degToVec.getAngle() << " to vector: (" << degToVec.getX() << ", " << degToVec.getY() << ")" << std::endl;

	degToVec = AngleToVector(-85);
	std::cout << degToVec.getAngle() << " to vector: (" << degToVec.getX() << ", " << degToVec.getY() << ")" << std::endl;
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

int main()
{
	// SimpleLSystemTest();
	DegreeToVectorTest();

	LSystem tree = LSystem("F");
	tree.AddRule(new Rule('F', "FF+[+F-F-F]-[-F+F+F]"));
	tree.Generate();

	Turtle turtle = Turtle();
	float lineLength = 0.5f;

	for (auto c : tree.getSentence())
	{
		if (c == 'F')
		{
			turtle.DrawLine(lineLength);
		}
		else if (c == '+')
		{
			turtle.Rotate(45);
		}
		else if (c == '-')
		{
			turtle.Rotate(-45);
		}
		else if (c == '[')
		{
			turtle.PushPosition();
		}
		else if (c == ']')
		{
			turtle.PopPosition();
		}
	}




	system("PAUSE");
	return 0;
}