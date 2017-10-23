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

	void FaceAngle(float deg);
	void Rotate(float deg);
	void Move(float length);
	void DrawLine(float length);
	void PushPosition();
	void PopPosition();
	void PushPositionAndAngle();
	void PopPositionAndAngle();

	std::vector<Line*> getLines() { return lines; }

private:

	void AddLine(Vec2* start, Vec2* end);

	float angle;
	Vec2* position;	
	std::stack<float> angleStack;
	std::stack<Vec2*> posStack;
	std::vector<Line*> lines;
};

