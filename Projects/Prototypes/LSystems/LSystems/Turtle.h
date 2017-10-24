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

	void Reposition(Vec2* pos);
	void Reposition(int newX, int newY);
	void FaceAngle(float deg);
	void Rotate(float deg);
	void Move(float length);
	void DrawLine(float length);
	void Push();
	void Pop();
	void PushPosition();
	void PopPosition();
	void PushAngle();
	void PopAngle();

	std::vector<Line*> getLines() { return lines; }

private:

	void AddLine(Vec2* start, Vec2* end);

	float angle;
	Vec2* position;	
	std::stack<float> angleStack;
	std::stack<Vec2*> posStack;
	std::vector<Line*> lines;
};