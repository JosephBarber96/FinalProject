#pragma once
#include <stack>
#include <vector>
class Vec2;
class Line;
class Turtle
{
public:
	Turtle();
	~Turtle();

	void Rotate(float deg);
	void DrawLine(float length);
	void PushPosition();
	void PopPosition();

	std::vector<Line*> getLines() { return lines; }

private:

	void AddLine(Vec2* start, Vec2* end);

	float angle;
	Vec2* position;	
	std::stack<Vec2*> posStack;
	std::vector<Line*> lines;
};

